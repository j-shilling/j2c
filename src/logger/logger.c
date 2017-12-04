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

static void
j2c_logger_file_write_callback (GObject *source_object,
				GAsyncResult *res,
				gpointer user_data)
{
  GOutputStream *ostream = G_OUTPUT_STREAM (source_object);

  GError *error = NULL;
  if (!g_output_stream_close (ostream, NULL, &error))
    {
      j2c_logger_set_file (NULL);
      j2c_logger_warning ("Could not close log file: %s", error->message);
      g_error_free (error);
    }
  g_object_unref (ostream);
  g_free (user_data);
}

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

  GFileInfo *info = g_file_query_info (logger.file,
				       G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME,
				       G_FILE_QUERY_INFO_NONE,
				       NULL,
				       NULL);

  const gchar *name = info ?
    g_file_info_get_attribute_string (info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME) :
    NULL;
  j2c_logger_fine ("Logging to file \'%s\'", name ? name : "[unkown_name]");

  if (info)
    g_object_unref (info);
}

void
j2c_logger_log (J2cLoggerLevel const level, gchar const *const __fmt, ...)
{
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

  g_fprintf (out, "%s", text);

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
	  gchar *buffer = g_strdup (text);
	  gsize count = strlen (buffer);
	  g_output_stream_write_async (G_OUTPUT_STREAM(ostream),
				       buffer,
				       count,
				       G_PRIORITY_DEFAULT,
				       NULL,
				       j2c_logger_file_write_callback,
				       buffer);
	}

    }

  g_free (text);
}

void
j2c_logger_heading (gchar const *const heading)
{
  static const guint line_length = 64;

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
}
