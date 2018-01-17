#include <glib.h>
#include <gio/gio.h>

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <j2c/zip.h>
#include <j2c/jre.h>

static void
zip_test_zip_new (void)
{
  GError *error = NULL;
  GFile *rt = j2c_jre_get_file ("rt.jar"); 
  J2cZip *zip = j2c_zip_new (rt, &error);
  g_object_unref (rt);
  g_assert_null (error);
  g_assert_nonnull (zip);

  g_object_unref (zip);
}

static void
zip_test_zip_get_name(void)
{
  GError *error = NULL;
  GFile *rt = j2c_jre_get_file ("rt.jar"); 
  J2cZip *zip = j2c_zip_new (rt, NULL);
  g_object_unref (rt);

  gint64 len = j2c_zip_num_entries (zip, NULL);
  for (gint i = 0; i < len; i++)
    {
      gchar *name = j2c_zip_get_name (zip, i, &error);
      g_assert_null (error);
      g_assert_nonnull (name);
      g_free (name);
    }

  g_object_unref (zip);
}

static void
zip_test_zip_close(void)
{
  GError *error = NULL;
  GFile *rt = j2c_jre_get_file ("rt.jar"); 
  J2cZip *zip = j2c_zip_new (rt, NULL);
  g_object_unref (rt);

  j2c_zip_close (zip, &error);
  g_assert_null (error);
  g_object_unref (zip);
}

static void
zip_file_test_open_close (void)
{
  GError *error = NULL;
  GFile *rt = j2c_jre_get_file ("rt.jar"); 
  J2cZip *zip = j2c_zip_new (rt, NULL);
  g_object_unref (rt);

  gint64 len = j2c_zip_num_entries (zip, NULL);
  for (gint i = 0; i < len; i++)
    {
      J2cZipFile *file = j2c_zip_file_open (zip, i, &error);
      g_assert_null (error);
      g_assert_nonnull (file);

      gboolean stat = j2c_zip_file_close (file, &error);
      g_assert_null (error);
      g_assert_true (stat);

      g_object_unref (file);
    }

  g_object_unref (zip);
}

static void
zip_file_test_do_read (gpointer data, gpointer user_data)
{
  J2cZipFile *file = data;

  GError *error = NULL;
  gchar *buf = g_malloc (10);
  j2c_zip_file_read (file,  buf, 10, &error);
  g_assert_null (error);

  g_object_unref (file);
}

static void
zip_file_test_read_concurrent (void)
{
  GThreadPool *pool = g_thread_pool_new (zip_file_test_do_read,
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
      if (name[strlen (name) - 1] != '/')
	{
	  J2cZipFile *file = j2c_zip_file_open (zip, i, NULL);
	  g_thread_pool_push (pool, file, NULL);
	}
      g_free (name);
    }
  g_thread_pool_free (pool, FALSE, TRUE);
  g_object_unref (zip); 
}

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/zip/zip-new", zip_test_zip_new);
  g_test_add_func ("/zip/zip-name", zip_test_zip_get_name);
  g_test_add_func ("/zip/zip-close", zip_test_zip_close);
  g_test_add_func ("/zip-file/zip-file-open-close", zip_file_test_open_close);
  g_test_add_func ("/zip-file/zip-file-read-concurrent", zip_file_test_read_concurrent);

  return g_test_run();
}
