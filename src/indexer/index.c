#include <j2c/index.h>
#include <j2c/logger.h>
#include <j2c/jar-file.h>
#include <j2c/object-array.h>

#include <glib.h>
#include <stdlib.h>

static struct 
{
  GThreadPool *pool;

  J2cObjectArray *target_types;
  GTree *types;
  GTree *resources;

  J2cIndexedFile *main;

  GMutex m;
} index = {
    .pool = NULL,
    .types = NULL,
    .resources = NULL,
    .main = NULL
};

typedef struct
{
  J2cIndexedFile *indexed_file;
  gboolean target;
} J2cInsertData;

typedef struct
{
  J2cIndexedFile *indexed_file;
  GWeakRef weak_ref;
} J2cIndexValue;

static gboolean j2c_index_free_nodes (gpointer key, gpointer data, gpointer user_data);
static gint     j2c_index_compare_nodes (gconstpointer a, gconstpointer b);

/* GTree functions */
static gint j2c_index_key_compare (gconstpointer a, gconstpointer b, gpointer user_data);
static void j2c_index_key_destroy (gpointer data);
static void j2c_index_value_destroy (gpointer data);

/* GThreadPool functions */
static void j2c_index_insert_item (gpointer data, gpointer user_data);

void
j2c_index_init (gint max_threads)
{
  g_return_if_fail (index.pool == NULL);
  g_return_if_fail (index.types == NULL);
  g_return_if_fail (index.resources == NULL);
  g_return_if_fail (index.target_types == NULL);

  GError *error = NULL;
  index.pool = g_thread_pool_new (j2c_index_insert_item,
				  NULL,
				  max_threads > 0 ? max_threads : -1,
				  TRUE,
				  &error);
  if (error)
    {
      j2c_logger_fatal ("%s", error->message);
      exit (EXIT_FAILURE);
    }

  index.types = g_tree_new_full (j2c_index_key_compare,
				 NULL,
				 j2c_index_key_destroy,
				 j2c_index_value_destroy);

  index.resources = g_tree_new_full (j2c_index_key_compare,
		  		     NULL,
				     j2c_index_key_destroy,
				     j2c_index_value_destroy);

  index.target_types = j2c_object_array_new ();

  g_mutex_init (&index.m);
}

void
j2c_index_insert (J2cReadable *readable, gboolean const target)
{
  g_return_if_fail (NULL != index.pool);
  g_return_if_fail (NULL != readable);

  GError *error = NULL;
  J2cIndexedFile *item = j2c_indexed_file_new (readable, &error);
  if (item)
    {
      J2cInsertData *data = g_malloc (sizeof (J2cInsertData));
      data->indexed_file = g_object_ref(item);
      data->target = target;
      g_object_unref (item);

      g_thread_pool_push (index.pool, data, &error);
      if (error)
	{
	  j2c_logger_fatal ("%s", error->message);
	  exit (EXIT_FAILURE);
	}
    }
  else
    {
      j2c_logger_warning ("Could not create indexed file from %s: %s.",
			  j2c_readable_name (readable),
			  error->message);
    }
}

void
j2c_index_lock (void)
{
  g_return_if_fail (index.pool != NULL);

  g_thread_pool_free (index.pool, FALSE, TRUE);
  index.pool = NULL;
}

J2cIndexedFile *
j2c_index_get_main (void)
{
  if (index.main)
    return g_object_ref(index.main);
  else
    return NULL;
}

J2cCompilationUnit *
j2c_index_get_compilation_unit (gchar *java_name)
{
  J2cIndexValue *value = g_tree_lookup (index.types, java_name);
  if (!value)
    return NULL;

  J2cCompilationUnit *ret = g_weak_ref_get (&value->weak_ref);
  if (!ret)
    {
      GError *error = NULL;
      ret = j2c_compilation_unit_new (value->indexed_file, &error);
      if (error)
	{
	  gchar *name = j2c_indexed_file_get_name (value->indexed_file);
	  j2c_logger_severe ("Could not created a J2cCompilationUnit from \'%s\': ",
			     name, error->message);
	  g_free (name);
	  g_error_free (error);

	  return NULL;
	}
      g_weak_ref_set (&value->weak_ref, G_OBJECT (ret));
    }

  return ret;
}

/* GThreadPool functions */
static void
j2c_index_insert_item (gpointer data, gpointer user_data)
{
  g_return_if_fail (NULL != data);

  J2cInsertData *insert_data = data;
  J2cIndexedFile *item = insert_data->indexed_file;
  gboolean const target = insert_data->target;
  g_free (insert_data);

  g_return_if_fail (NULL != item);
  g_return_if_fail (NULL != index.types);
  g_return_if_fail (NULL != index.resources);

  g_mutex_lock (&index.m);

  gchar *key = j2c_indexed_file_get_name (item);
  GTree **tree = (J2C_FILE_TYPE_RESOURCE == j2c_indexed_file_get_file_type (item)) ?
      &index.resources : &index.types;

  if (g_tree_lookup (*tree, key))
    {
      j2c_logger_warning ("Cannot add %s to an index more than once", key);
      g_free (key);
      goto end;
    }

  J2cIndexValue *value = g_malloc (sizeof (J2cIndexValue));
  value->indexed_file = g_object_ref (item);
  g_weak_ref_init (&value->weak_ref, NULL);

  g_tree_insert (*tree, key, value);
  j2c_logger_fine ("Indexed %s: %s",
		   J2C_FILE_TYPE_RESOURCE == j2c_indexed_file_get_file_type (item) ?
		     "resource" : "type",
		   key);

  if (target)
    {
      j2c_object_array_add (index.target_types, item);

      if (NULL != j2c_indexed_file_get_main(item))
	{
	  if (index.main)
	    j2c_logger_warning ("main method in type %s conflicts with main method in type %s",
				key, j2c_indexed_file_get_name(index.main));
	  else
	    {
	      index.main = g_object_ref (item);
	      j2c_logger_fine ("main method found in type %s", key);
	    }
	}
    }

end:
  g_mutex_unlock (&index.m);
  g_object_unref (item);
}

/* GTree functions */
static gint
j2c_index_key_compare (gconstpointer a, gconstpointer b, gpointer user_data)
{
  return g_strcmp0 ((const gchar *)a, (const gchar *)b);
}

static void
j2c_index_key_destroy (gpointer data)
{
  if (data)
    g_free (data);
}

static void
j2c_index_value_destroy (gpointer data)
{
  if (data)
    {
      J2cIndexValue *value = data;
      g_object_unref (value->indexed_file);
      g_weak_ref_clear (&value->weak_ref);
      g_free (value);
    }
}
