#include <j2c/index.h>
#include <j2c/logger.h>
#include <j2c/jar-file.h>
#include <j2c/object-array.h>

#include <glib.h>

struct _J2cIndex
{
  GObject parent;

  J2cObjectArray *target_types;
  GTree *types;
  GTree *resources;

  J2cIndexedFile *main;

  GMutex m;
};

G_DEFINE_TYPE (J2cIndex, j2c_index, G_TYPE_OBJECT)

static gboolean j2c_index_free_nodes (gpointer key, gpointer data, gpointer user_data);
static gint     j2c_index_compare_nodes (gconstpointer a, gconstpointer b);

static void
j2c_index_dispose (GObject *object)
{
  J2cIndex *self = J2C_INDEX (object);

  g_mutex_lock (&self->m);
  if (self->target_types)
    g_clear_object (&self->target_types);
  if (self->main)
    g_clear_object (&self->main);

  if (self->types)
    {
      g_tree_foreach (self->types, j2c_index_free_nodes, NULL);
      g_tree_destroy (self->types);
      self->types = NULL;
    }
  if (self->resources)
    {
      g_tree_foreach (self->resources, j2c_index_free_nodes, NULL);
      g_tree_destroy (self->resources);
      self->resources = NULL;
    }
  g_mutex_unlock (&self->m);
  g_mutex_clear (&self->m);

  G_OBJECT_CLASS(j2c_index_parent_class)->dispose (object);
}

static void
j2c_index_class_init (J2cIndexClass *klass)
{
  G_OBJECT_CLASS (klass)->dispose = j2c_index_dispose;
}

static void
j2c_index_init (J2cIndex *self)
{
  self->target_types = NULL;
  self->types = NULL;
  self->resources = NULL;
  self->main = NULL;
  g_mutex_init (&self->m);
}

J2cIndex *
j2c_index_new (void)
{
  return g_object_new (J2C_TYPE_INDEX, NULL);
}

void
j2c_index_insert (J2cIndex *self, J2cIndexedFile *item, gboolean const target)
{
  g_return_if_fail (NULL != self);
  g_return_if_fail (NULL != item);

  g_mutex_lock (&self->m);

  gchar *key = j2c_indexed_file_get_name (item);
  GTree **tree = (J2C_FILE_TYPE_RESOURCE == j2c_indexed_file_get_file_type (item)) ?
      &self->resources : &self->types;

  if (!*tree)
    *tree = g_tree_new (j2c_index_compare_nodes);
  else
    {
      if (g_tree_lookup (*tree, key))
        {
          j2c_logger_warning ("Cannot add %s to an index more than once", key);
          goto end;
        }
    }

  g_tree_insert (*tree, key, g_object_ref(item));
  j2c_logger_fine ("Indexed %s: %s",
		   J2C_FILE_TYPE_RESOURCE == j2c_indexed_file_get_file_type (item) ?
		     "resource" : "type",
		   key);

  if (target)
    j2c_object_array_add (self->target_types, item);

  if (NULL != j2c_indexed_file_get_main(item))
    {
      if (self->main)
        j2c_logger_warning ("main method in type %s conflicts with main method in type %s",
                            key, j2c_indexed_file_get_name(self->main));
      else
        {
          self->main = g_object_ref (item);
          j2c_logger_fine ("main method found in type %s", key);
        }
    }

end:
  g_mutex_unlock (&self->m);
}

void
j2c_index_insert_file (J2cIndex *self, J2cReadable *file, gboolean const target)
{
  g_return_if_fail (NULL != self);
  g_return_if_fail (NULL != file);

  GError *error = NULL;
  J2cIndexedFile *item = j2c_indexed_file_new (file, &error);
  if (item)
    {
      j2c_index_insert (self, item, target);
    }
  else
    {
      j2c_logger_warning ("Could not create indexed file from %s: %s.",
			  j2c_readable_name (file),
			  error->message);
    }
}

static gboolean
j2c_index_free_nodes (gpointer key, gpointer data, gpointer user_data)
{
  if (data)
    g_object_unref (data);
  if (key)
    g_free (key);
  return FALSE;
}

static gint
j2c_index_compare_nodes (gconstpointer a, gconstpointer b)
{
  return g_strcmp0 ((const gchar *) a, (const gchar *) b);
}
