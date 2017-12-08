#include <j2c/index.h>
#include <j2c/logger.h>
#include <j2c/jar-file.h>

#include <glib.h>

struct _J2cIndex
{
  GTree *types;
  GTree *resources;
  GMutex m;
};

static gboolean j2c_index_free_nodes (gpointer key, gpointer data, gpointer user_data);
static gint     j2c_index_compare_nodes (gconstpointer a, gconstpointer b);

J2cIndex *
j2c_index_new (void)
{
  J2cIndex *ret = g_malloc (sizeof (J2cIndex));
  ret->types = NULL;
  ret->resources = NULL;
  g_mutex_init (&ret->m);

  return ret;
}

void
j2c_index_delete (J2cIndex *self)
{
  g_return_if_fail (NULL != self);

  if (self->types || self->resources)
    {
      g_mutex_lock (&self->m);

      if (self->types)
	{
	  g_tree_foreach (self->types, j2c_index_free_nodes, NULL);
	  g_tree_destroy (self->types);
	}
      if (self->resources)
      	{
      	  g_tree_foreach (self->resources, j2c_index_free_nodes, NULL);
      	  g_tree_destroy (self->resources);
      	}

      g_mutex_unlock (&self->m);
    }

  g_free (self);
}

void
j2c_index_insert (J2cIndex *self, J2cIndexedFile *item)
{
  g_return_if_fail (NULL != self);
  g_return_if_fail (NULL != item);

  g_mutex_lock (&self->m);

  gchar *key = j2c_indexed_file_get_name (item);
  GTree *tree = J2C_FILE_TYPE_RESOURCE == j2c_indexed_file_get_file_type (item) ?
      self->resources : self->types;

  if (!tree)
    tree = g_tree_new (j2c_index_compare_nodes);
  else
    {
      if (g_tree_lookup (tree, key))
	{
	  j2c_logger_warning ("Cannot add %s to an index more than once", key);
	  goto end;
	}
    }

  g_tree_insert (tree, key, item);

end:
  g_mutex_unlock (&self->m);
}

void
j2c_index_insert_file (J2cIndex *self, GFile *file)
{
  g_return_if_fail (NULL != self);
  g_return_if_fail (NULL != file);

  GError *error = NULL;
  J2cJarFile *jar_file = j2c_jar_file_new (file, &error);
  if (jar_file)
    {
      J2cReadable **children = j2c_jar_file_expand (jar_file);
      for (J2cReadable **cur = children; cur && *cur; cur++)
	{
	  J2cReadable *child = *cur;
	  J2cIndexedFile *item = j2c_indexed_file_new (child);
	  g_object_unref (child);
	  j2c_index_insert (self, item);
	}
      g_free (children);
    }
  else if (g_error_matches (error, J2C_JAR_FILE_ERROR, J2C_JAR_FILE_NOT_JAR_ERROR))
    {
      g_error_free (error);
      error = NULL;

      J2cReadable *readable = j2c_readable_new (file);
      if (!readable)
	{
	  j2c_logger_warning ("Error reading %s: %s",
			      g_file_get_path (file),
			      error->message);
	  g_error_free (error);
	  return;
	}

      J2cIndexedFile *item = j2c_indexed_file_new (readable);
      g_object_unref (readable);
      j2c_index_insert (self, item);
    }
  else
    {
      j2c_logger_warning("Error reading %s: %s", g_file_get_path (file),
			 error->message);
      g_error_free (error);
      return;
    }
}

static gboolean
j2c_index_free_nodes (gpointer key, gpointer data, gpointer user_data)
{
  if (data)
    g_object_unref (data);
  return FALSE;
}

static gint
j2c_index_compare_nodes (gconstpointer a, gconstpointer b)
{
  return g_strcmp0 ((const gchar *) a, (const gchar *) b);
}

