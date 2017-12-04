#include <stdlib.h>
#include <locale.h>

#include <glib.h>

#include <j2c/logger.h>

static gboolean j2c_parse_verbosity (const gchar *option_name,
				     const gchar *value,
				     gpointer data,
				     GError **error);

static void j2c_parse_input_files (gpointer data,
				   gpointer user_data);

static guint verbosity = 0;
static gint max_threads = 5;

static gchar *default_log_filename = "j2c.log";
static gchar *log_filename = NULL;

static gchar *default_output_filename = "target";
static gchar *output_filename = NULL;

static gchar **class_path_filenames = NULL;
static gchar **target_filenames = NULL;

static GSList *class_path_files = NULL;
static GMutex class_path_m;

static GSList *target_files = NULL;
static GMutex target_m;

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
#else
  gchar **args = g_strdupv (argv);
#endif

  if (!g_option_context_parse (context, &argc, &args, &error))
    {
      j2c_logger_fatal ("Error parsing arguments: %s", error->message);
      exit (EXIT_FAILURE);
    }
  g_strfreev (args);
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
      if (log_filename != default_log_filename)
	g_free (log_filename);
    }

  j2c_logger_finest ("Logging initialized.");

  if (max_threads < 1)
    max_threads = 1;
  if (!log_filename)
    log_filename = default_log_filename;
  if (!output_filename)
    output_filename = default_output_filename;

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

  g_mutex_init (&class_path_m);
  g_mutex_init (&target_m);

  gpointer user_data[2] = { &class_path_files, &class_path_m };
  GThreadPool *pool = g_thread_pool_new (j2c_parse_input_files,
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

  user_data[0] = &target_files;
  user_data[1] = &target_m;
  pool = g_thread_pool_new (j2c_parse_input_files,
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

  j2c_logger_finer ("Parsed files in class path:");
  for (GSList *file = class_path_files; file; file = file->next)
    {
      GFile *f = file->data;
      GFileInfo *info = g_file_query_info (f, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME, G_FILE_QUERY_INFO_NONE, NULL, &error);
      if (error)
	{
	  j2c_logger_warning ("Error getting filename: %s", error->message);
	  g_error_free (error);
	  error = NULL;
	  continue;
	}

      j2c_logger_finer ("\t%s",
			g_file_info_get_attribute_string (info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME));
      if (info)
	g_object_unref (info);
    }

  j2c_logger_finer ("Parsed files in target path:");
  for (GSList *file = target_files; file; file = file->next)
    {
      GFile *f = (GFile *)file->data;
      GFileInfo *info = g_file_query_info (f, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME, G_FILE_QUERY_INFO_NONE, NULL, &error);
      if (!info)
	{
	  j2c_logger_warning ("Error getting filename: %s", error ? error->message : "unkown error");
	  if (error)
	    g_error_free (error);
	  error = NULL;
	  continue;
	}

      j2c_logger_finer ("\t%s",
			g_file_info_get_attribute_string (info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME));
      if (info)
	g_object_unref (info);
    }
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
  GError *error = NULL;

  gchar const *filename = (gchar const *)data;
  GSList **files = ((GSList ***)user_data)[0];
  GMutex *m = ((GMutex **)user_data)[1];

  if (!filename || !files)
    return;

  j2c_logger_finest ("Parsing %s", filename);

  GFile *file = g_file_new_for_commandline_arg (filename);
  GFileInfo *info = g_file_query_info (file,
				       G_FILE_ATTRIBUTE_STANDARD_TYPE ","
				       G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME ","
				       G_FILE_ATTRIBUTE_ACCESS_CAN_READ,
				       G_FILE_QUERY_INFO_NONE,
				       NULL,
				       &error);
  if (!info)
    {
      j2c_logger_warning ("Could not get info for \'%s\': %s",
			  filename,
			  error->message);
      g_error_free (error);
      g_object_unref (file);
      return;
    }

  if (!g_file_info_get_attribute_boolean (info, G_FILE_ATTRIBUTE_ACCESS_CAN_READ))
    {
      j2c_logger_warning ("Cannot read file \'%s\'",
			  g_file_info_get_attribute_string (info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME));
      g_object_unref (info);
      g_object_unref (file);
      return;
    }

  if (G_FILE_TYPE_DIRECTORY !=
      g_file_info_get_attribute_uint32 (info, G_FILE_ATTRIBUTE_STANDARD_TYPE))
    {
      g_mutex_lock (m);
      *files = g_slist_prepend (*files, file);
      g_mutex_unlock (m);
    }
  else
    {
      GFileEnumerator *direnum = g_file_enumerate_children (file,
							    G_FILE_ATTRIBUTE_STANDARD_TYPE ","
							    G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME ","
							    G_FILE_ATTRIBUTE_ACCESS_CAN_READ,
							    G_FILE_QUERY_INFO_NONE,
							    NULL,
							    &error);
      if (error)
	{
	  j2c_logger_warning ("Cannot read directory \'%s\': %s",
			      g_file_info_get_attribute_string (info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME),
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
				  g_file_info_get_attribute_string (info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME),
				  error->message);
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
				      g_file_info_get_attribute_string (child_info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME));
		  continue;
		}

	      if (G_FILE_TYPE_DIRECTORY ==
		  g_file_info_get_attribute_uint32 (child_info, G_FILE_ATTRIBUTE_STANDARD_TYPE))
		{
		  j2c_logger_fine ("Ignoring subdirectory \'%s\'.",
				   g_file_info_get_attribute_string (child_info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME));
		  continue;
		}
	     
	      g_mutex_lock (m);
	      G_IS_FILE (child);
	      *files = g_slist_prepend (*files, g_file_dup (child));
	      g_mutex_unlock (m);
	    }
	}
      while (child && child_info);

      g_object_unref (file);
      g_object_unref (direnum);
    }

  g_object_unref (info);
}


