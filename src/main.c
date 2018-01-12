#include <stdlib.h>
#include <locale.h>

#include <glib.h>

#include <j2c/logger.h>
#include <j2c/index.h>
#include <j2c/reader.h>
#include <j2c/dependency-info.h>
#include <j2c/method.h>

static gboolean j2c_parse_verbosity (const gchar *option_name,
				     const gchar *value,
				     gpointer data,
				     GError **error);

static void j2c_parse_input_files (gpointer data,
				   gpointer user_data);

static void j2c_index_files (gpointer data,
			     gpointer user_data);
static void
j2c_find_dependencies (gpointer data, gpointer user_data);

static guint verbosity = 0;
static gint max_threads = 5;

static gchar *log_filename = NULL;
static gchar *output_filename = NULL;

static gchar **class_path_filenames = NULL;
static gchar **target_filenames = NULL;

static GOptionEntry entries[] =
{
  { "verbose", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, j2c_parse_verbosity, "Activate more verbose logging.", NULL },
  { "jobs", 'j', 0, G_OPTION_ARG_INT, &max_threads, "Limit the number of parallel threads in a given thread-pool.", "[INT]" },
  { "log", 'l', 0, G_OPTION_ARG_FILENAME, &log_filename, "Set the log file to be used.", "[FILE]" },
  { "output", 'o', 0, G_OPTION_ARG_FILENAME, &output_filename, "Set the directory to write to.", "[DIR]" },

  { "class-path", 'p', 0, G_OPTION_ARG_FILENAME_ARRAY, &class_path_filenames, "Add file or directory to the index of available types.", "[FILE|DIR]" },
  { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &target_filenames, "Sources, classfiles, or jars to compile.", "[FILE|DIR]" },
  { NULL}
};

