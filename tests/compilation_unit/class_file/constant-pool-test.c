#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

#include <j2c/constant-pool.h>
#include <j2c/constant-pool-item.h>

static void
parse_start_element (GMarkupParseContext *context,
		     const gchar *element_name,
		     const gchar **attribute_names,
		     const gchar **attribute_values,
		     gpointer user_data,
		     GError **error)
{

}

static void
parse_text (GMarkupParseContext *context,
	    const gchar *text,
	    gsize text_len,
	    gpointer user_data,
	    GError **error)
{

}

static void
collect_children (GPtrArray *array, GFile *file)
{
  g_return_if_fail (array != NULL);
  g_return_if_fail (file != NULL);
  g_return_if_fail (g_file_query_exists (file, NULL));
  g_return_if_fail (g_file_query_file_type (file, G_FILE_QUERY_INFO_NONE, NULL) == G_FILE_TYPE_DIRECTORY);

  GError *error = NULL;
  GFileEnumerator *direnum = g_file_enumerate_children (file,
							G_FILE_ATTRIBUTE_STANDARD_TYPE,
							G_FILE_QUERY_INFO_NONE,
							NULL,
							&error);
  if (error)
    goto error;

  while (TRUE)
    {
      GFile *child = NULL;
      GFileInfo *info = NULL;

      if (!g_file_enumerator_iterate (direnum, &info, &child, NULL, &error))
	goto error;
      if (!info)
	break;

      if (g_file_info_get_attribute_uint32 (info, G_FILE_ATTRIBUTE_STANDARD_TYPE)
	    == G_FILE_TYPE_DIRECTORY)
	{
	  collect_children (array, child);
	}
      else
	{
	  g_ptr_array_add (array, g_object_ref (child));
	}

    }

  g_object_unref (direnum);

error:
  if (error)
    {
      fprintf (stderr, "%s", error->message);
      exit (EXIT_FAILURE);
    }

}

static void
test_constant_pool (gconstpointer user_data)
{
  GError *error = NULL;

  /*
   * GET FILE AS BYTES
   */
  GFileInputStream *in = g_file_read ((GFile *)user_data, NULL, &error);
  GOutputStream *out = g_memory_output_stream_new (NULL, 0, g_realloc, g_free);

  gchar buffer[100];
  gsize read = g_input_stream_read (G_INPUT_STREAM (in), buffer, 100, NULL, &error);
  while (read && !error)
    {
      g_output_stream_write (out, buffer, read, NULL, &error);
      if (!error)
	read = g_input_stream_read (G_INPUT_STREAM (in), buffer, 100, NULL, &error);
    }

  g_object_unref (in);

  if (!error)
    g_output_stream_close (out, NULL, &error);

  if (error)
    {
      g_object_unref (out);
      goto error;
    }


  GBytes *bytes = g_memory_output_stream_steal_as_bytes (G_MEMORY_OUTPUT_STREAM(out));

  g_object_unref (out);


  /*
   *  PARSE XML DATA
   */

  GMarkupParser *parser = g_malloc0 (sizeof (GMarkupParser));
  parser->start_element = parse_start_element;
  parser->text = parse_text;
  GMarkupParseContext *context = g_markup_parse_context_new (parser, 0, NULL, g_free);

  gsize text_len = 0;
  gchar *text = g_bytes_unref_to_data (bytes, &text_len);
  g_markup_parse_context_parse (context, text, text_len, &error);

  g_free (text);
  if (error)
    goto error;

error:
  if (error)
    {
      fprintf (stderr, "%s\n", error->message);
      exit (EXIT_FAILURE);
    }
}

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  GPtrArray *files = g_ptr_array_new_with_free_func (g_object_unref);
  GFile *root_dir = g_file_new_for_path ("./constant_pool_data");
  collect_children (files, root_dir);
  g_object_unref (root_dir);

  g_test_init (&argc, &argv, NULL);

  for (gint i = 0; i < files->len; i ++)
    {
      GFile *file = g_ptr_array_index (files, i);

      GFileInfo *info = g_file_query_info (file, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME, G_FILE_QUERY_INFO_NONE, NULL, NULL);
      if (!info)
	exit (EXIT_FAILURE);

      const gchar *path = g_file_info_get_display_name (info);

      gchar *test = g_strdup_printf ("/compilation_unit/class_file/constant_pool/%s",
				     path);

      g_object_unref (info);
      g_test_add_data_func_full (test,
				 g_object_ref (file),
				 test_constant_pool,
				 g_object_unref);

      g_free (test);
    }

  g_ptr_array_free (files, TRUE);

  return g_test_run();
}
