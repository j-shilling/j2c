#include <j2c/index.h>
#include <j2c/logger.h>
#include <j2c/jar-file.h>
#include <j2c/object-array.h>
#include <j2c/options.h>
#include <j2c/readable.h>
#include <j2c/reader.h>
#include <j2c/jre.h>

#include <glib.h>
#include <stdlib.h>

static struct 
{
  GThreadPool *insert_item;
  GThreadPool *read_item;

  J2cObjectArray *target_types;
  GTree *types;
  GTree *resources;

  J2cIndexedFile *main;

  GMutex m;
} index = {
    .insert_item = NULL,
    .read_item = NULL,
    .types = NULL,
    .resources = NULL,
    .main = NULL
};

typedef struct
{
  gpointer file;
  gboolean target;
} J2cInsertData;

typedef struct
{
  J2cIndexedFile *indexed_file;
  GWeakRef weak_ref;
} J2cIndexValue;

static void j2c_index_read (GFile *file, gboolean const target);
static void j2c_index_insert (J2cReadable *readable, gboolean const target);

/* GTree functions */
static gint j2c_index_key_compare (gconstpointer a, gconstpointer b, gpointer user_data);
static void j2c_index_key_destroy (gpointer data);
static void j2c_index_value_destroy (gpointer data);

/* GThreadPool functions */
static void j2c_index_insert_item (gpointer data, gpointer user_data);
static void j2c_index_read_item (gpointer data, gpointer user_data);

void
j2c_index_init (void)
{

  j2c_logger_heading ("indexing files");

  g_return_if_fail (index.insert_item == NULL);
  g_return_if_fail (index.read_item == NULL);
  g_return_if_fail (index.types == NULL);
  g_return_if_fail (index.resources == NULL);
  g_return_if_fail (index.target_types == NULL);

  index.insert_item = g_thread_pool_new (j2c_index_insert_item,
				  NULL,
				  j2c_options_max_threads(),
				  FALSE,
				  NULL);
  index.read_item = g_thread_pool_new (j2c_index_read_item,
				  NULL,
				  j2c_options_max_threads(),
				  FALSE,
				  NULL);
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


  GFile *jre = j2c_jre_get_lib_dir ();
  j2c_index_read (jre, FALSE);
  g_object_unref (jre);

  GFile **files = j2c_options_class_path_files ();
  for (GFile **cur = files; cur && *cur; cur++)
    {
      j2c_index_read (*cur, FALSE);
      g_object_unref (*cur);
    }
  g_free (files);

  files = j2c_options_target_files ();
  for (GFile **cur = files; cur && *cur; cur++)
    {
      j2c_index_read (*cur, TRUE);
      g_object_unref (*cur);
    }
  g_free (files);

  g_thread_pool_free (index.read_item, FALSE, TRUE);
  index.read_item = NULL;
  g_thread_pool_free (index.insert_item, FALSE, TRUE);
  index.insert_item = NULL;
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
j2c_index_read_item (gpointer data, gpointer user_data)
{
  g_return_if_fail (NULL != data);
  J2cInsertData *insert_data = data;
  GFile *file = insert_data->file;
  gboolean const target = insert_data->target;
  g_free (insert_data);

  J2cReadableList *list = j2c_readable_list_new();
  j2c_readable_list_add (list, file);
  GSList *readables = j2c_readable_list_finish (list);

  for (GSList *cur = readables; cur; cur = cur->next)
    j2c_index_insert (J2C_READABLE(cur->data), target);
}

static void
j2c_index_insert_item (gpointer data, gpointer user_data)
{
  g_return_if_fail (NULL != data);

  J2cInsertData *insert_data = data;
  J2cIndexedFile *item = insert_data->file;
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

static void
j2c_index_read (GFile *file, gboolean const target)
{
  g_return_if_fail (NULL != index.read_item);
  g_return_if_fail (NULL != file);

  J2cInsertData *data = g_malloc (sizeof (J2cInsertData));
  data->file = g_object_ref (file);
  data->target = target;

  g_thread_pool_push (index.read_item, data, NULL);
}

static void
j2c_index_insert (J2cReadable *readable, gboolean const target)
{
  g_return_if_fail (NULL != index.insert_item);
  g_return_if_fail (NULL != readable);

  GError *error = NULL;
  J2cIndexedFile *item = j2c_indexed_file_new (readable, &error);
  if (item)
    {
      J2cInsertData *data = g_malloc (sizeof (J2cInsertData));
      data->file = g_object_ref(item);
      data->target = target;
      g_object_unref (item);

      g_thread_pool_push (index.insert_item, data, NULL);
    }
  else
    {
      j2c_logger_warning ("Could not create indexed file from %s: %s.",
			  j2c_readable_name (readable),
			  error->message);
    }
}