gint
main (gint argc, gchar *argv[])
{
  setlocale (LC_ALL, "");
  GError *error = NULL;

  /****
    PARSE COMMAND LINE ARGS
   ****/

  j2c_logger_heading ("Parsing User Options");

  GOptionContext *context = g_option_context_new ("[FILES|DIRS]");
  g_option_context_set_summary (context, "Genderate C+GObject project from java.");
  g_option_context_add_main_entries (context, entries, NULL);

#ifdef G_OS_WIN32
  gchar **args = g_win32_get_command_line();
  if (!g_option_context_parse_strv (context, &args, &error))
#else
  gchar **args = argv;
  if (!g_option_context_parse (context, &argc, &args, &error))
#endif
    {
      j2c_logger_fatal ("Error parsing arguments: %s", error->message);
      exit (EXIT_FAILURE);
    }
#ifdef G_OS_WIN32
  g_strfreev (args);
#endif
  g_option_context_free (context);

  j2c_logger_finest ("Command line arguments parsed.");

  /****
    SET UP LOGGING
   ****/

  if (verbosity == 1)
    j2c_logger_set_level (J2C_LOGGER_LEVEL_FINE);
  else if (verbosity == 2)
    j2c_logger_set_level (J2C_LOGGER_LEVEL_FINER);
  else if (verbosity > 2)
    j2c_logger_set_level (J2C_LOGGER_LEVEL_FINEST);

  if (log_filename)
    {
      j2c_logger_set_file (g_file_new_for_commandline_arg (log_filename));
      g_free (log_filename);
    }
  else
    {
      j2c_logger_set_file (g_file_new_for_path ("j2c.log"));
    }

  j2c_logger_finest ("Logging initialized.");

  if (max_threads < 1)
    max_threads = 1;

  j2c_logger_finer ("Parsed args:");
  if (verbosity == 1)
    j2c_logger_finer ("\t-v");
  else if (verbosity == 2)
    j2c_logger_finer ("\t-vv");
  else if (verbosity > 2)
    j2c_logger_finer ("\t-vvv");
  j2c_logger_finer ("\t-j %d", max_threads);
  j2c_logger_finer ("\t-l %s", log_filename);
  j2c_logger_finer ("\t-o %s", output_filename);

  if (class_path_filenames)
    {
      j2c_logger_finer ("In class path:");
      for (gchar **file = class_path_filenames;
	   file && *file; file ++)
	j2c_logger_finer ("\t%s", *file);
    }
  if (target_filenames)
    {
      j2c_logger_finer ("In target path:");
      for (gchar **file = target_filenames;
	   file && *file; file ++)
	j2c_logger_finer ("\t%s", *file);
    }

  /****
    PARSE INPUT FILES
   ****/

  j2c_logger_heading ("Scanning input files.");

  J2cReadableList *class_path_files = j2c_readable_list_new ();
  GThreadPool *pool = g_thread_pool_new (j2c_parse_input_files,
					 class_path_files,
					 max_threads,
					 TRUE,
					 &error);
  if (error)
    {
      j2c_logger_severe ("Error creating thread pool: %s", error->message);
      g_error_free (error);
      error = NULL;
    }

  j2c_logger_finest ("Parsing class path files.");
  for (gchar **filename = class_path_filenames;
       filename && *filename; filename ++)
    {
      if (!g_thread_pool_push (pool, *filename, &error))
	{
	  j2c_logger_severe ("%s", error->message);
	  g_error_free (error);
	  error = NULL;
	}
    }

  g_thread_pool_free (pool, FALSE, TRUE);
  g_strfreev (class_path_filenames);

  J2cReadableList *target_files = j2c_readable_list_new ();
  pool = g_thread_pool_new (j2c_parse_input_files,
			    target_files,
			    max_threads,
			    TRUE,
			    &error);
  if (error)
    {
      j2c_logger_severe ("Error creating thread pool: %s", error->message);
      g_error_free (error);
      error = NULL;
    }

  j2c_logger_finest ("Parsing target files.");
  for (gchar **filename = target_filenames;
       filename && *filename; filename ++)
    {
      if (!g_thread_pool_push (pool, *filename, &error))
	{
	  j2c_logger_severe ("%s", error->message);
	  g_error_free (error);
	  error = NULL;
	}
    }

  g_thread_pool_free (pool, FALSE, TRUE);
  g_strfreev (target_filenames);

  /****
    INDEXING FILES
   ****/

  j2c_logger_heading ("indexing files");
  J2cIndex *index = j2c_index_new ();
  gpointer user_data[2] = { index, NULL };
  pool = g_thread_pool_new (j2c_index_files,
			    user_data,
			    max_threads,
			    TRUE,
			    &error);
  if (error)
    {
      j2c_logger_severe ("Error creating thread pool: %s", error->message);
      g_error_free (error);
      error = NULL;
    }
  j2c_logger_finer ("Indexing class path.");
  for (GSList *file = class_path_files->list; file; file = file->next)
    {
      if (!g_thread_pool_push (pool, file->data, &error))
	{
	  j2c_logger_severe ("%s", error->message);
	  g_error_free (error);
	  error = NULL;
	}
    }
  g_thread_pool_free (pool, FALSE, TRUE);
  j2c_readable_list_destroy (class_path_files);

  user_data[1] = index;
  pool = g_thread_pool_new (j2c_index_files,
			    user_data,
			    max_threads,
			    TRUE,
			    &error);
  if (error)
    {
      j2c_logger_severe ("Error creating thread pool: %s", error->message);
      g_error_free (error);
      error = NULL;
    }
  j2c_logger_finer ("Indexing target.");
  for (GSList *file = target_files->list; file; file = file->next)
    {
      if (!g_thread_pool_push (pool, file->data, &error))
	{
	  j2c_logger_severe ("%s", error->message);
	  g_error_free (error);
	  error = NULL;
	}
    }
  g_thread_pool_free (pool, FALSE, TRUE);
  j2c_readable_list_destroy (target_files);

  J2cIndexedFile *main = j2c_index_get_main (index);
  J2cMethod *method = j2c_indexed_file_get_main (main);
  J2cDependencyInfo *deps = g_object_new (J2C_TYPE_DEPENDENCY_INFO, NULL);
  j2c_find_dependencies (method, deps);

  g_object_unref (method);
  g_object_unref (main);
  g_object_unref (deps);


  g_object_unref (index);
}

static gboolean
j2c_parse_verbosity (const gchar *option_name,
		     const gchar *value,
		     gpointer data,
		     GError **error)
{
  verbosity ++;
  return TRUE;
}

static void
j2c_parse_input_files (gpointer data,
	    	       gpointer user_data)
{
  j2c_readable_list_add ((J2cReadableList *)user_data, (gchar const *const) data);
}

static void
j2c_index_files (gpointer data, gpointer user_data)
{
  g_return_if_fail (NULL != user_data);
  g_return_if_fail (J2C_IS_READABLE (data));

  gpointer *args = (gpointer *) user_data;

  j2c_index_insert_file ((J2cIndex *)args[0], J2C_READABLE (data), args[1] == NULL);
}

static void
j2c_find_dependencies (gpointer data, gpointer user_data)
{
  J2cDependencyInfo *info = user_data;
  J2cMethod *method = data;

  gchar *method_name = j2c_method_get_java_name (method);
  j2c_logger_fine ("Scanning %s", method_name);
  g_free (method_name);

  J2cDependencyInfo *result = j2c_method_get_dependency_info (method);

  g_object_unref (result);
}
