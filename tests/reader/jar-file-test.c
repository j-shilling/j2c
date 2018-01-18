#include <glib.h>
#include <gio/gio.h>

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <j2c/zip-input-stream.h>
#include <j2c/jar-file-priv.h>
#include <j2c/jre.h>

static void
jar_file_test_new (void)
{
  GFile *rt = j2c_jre_get_file ("rt.jar"); 
  GError *error = NULL;
  J2cJarFile *jar = j2c_jar_file_new (rt, &error);
  g_assert_null (error);
  g_assert_nonnull (jar);
  g_object_unref (rt);
  g_object_unref (jar);

  GFile *me = g_file_new_for_path ("./jar-file-test.c");
  jar = j2c_jar_file_new (me, &error);
  g_assert_null (jar);
  g_assert_nonnull (error);

  g_assert_true (g_error_matches (error, J2C_JAR_FILE_ERROR, J2C_JAR_FILE_NOT_JAR_ERROR));
  g_error_free (error);
}

static void
jar_file_test_do_read (gpointer data, gpointer user_data)
{
  GError *error = NULL;
  GInputStream *input_stream = j2c_readable_read (data, &error);
  g_assert_null (error);

  GDataInputStream *in = g_data_input_stream_new (input_stream);
  g_object_unref (data);
  g_data_input_stream_set_byte_order (in, G_DATA_STREAM_BYTE_ORDER_BIG_ENDIAN);

  guint32 result = g_data_input_stream_read_uint32 (in, NULL, &error);
  g_assert_null (error);
  g_assert_cmpuint (result, ==, 0xcafebabe);

  g_object_unref (in);
}

static void
jar_file_test_read (void)
{
  GFile *rt = j2c_jre_get_file ("rt.jar"); 
  J2cJarFile *jar = j2c_jar_file_new (rt, NULL);
  g_object_unref (rt);

  GError *error = NULL;
  J2cReadable **items = j2c_jar_file_expand (jar, &error);
  g_assert_null (error);
  g_assert_nonnull (items);

  for (J2cReadable **cur = items; cur && *cur; cur ++)
    {
      J2cReadable *item = *cur;
      const gchar *name = j2c_readable_name (item);
      if (g_str_has_suffix (name, ".class"))
	jar_file_test_do_read (item, NULL);
      else
	g_object_unref (item);
    }
  g_free (items);
  g_object_unref (jar); 
}

static void
jar_file_test_read_concurrent (void)
{
  GThreadPool *pool = g_thread_pool_new (jar_file_test_do_read,
					 NULL,
					 10,
					 TRUE,
					 NULL);

  GFile *rt = j2c_jre_get_file ("rt.jar"); 
  J2cJarFile *jar = j2c_jar_file_new (rt, NULL);
  g_object_unref (rt);

  GError *error = NULL;
  J2cReadable **items = j2c_jar_file_expand (jar, &error);
  g_assert_null (error);
  g_assert_nonnull (items);

  for (J2cReadable **cur = items; cur && *cur; cur ++)
    {
      J2cReadable *item = *cur;
      const gchar *name = j2c_readable_name (item);
      if (g_str_has_suffix (name, ".class"))
	g_thread_pool_push (pool, item, NULL);
      else
	g_object_unref (item);
    }
  g_free (items);
  g_object_unref (jar);
  g_thread_pool_free (pool, FALSE, TRUE);
}

static void
jar_file_do_open_check (gpointer data, gpointer user_data)
{
  J2cZip *zip = j2c_jar_file_open (J2C_JAR_FILE (data), NULL);
  g_assert_true (zip == user_data);
  g_object_unref (zip);
}

static void
jar_file_test_open_concurrent (void)
{
  GFile *rt = j2c_jre_get_file ("rt.jar"); 
  J2cJarFile *jar = j2c_jar_file_new (rt, NULL);
  g_object_unref (rt);

  GError *error = NULL;
  J2cZip *zip = j2c_jar_file_open (jar, &error);
  g_assert_null (error);

  GThreadPool *pool = g_thread_pool_new (jar_file_do_open_check,
					 zip,
					 10,
					 TRUE,
					 NULL);

  for (gint i = 0; i < 30; i ++)
    g_thread_pool_push (pool, jar, NULL);
  g_thread_pool_free (pool, FALSE, TRUE);

  g_object_unref (zip);
  g_object_unref (jar);
}

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/jar-file/jar-file-new", jar_file_test_new);
  g_test_add_func ("/jar-file/jar-file-read", jar_file_test_read);
  g_test_add_func ("/jar-file/jar-file-read-concurrent", jar_file_test_read_concurrent);
  g_test_add_func ("/jar-file/jar-file-open-concurrent", jar_file_test_open_concurrent);

  return g_test_run();
}
