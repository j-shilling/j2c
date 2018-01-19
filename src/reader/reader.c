#include <j2c/reader.h>
#include <j2c/readable.h>
#include <j2c/logger.h>
#include <j2c/jar-file.h>
#include <j2c/options.h>

struct _J2cReadableList
{
  GSList *list;
  GMutex mutex;
  GThreadPool *expand_dirs;
  GThreadPool *expand_jars;
  GThreadPool *insert;
};

static void
j2c_readable_list_insert (gpointer data, gpointer user_data)
{
  J2cReadableList *list = user_data;
  J2cReadable *readable = J2C_READABLE (data);

  g_mutex_lock (&list->mutex);
  list->list = g_slist_prepend (list->list, readable);
  g_mutex_unlock (&list->mutex);

  j2c_logger_fine ("Read %s", j2c_readable_name (readable));
}

static void
j2c_readable_list_expand_jars (gpointer data, gpointer user_data)
{
  J2cReadableList *list = user_data;
  g_return_if_fail (list->insert != NULL);

  GFile *file = G_FILE (data);

  GError *error = NULL;
  J2cJarFile *jar = j2c_jar_file_new (file, &error);
  if (!jar)
    {
      if (!error || g_error_matches (error, J2C_JAR_FILE_ERROR, J2C_JAR_FILE_NOT_JAR_ERROR))
	{
	  J2cReadable *readable = j2c_readable_new (file);

	  g_thread_pool_push (list->insert, readable, NULL);
	}
      else
	{
	  j2c_logger_warning ("Error trying to test for jar file format: %s",
			      error->message);
	  g_error_free (error);
	}
    }
  else
    {
      J2cReadable **contents = j2c_jar_file_expand (jar, NULL);
      for (J2cReadable **cur = contents; *cur != NULL; cur ++)
	g_thread_pool_push (list->insert, *cur, NULL);

      g_free (contents);
      g_object_unref (jar);
    }

  g_object_unref (file);
}

static void
j2c_readable_list_expand_dirs (gpointer data, gpointer user_data)
{
  J2cReadableList *list = user_data;
  g_return_if_fail (NULL != data);
  g_return_if_fail (list->expand_jars != NULL);

  GFile *file = data;

  GError *error = NULL;
  gchar *path = g_file_get_path (file);
  j2c_logger_finest ("Trying to read %s", path);

  GFileInfo *info = g_file_query_info (file,
				       G_FILE_ATTRIBUTE_STANDARD_TYPE ","
				       G_FILE_ATTRIBUTE_ACCESS_CAN_READ,
				       G_FILE_QUERY_INFO_NONE,
				       NULL,
				       &error);
  if (!info)
    {
      j2c_logger_warning ("Could not get info for \'%s\': %s",
			  path,
			  error ? error->message : "Unkown error.");
      g_error_free (error);
      g_object_unref (file);
      g_free (path);
      return;
    }

  if (!g_file_info_get_attribute_boolean (info, G_FILE_ATTRIBUTE_ACCESS_CAN_READ))
    {
      j2c_logger_warning ("Cannot read file \'%s\'", path);
      g_object_unref (info);
      g_object_unref (file);
      g_free (path);
      return;
    }

  if (G_FILE_TYPE_DIRECTORY !=
      g_file_info_get_attribute_uint32 (info, G_FILE_ATTRIBUTE_STANDARD_TYPE))
    {
      g_thread_pool_push (list->expand_jars, file, NULL);
    }
  else
    {
      j2c_logger_fine ("Entering directory %s", path);
      GFileEnumerator *direnum = g_file_enumerate_children (file,
							    G_FILE_ATTRIBUTE_STANDARD_TYPE ","
							    G_FILE_ATTRIBUTE_STANDARD_NAME ","
							    G_FILE_ATTRIBUTE_ACCESS_CAN_READ,
							    G_FILE_QUERY_INFO_NONE,
							    NULL,
							    &error);
      if (error)
	{
	  j2c_logger_warning ("Cannot read directory \'%s\': %s",
			      path,
			      error->message);
	  g_error_free (error);
	  g_object_unref (info);
          g_object_unref (file);
	  g_free (path);
	  return;
	}

      GFile *child = NULL;
      GFileInfo *child_info = NULL;
      do
	{
	  if (!g_file_enumerator_iterate (direnum, &child_info, &child, NULL, &error))
	    {
	      j2c_logger_warning ("Error reading children of \'%s\': %s",
				  path,
				  error ? error->message : "Unkown error.");
	      g_error_free (error);
	      g_object_unref (info);
	      g_object_unref (direnum);
              g_object_unref (file);
	      g_free (path);
	      return;
	    }

	  if (child && child_info)
	    {
	      if (G_FILE_TYPE_DIRECTORY ==
		  g_file_info_get_attribute_uint32 (child_info, G_FILE_ATTRIBUTE_STANDARD_TYPE))
		{
		  gchar *child_path = g_file_get_path (child);
		  j2c_logger_fine ("Ignoring subdirectory %s", child_path);
		  g_free (child_path);
		  continue;
		}

	      if (!g_file_info_get_attribute_boolean (child_info, G_FILE_ATTRIBUTE_ACCESS_CAN_READ))
		{
		  gchar *child_path = g_file_get_path (child);
		  j2c_logger_warning ("Cannot read file \'%s\'",
				      child_path);
		  g_free (child_path);
		  continue;
		}

	      g_thread_pool_push (list->expand_jars, g_object_ref(child), NULL);
	    }
	}
      while (child && child_info);

      g_object_unref (file);
      g_object_unref (direnum);
    }

  g_object_unref (info);
  g_object_unref (file);
}

J2cReadableList *
j2c_readable_list_new (void)
{
  J2cReadableList *list = g_malloc (sizeof (J2cReadableList));

  g_mutex_init (&list->mutex);
  list->expand_dirs = g_thread_pool_new (j2c_readable_list_expand_dirs,
					 list,
					 j2c_options_max_threads(),
					 FALSE,
					 NULL);
  list->expand_jars = g_thread_pool_new (j2c_readable_list_expand_jars,
 					 list,
					 j2c_options_max_threads(),
					 FALSE,
				 	 NULL);
  list->insert = g_thread_pool_new (j2c_readable_list_insert,
				    list,
				    j2c_options_max_threads(),
				    FALSE,
				    NULL);
  return list;
}

GSList *
j2c_readable_list_finish (J2cReadableList *list)
{
  g_return_val_if_fail (list->insert != NULL, NULL);
  g_return_val_if_fail (list->expand_dirs != NULL, NULL);
  g_return_val_if_fail (list->expand_jars != NULL, NULL);

  g_thread_pool_free (list->expand_dirs, FALSE, TRUE);
  list->expand_dirs = NULL;
  g_thread_pool_free (list->expand_jars, FALSE, TRUE);
  list->expand_jars = NULL;
  g_thread_pool_free (list->insert, FALSE, TRUE);
  list->insert = NULL;
  g_mutex_clear (&list->mutex);

  GSList *ret = list->list;
  list->list = NULL;
  return ret;
}

void
j2c_readable_list_add (J2cReadableList *list, GFile *file)
{
  g_return_if_fail (NULL != list);
  g_return_if_fail (NULL != list->expand_dirs);
  g_return_if_fail (NULL != file);

  g_thread_pool_push (list->expand_dirs, g_object_ref (file), NULL);
}
