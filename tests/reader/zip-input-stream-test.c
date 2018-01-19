#include <glib.h>
#include <gio/gio.h>

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <j2c/zip.h>
#include <j2c/zip-input-stream.h>
#include <j2c/jre.h>

static void
zip_input_stream_test_do_read (gpointer data, gpointer user_data)
{
  GDataInputStream *in = g_data_input_stream_new (data);
  g_object_unref (data);
  g_data_input_stream_set_byte_order (in, G_DATA_STREAM_BYTE_ORDER_BIG_ENDIAN);

  GError *error = NULL;
  guint32 result = g_data_input_stream_read_uint32 (in, NULL, &error);
  g_assert_null (error);
  g_assert_cmpuint (result, ==, 0xcafebabe);

  g_object_unref (in);
}

static void
zip_input_stream_test_read (void)
{
  GFile *rt = j2c_jre_get_file ("rt.jar"); 
  J2cZip *zip = j2c_zip_new (rt, NULL);
  g_object_unref (rt);

  gint64 len = j2c_zip_num_entries (zip, NULL);
  for (gint i = 0; i < len; i++)
    {
      gchar *name = j2c_zip_get_name (zip, i, NULL);
      if (g_str_has_suffix (name, ".class"))
	{
	  GError *error = NULL;
	  J2cZipInputStream *in = j2c_zip_input_stream_open (zip, i, &error);
	  g_assert_null (error);
	  zip_input_stream_test_do_read (in, NULL);
	}
      g_free (name);
    }
  g_object_unref (zip); 
}

static void
zip_input_stream_test_read_concurrent (void)
{
  GThreadPool *pool = g_thread_pool_new (zip_input_stream_test_do_read,
					 NULL,
					 10,
					 TRUE,
					 NULL);

  GFile *rt = j2c_jre_get_file ("rt.jar"); 
  J2cZip *zip = j2c_zip_new (rt, NULL);
  g_object_unref (rt);

  gint64 len = j2c_zip_num_entries (zip, NULL);
  for (gint i = 0; i < len; i++)
    {
      gchar *name = j2c_zip_get_name (zip, i, NULL);
      if (g_str_has_suffix (name, ".class"))
	{
	  GError *error = NULL;
	  J2cZipInputStream *in = j2c_zip_input_stream_open (zip, i, &error);
	  g_assert_null (error);
	  g_thread_pool_push (pool, in, NULL);
	}
      g_free (name);
    }
  g_object_unref (zip); 
  g_thread_pool_free (pool, FALSE, TRUE);
}

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/zip-input-stream/zip-input-stream-read", zip_input_stream_test_read);
  g_test_add_func ("/zip-input-stream/zip-input-stream-read-concurrent", zip_input_stream_test_read_concurrent);

  return g_test_run();
}
