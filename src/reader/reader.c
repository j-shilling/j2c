#include <j2c/reader.h>
#include <j2c/readable.h>
#include <j2c/logger.h>
#include <j2c/jar-file.h>

#include <glib-object.h>

static void j2c_readable_list_add_file (J2cReadableList *self, GFile *file);

J2cReadableList *
j2c_readable_list_new (void)
{
  J2cReadableList *ret = g_malloc (sizeof (J2cReadableList));
  ret->list = NULL;
  g_mutex_init (&ret->m);

  return ret;
}

void
j2c_readable_list_destroy (J2cReadableList *self)
{
  g_return_if_fail (NULL != self);

  g_mutex_lock (&self->m);
  g_slist_free_full (self->list, g_object_unref);
  g_mutex_unlock (&self->m);
  g_mutex_clear (&self->m);

  g_free (self);
}

void
j2c_readable_list_add (J2cReadableList *self, gchar const *const filename)
{
  g_return_if_fail (NULL != self);
  g_return_if_fail (NULL != filename && *filename != '\0');

  GError *error = NULL;

  j2c_logger_finest ("Trying to read %s", filename);

  GFile *file = g_file_new_for_commandline_arg (filename);
  GFileInfo *info = g_file_query_info (file,
				       G_FILE_ATTRIBUTE_STANDARD_TYPE ","
				       G_FILE_ATTRIBUTE_ACCESS_CAN_READ,
				       G_FILE_QUERY_INFO_NONE,
				       NULL,
				       &error);
  if (!info)
    {
      j2c_logger_warning ("Could not get info for \'%s\': %s",
			  filename,
			  error ? error->message : "Unkown error.");
      g_error_free (error);
      g_object_unref (file);
      return;
    }

  if (!g_file_info_get_attribute_boolean (info, G_FILE_ATTRIBUTE_ACCESS_CAN_READ))
    {
      j2c_logger_warning ("Cannot read file \'%s\'", g_file_get_path (file));
      g_object_unref (info);
      g_object_unref (file);
      return;
    }

  if (G_FILE_TYPE_DIRECTORY !=
      g_file_info_get_attribute_uint32 (info, G_FILE_ATTRIBUTE_STANDARD_TYPE))
    {
      j2c_readable_list_add_file (self, file);
    }
  else
    {
      j2c_logger_fine ("Entering directory %s", g_file_get_path (file));
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
			      g_file_get_path (file),
			      error->message);
	  g_error_free (error);
	  g_object_unref (info);
	  g_object_unref (file);
	  return;
	}

      GFile *child = NULL;
      GFileInfo *child_info = NULL;
      do
	{
	  if (!g_file_enumerator_iterate (direnum, &child_info, &child, NULL, &error))
	    {
	      j2c_logger_warning ("Error reading children of \'%s\': %s",
				  g_file_get_path (file),
				  error ? error->message : "Unkown error.");
	      g_error_free (error);
	      g_object_unref (file);
	      g_object_unref (info);
	      g_object_unref (direnum);
	      return;
	    }

	  if (child && child_info)
	    {
	      if (!g_file_info_get_attribute_boolean (child_info, G_FILE_ATTRIBUTE_ACCESS_CAN_READ))
		{
		  j2c_logger_warning ("Cannot read file \'%s\'",
				      g_file_get_path (child));
		  continue;
		}

	      if (G_FILE_TYPE_DIRECTORY ==
		  g_file_info_get_attribute_uint32 (child_info, G_FILE_ATTRIBUTE_STANDARD_TYPE))
		{
		  j2c_logger_fine ("Ignoring subdirectory \'%s\'.",
				   g_file_get_path (child));
		  continue;
		}

	      j2c_readable_list_add_file (self, child);
	    }
	}
      while (child && child_info);

      g_object_unref (file);
      g_object_unref (direnum);
    }

  g_object_unref (info);
  g_object_unref (file);
}

static void
j2c_readable_list_add_file (J2cReadableList *self, GFile *file)
{
  g_return_if_fail (NULL != self);
  g_return_if_fail (NULL != file);

  GError *error = NULL;
  J2cJarFile *jar = j2c_jar_file_new (file, &error);
  if (!jar)
    {
      if (!error || g_error_matches (error, J2C_JAR_FILE_ERROR, J2C_JAR_FILE_NOT_JAR_ERROR))
	{
	  J2cReadable *readable = j2c_readable_new (file);

	  g_mutex_lock (&self->m);
	  self->list = g_slist_prepend (self->list, readable);
	  g_mutex_unlock (&self->m);

	  j2c_logger_fine ("Read %s", j2c_readable_name (readable));
	  return;
	}
      else
	{
	  j2c_logger_warning ("Error trying to test for jar file format: %s",
			      error->message);
	  g_error_free (error);
	  return;
	}
    }
  else
    {
      J2cReadable **contents = j2c_jar_file_expand (jar);
      for (J2cReadable **cur = contents; *cur != NULL; cur ++)
	{
	  g_mutex_lock (&self->m);
	  self->list = g_slist_prepend (self->list, g_object_ref(*cur));
	  g_mutex_unlock (&self->m);

	  j2c_logger_fine ("Read %s", j2c_readable_name (*cur));
	  g_clear_object (cur);
	}
      g_free (contents);
      g_object_unref (jar);
      return;
    }
}
