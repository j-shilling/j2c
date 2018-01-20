#include <j2c/options.h>
#include <j2c/logger.h>
#include <config.h>

#include <stdlib.h>
#include <locale.h>

#define LOG_FILENAME_DEFAULT	  "./" PACKAGE ".log"
#define OUTPUT_FILENAME_DEFAULT	  "./target"

static gboolean j2c_parse_verbosity (const gchar *option_name,
				     const gchar *value,
				     gpointer data,
				     GError **error);

static struct
{
  J2cLoggerLevel log_level;
  guint max_threads;
  gchar *log_filename;
  gchar *output_filename;
  gchar **class_path_filenames;
  gchar **target_filenames;
} user_options = {
  .log_level = J2C_LOGGER_LEVEL_INFO,
  .max_threads = 5,
  .log_filename = NULL,
  .output_filename = NULL,
  .class_path_filenames = NULL,
  .target_filenames = NULL
};

static GOptionEntry entries[] =
{
  { "verbose", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, j2c_parse_verbosity, "Activate more verbose logging.", NULL },
  { "jobs", 'j', 0, G_OPTION_ARG_INT, &user_options.max_threads, "Limit the number of parallel threads in a given thread-pool.", "[INT]" },
  { "log", 'l', 0, G_OPTION_ARG_FILENAME, &user_options.log_filename, "Set the log file to be used.", "[FILE]" },
  { "output", 'o', 0, G_OPTION_ARG_FILENAME, &user_options.output_filename, "Set the directory to write to.", "[DIR]" },

  { "include", 'I', 0, G_OPTION_ARG_FILENAME_ARRAY, &user_options.class_path_filenames, "Add file or directory to the index of available types.", "[FILE|DIR]" },
  { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &user_options.target_filenames, "Sources, classfiles, or jars to compile.", "[FILE|DIR]" },
  { NULL}
};

J2cLoggerLevel
j2c_options_logger_level (void)
{
  return user_options.log_level;
}

gint
j2c_options_max_threads (void)
{
  return user_options.max_threads;
}

GFile *
j2c_options_log_file (void)
{
  if (user_options.log_filename)
    return g_file_new_for_commandline_arg (user_options.log_filename);
  else
    return g_file_new_for_path (LOG_FILENAME_DEFAULT);
}
				
GFile *
j2c_options_output_file (void)
{
  if (user_options.output_filename)
    return g_file_new_for_commandline_arg (user_options.output_filename);
  else
    return g_file_new_for_path (OUTPUT_FILENAME_DEFAULT);
}

GFile **
j2c_options_class_path_files (void)
{
  gsize nmemb = 0;
  for (gchar **cur = user_options.class_path_filenames;
       cur && *cur;
       cur ++)
    nmemb ++;

  GFile **ret = g_malloc0 (sizeof (GFile *) * (nmemb + 1));
  for (gint i = 0; i < nmemb; i ++)
    ret[i] = g_file_new_for_commandline_arg (user_options.class_path_filenames[i]);

  return ret;
}

GFile **
j2c_options_target_files (void)
{
  gsize nmemb = 0;
  for (gchar **cur = user_options.target_filenames;
       cur && *cur;
       cur ++)
    nmemb ++;

  GFile **ret = g_malloc0 (sizeof (GFile *) * (nmemb + 1));
  for (gint i = 0; i < nmemb; i ++)
    ret[i] = g_file_new_for_commandline_arg (user_options.target_filenames[i]);

  return ret;
}

void
j2c_parse_options (gint argc, gchar *argv[])
{
  setlocale (LC_ALL, "");
  GOptionContext *context = g_option_context_new ("");
  g_option_context_set_summary (context, "Genderate C+GObject project from java.");
  g_option_context_add_main_entries (context, entries, NULL);

#ifdef G_OS_WIN32
  gchar **args = g_win32_get_command_line();
  if (!g_option_context_parse_strv (context, &args, &error))
#else
  gchar **args = argv;
  GError *error = NULL;
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
}

static gboolean
j2c_parse_verbosity (const gchar *option_name,
		     const gchar *value,
		     gpointer data,
		     GError **error)
{
  if (user_options.log_level == J2C_LOGGER_LEVEL_INFO)
    user_options.log_level = J2C_LOGGER_LEVEL_FINE;
  else if (user_options.log_level == J2C_LOGGER_LEVEL_FINE)
    user_options.log_level = J2C_LOGGER_LEVEL_FINER;
  else
    user_options.log_level = J2C_LOGGER_LEVEL_FINEST;
   
  return TRUE;
}
