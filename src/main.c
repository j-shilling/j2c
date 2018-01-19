#include <stdlib.h>
#include <locale.h>

#include <glib.h>

#include <j2c/options.h>
#include <j2c/logger.h>
#include <j2c/index.h>
#include <j2c/reader.h>
#include <j2c/dependency-info.h>
#include <j2c/method.h>

static void j2c_parse_input_files (gpointer data,
				   gpointer user_data);

static void j2c_index_files (gpointer data,
			     gpointer user_data);
static void
j2c_find_dependencies (gpointer data, gpointer user_data);

gint
main (gint argc, gchar *argv[])
{
  setlocale (LC_ALL, "");
  GError *error = NULL;

  /****
    PARSE COMMAND LINE ARGS
   ****/

  j2c_parse_options (argc, argv);

  /****
    INDEXING FILES
   ****/

  j2c_index_init ();

  /****
    BUILD DEPENDENCY TREE
   ****/
  j2c_logger_heading ("buidling dependency tree");
  j2c_dependency_info_init (j2c_options_max_threads());

  J2cIndexedFile *main = j2c_index_get_main ();
  if (!main)
    {
      j2c_logger_fatal ("Could not fine public static void main() in target.");
      exit (EXIT_FAILURE);
    }

  J2cMethod *method = j2c_indexed_file_get_main (main);
  j2c_dependency_info_add_method_call (method, NULL);
  j2c_dependency_info_lock ();

  g_object_unref (method);
  g_object_unref (main);
}
