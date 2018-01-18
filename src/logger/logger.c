#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <glib.h>
#include <glib/gprintf.h>

#include <j2c/logger.h>

struct _Logger
{
  J2cLoggerLevel level;
  GFile *file;
  GMutex *mutex;
};

static struct _Logger logger = { J2C_LOGGER_LEVEL_WARNING, NULL };

struct _Level
{
  gchar *preamble;
  gboolean use_stderr;
};

static struct _Level levels_table[J2C_LOGGER_LEVEL_N_LEVELS] =
{
    { "[FINEST]:  ", FALSE },
    { "[FINER]:   ", FALSE },
    { "[FINE]:    ", FALSE },
    { "[INFO]:    ", FALSE },
    { "[WARNING]: ", TRUE  },
    { "[SEVERE]:  ", TRUE  },
    { "[FATAL]:   ", TRUE  }
};

void
j2c_logger_set_level (J2cLoggerLevel const level)
{
  logger.level = level;
}

void
j2c_logger_set_file (GFile *file)
{
  if (logger.file)
    g_object_unref (logger.file);

  logger.file = file;

  gchar *path = g_file_get_path (logger.file);
  j2c_logger_fine ("Logging to file \'%s\'", path);
  g_free (path);
}

typedef struct
{
  FILE *out;
  gchar *text;
} J2cWriteArgs;

static gpointer
j2c_logger_write (gpointer data)
{
  J2cWriteArgs *args = data;
  g_mutex_lock (logger.mutex);

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
          j2c_logger_set_file (NULL);
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

  return NULL;
}

void
j2c_logger_log (J2cLoggerLevel const level, gchar const *const __fmt, ...)
{
  if (logger.mutex == NULL)
    {
      logger.mutex = g_malloc (sizeof (GMutex));
      g_mutex_init (logger.mutex);
    }

  if (level < logger.level)
    return;

  FILE *out = levels_table[level].use_stderr ? stderr : stdout;

  va_list args;
  va_start (args, __fmt);

  gchar *msg = NULL;
  g_vasprintf (&msg, __fmt, args);

  gboolean needs_new_line = ('\n' !=
			     msg[strlen(msg) - 1]);
  gchar *text = g_strdup_printf (needs_new_line ? "%s%s\n" : "%s%s",
				 levels_table[level].preamble,
				 msg);

  g_free (msg);

  J2cWriteArgs *write_args = g_malloc (sizeof (J2cWriteArgs));
  write_args->text = text;
  write_args->out = out;

  GThread *thread = g_thread_new ("j2c-logger-write", j2c_logger_write, write_args);
}

static gpointer
j2c_logger_write_heading (gpointer data)
{
  static const guint line_length = 64;
  gchar const *const heading = data;

  g_mutex_lock (logger.mutex);
  g_printf ("\n");
  g_printf ("\n");
  for (int i = 0; i < line_length; i ++)
    g_printf ("*");
  g_printf ("\n*  ");

  static const guint max = 58;
  gchar *text = (gchar *)heading;
  gunichar ch = g_utf8_get_char (text);
  guint i = 0;

  while (i < max)
    {
      if (ch != '\0')
        g_printf ("%c", g_unichar_toupper (ch));
      else
        g_printf (" ");
      i++;

      if (ch != '\0')
        {
          text = g_utf8_next_char (text);
          ch = g_utf8_get_char (text);
        }
    }

  g_printf ("  *\n");
  for (int i = 0; i < line_length; i ++)
    g_printf ("*");
  g_printf ("\n");
  g_printf ("\n");
  g_mutex_unlock (logger.mutex);

  return NULL;
}
void
j2c_logger_heading (gchar const *const heading)
{
  if (logger.mutex == NULL)
    {
      logger.mutex = g_malloc (sizeof (GMutex));
      g_mutex_init (logger.mutex);
    }

  GThread *thread = g_thread_new ("j2c-logger-write", j2c_logger_write_heading, (gpointer) heading);
}
