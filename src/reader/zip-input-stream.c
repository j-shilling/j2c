#include <j2c/zip-input-stream.h>
#include <j2c/zip.h>

#include <glib/gprintf.h>

/****
 * OBJECT STRUCTURES
 */

struct _J2cZipInputStream
{
  GInputStream parent_instance;

  J2cZip *zip;
  J2cZipFile *zfile;
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
j2c_zip_input_stream_open (J2cZip *zip, guint64 index, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (zip != NULL, NULL);

  GError *tmp_error = NULL;
  J2cZipFile *zfile = j2c_zip_file_open (zip, index, &tmp_error);
  if (tmp_error)
    {
      g_propagate_error (error, tmp_error);
      return NULL;
    }

  J2cZipInputStream *ret = g_object_new (J2C_TYPE_ZIP_INPUT_STREAM, NULL);
  ret->zip = g_object_ref (zip);
  ret->zfile = zfile;

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

  if (self->zip)
    g_clear_object (&self->zip);
  if (self->zfile)
    g_clear_object (&self->zfile);

  G_OBJECT_CLASS (j2c_zip_input_stream_parent_class)->dispose (object);
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

  return j2c_zip_file_read (self->zfile, buffer, count, error);
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
  if (self->zfile)
    return j2c_zip_file_close (self->zfile, error);
  else
    return TRUE;
}
