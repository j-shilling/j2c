#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <glib.h>
#include <glib/gprintf.h>

#include <j2c/logger.h>
#include <j2c/options.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_TEXT_BOLD    "\x1b[1m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static const gint line_length = 72;

struct _Logger
{
  J2cLoggerLevel level;
  GFile *file;
  GMutex *mutex;
  GThreadPool *pool;
};

static struct _Logger logger = { J2C_LOGGER_LEVEL_WARNING, NULL, NULL, NULL };

struct _Level
{
  gchar *preamble;
  gboolean use_stderr;
};

typedef struct
{
  FILE *out;
  gchar *preamble;
  gchar *text;
} J2cWriteArgs;


static struct _Level levels_table[J2C_LOGGER_LEVEL_N_LEVELS] =
{
    { ANSI_TEXT_BOLD ANSI_COLOR_BLUE "[" ANSI_COLOR_GREEN "FINEST" ANSI_COLOR_BLUE "]:  " ANSI_COLOR_RESET, FALSE },
    { ANSI_TEXT_BOLD ANSI_COLOR_BLUE "[" ANSI_COLOR_GREEN "FINER" ANSI_COLOR_BLUE "]:   " ANSI_COLOR_RESET, FALSE },
    { ANSI_TEXT_BOLD ANSI_COLOR_BLUE "[" ANSI_COLOR_GREEN "FINE" ANSI_COLOR_BLUE "]:    " ANSI_COLOR_RESET, FALSE },
    { ANSI_TEXT_BOLD ANSI_COLOR_BLUE "[" ANSI_COLOR_GREEN "INFO" ANSI_COLOR_BLUE "]:    " ANSI_COLOR_RESET, FALSE },
    { ANSI_TEXT_BOLD ANSI_COLOR_RED "[" ANSI_COLOR_YELLOW "WARNING" ANSI_COLOR_RED "]: " ANSI_COLOR_RESET, TRUE  },
    { ANSI_TEXT_BOLD ANSI_COLOR_RED "[SEVERE]:  " ANSI_COLOR_RESET, TRUE  },
    { ANSI_TEXT_BOLD ANSI_COLOR_RED "[FATAL]:   " ANSI_COLOR_RESET, TRUE  }
};

static void
j2c_logger_write (gpointer data, gpointer user_data)
{
  J2cWriteArgs *args = data;
  g_mutex_lock (logger.mutex);

  if (args->preamble);
    g_fprintf (args->out, "%s", args->preamble);
  g_fprintf (args->out, "%s", args->text);

  if (logger.file)
    {
      GError *error = NULL;
      GFileOutputStream *ostream = g_file_append_to (logger.file,
						     G_FILE_CREATE_NONE,
						     NULL,
						     &error);
      if (!ostream)
        {
	  g_clear_object (&logger.file);
          j2c_logger_warning ("Could not log to file: %s", error->message);
          g_error_free (error);
        }
      else
        {
          gsize count = strlen (args->text);
          g_output_stream_write (G_OUTPUT_STREAM(ostream),
				 args->text,
                           count,
                           NULL,
                           &error);
	  if (error)
	    j2c_logger_warning ("%s", error->message);
	  g_object_unref (ostream);
        }

    }

  g_mutex_unlock (logger.mutex);
  g_free (args->text);
  g_free (args);
}

static void
j2c_logger_exit (void)
{
  if (logger.pool)
    {
      g_thread_pool_free (logger.pool, FALSE, TRUE);
      logger.pool = NULL;
    }
  if (logger.mutex)
    {
      g_mutex_clear (logger.mutex);
      g_free (logger.mutex);
      logger.mutex = NULL;
    }
  if (logger.file)
    g_clear_object (&logger.file);
}

static void
j2c_logger_init (void)
{
  if (logger.mutex == NULL)
    {
      logger.mutex = g_malloc (sizeof (GMutex));
      g_mutex_init (logger.mutex);
    }

  if (logger.pool == NULL)
    {
      logger.pool = g_thread_pool_new (j2c_logger_write,
				       NULL,
				       j2c_options_max_threads(),
				       FALSE,
				       NULL);
   }

  if (logger.file == NULL)
    {
      logger.file = j2c_options_log_file ();
      logger.level = j2c_options_logger_level();
      atexit (j2c_logger_exit);

      j2c_logger_finest ("Logging initialized");
    }
}

void
j2c_logger_log (J2cLoggerLevel const level,
		gchar const *const file,
		gint const line,
		gchar const *const func,
		gchar const *const __fmt,
		...)
{
  j2c_logger_init();
  if (level < logger.level)
    return;

  FILE *out = levels_table[level].use_stderr ? stderr : stdout;

  va_list args;
  va_start (args, __fmt);

  gchar *msg = NULL;
  g_vasprintf (&msg, __fmt, args);

  gboolean needs_new_line = ('\n' !=
			     msg[strlen(msg) - 1]);
  gchar *text = g_strdup_printf (needs_new_line ? "%s:%s:%d %s\n" : "%s:%s:%d %s",
				 file, func, line,
				 msg);

  g_free (msg);

  J2cWriteArgs *write_args = g_malloc (sizeof (J2cWriteArgs));
  write_args->preamble = levels_table[level].preamble;
  write_args->text = text;
  write_args->out = out;

  g_thread_pool_push (logger.pool, write_args, NULL);
}

void
j2c_logger_heading (gchar const *const heading)
{
  j2c_logger_init();

  gchar *text = g_malloc0 ( 2 * (3 + line_length) /* upper and lower lines */
			+        strlen (heading) + 100);
  gint index = 0;
  text[index++] = '\n';
  text[index++] = '\n';
  for (int i = 0; i < line_length; i ++)
    text[index++] = '*';
  text[index++] = '\n';

  text[index++] = '*';
  text[index++] = ' ';

  gint max = line_length - 4;
  gchar *cur = (gchar *)heading;
  guint i = 0;

  gint chs = 0;
  while (*cur != '\0' && chs < max)
    {

      gunichar ch = g_unichar_toupper (g_utf8_get_char(cur));
      index += g_unichar_to_utf8 (ch, text + index);
      cur = g_utf8_next_char (cur);

      chs ++;
    }

  for (int i = 0; i < (max - chs); i ++)
    text[index++] = ' ';

  text[index++] = ' ';
  text[index++] = '*';
  text[index++] = '\n';
  for (int i = 0; i < line_length; i ++)
    text[index++] = '*';
  text[index++] = '\n';
  text[index++] = '\n';
 
  J2cWriteArgs *args = g_malloc (sizeof (J2cWriteArgs));
  args->text = text;
  args->preamble = NULL;
  args->out = stdout;

  g_thread_pool_push (logger.pool, args, NULL);
}
