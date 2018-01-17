#include <j2c/zip-input-stream.h>

#include <zip.h>

#include <glib/gprintf.h>

/****
 * OBJECT STRUCTURES
 */

struct _J2cZipInputStream
{
  GInputStream parent_instance;

  J2cJarFile *jar;
  zip_file_t *file;
};

G_DEFINE_TYPE(J2cZipInputStream, j2c_zip_input_stream, G_TYPE_INPUT_STREAM)

/****
 * PRIV METHOD PROTOTYPES
 */

/* GObjectClass */
static void
j2c_zip_input_stream_dispose (GObject *object);

/* GInputStream */
static gssize
j2c_zip_input_stream_read_fn (GInputStream *stream, void *buffer, gsize count,
			      GCancellable *cancellable, GError **error);
static gssize
j2c_zip_input_stream_skip (GInputStream *stream, gsize count,
			   GCancellable *cancellable, GError **error);
static gboolean
j2c_zip_input_stream_close_fn (GInputStream *stream, GCancellable *cancellable,
			       GError **error);

/****
 * CONSTRUCTION METHODS
 */

J2cZipInputStream *
j2c_zip_input_stream_open (J2cJarFile *jar, guint64 index, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (jar != NULL, NULL);

  zip_t *zip = j2c_jar_file_open (jar, error);
  zip_file_t *zfile = zip_fopen_index (zip, index, ZIP_FL_UNCHANGED);

  J2cZipInputStream *ret = g_object_new (J2C_TYPE_ZIP_INPUT_STREAM, NULL);
  ret->jar = g_object_ref (jar);
  ret->file = zfile;

  return ret;
}

static void
j2c_zip_input_stream_class_init (J2cZipInputStreamClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->dispose = j2c_zip_input_stream_dispose;

  GInputStreamClass *input_stream_class = G_INPUT_STREAM_CLASS(klass);
  input_stream_class->read_fn = j2c_zip_input_stream_read_fn;
  input_stream_class->skip = j2c_zip_input_stream_skip;
  input_stream_class->close_fn = j2c_zip_input_stream_close_fn;
}

static void
j2c_zip_input_stream_init (J2cZipInputStream *self)
{
}

/****
 * DESTRUCTION METHODS
 */

static void
j2c_zip_input_stream_dispose (GObject *object)
{
  J2cZipInputStream *self = J2C_ZIP_INPUT_STREAM (object);

  if (self->file)
    {
      //zip_fclose (self->file);
      self->file = NULL;
    }

  if (self->jar)
    {
      j2c_jar_file_close (self->jar, NULL);
      g_clear_object (&self->jar);
    }
}

/****
 * PRIV METHOD DEFINITIONS
 */

static gssize
j2c_zip_input_stream_read_fn (GInputStream *stream, void *buffer, gsize count,
			      GCancellable *cancellable, GError **error)
{
  g_return_val_if_fail (stream != NULL, 0);
  g_return_val_if_fail (buffer != NULL, 0);
  g_return_val_if_fail (error == NULL || *error == NULL, 0);

  J2cZipInputStream *self = J2C_ZIP_INPUT_STREAM (stream);

  zip_int64_t ret = zip_fread (self->file, buffer, count);
  if (ret != count)
    {
      if (self->file)
	{
	  zip_error_t *zer = zip_file_get_error (self->file);
	  j2c_zip_input_stream_set_error (zer, NULL, error);
	}
      else
	{
	  g_set_error (error,
		       J2C_ZIP_ERROR,
		       J2C_ZIP_CLOSE_ERROR,
		       "This stream is closed.");
	}
    }

  return (gssize) ret;
}
static gssize
j2c_zip_input_stream_skip (GInputStream *stream, gsize count,
			   GCancellable *cancellable, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, 0);

  gssize ret = 0;
  gchar tmp;
  GError *tmp_error = NULL;
  for (gint i = 0; i < count; i++)
    {
      j2c_zip_input_stream_read_fn (stream, &tmp, 1, cancellable, &tmp_error);
      if (tmp_error)
	{
	  g_propagate_error (error, tmp_error);
	  break;
	}
      else
	{
	  ret ++;
	}
    }

  return ret;
}

static gboolean
j2c_zip_input_stream_close_fn (GInputStream *stream, GCancellable *cancellable,
			       GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  g_return_val_if_fail (stream != NULL, FALSE);
  J2cZipInputStream *self = J2C_ZIP_INPUT_STREAM (stream);

  if (self->file)
    {
      self->file = NULL;
    }

  if (self->jar)
    {
      j2c_jar_file_close (self->jar, error);
      g_clear_object (&self->jar);
    }

  return TRUE;
}

/****
 * PUBLIC METHODS
 */

gboolean
j2c_zip_input_stream_set_error (zip_error_t *zer, gchar *path, GError **error)
{
  g_return_val_if_fail(error == NULL || *error == NULL, FALSE);
  g_return_val_if_fail(NULL != zer, FALSE);

  if (zip_error_code_zip (zer) == J2C_ZIP_OK)
    return FALSE;

  gchar *preamble = path ? g_strdup_printf ("%s: ", path) : g_strdup ("");
  const gchar *msg = zip_error_strerror (zer);

  g_set_error (error,
	       J2C_ZIP_ERROR,
	       zip_error_code_zip (zer), "%s%s", preamble, msg);

  g_free (preamble);

  return TRUE;
}

gboolean
j2c_zip_input_stream_set_error_from_code (gint zer, gchar *path, GError **error)
{
  g_return_val_if_fail(error == NULL || *error == NULL, FALSE);
  g_return_val_if_fail(zer != ZIP_ER_OK, FALSE);

  zip_error_t *ze = g_malloc (sizeof(zip_error_t));
  zip_error_init_with_code (ze, zer);

  gboolean ret = j2c_zip_input_stream_set_error (ze, NULL, error);

  zip_error_fini (ze);

  return ret;
}
