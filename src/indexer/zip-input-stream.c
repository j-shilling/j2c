#include <j2c/zip-input-stream.h>

#include <zip.h>

#include <glib/gprintf.h>

/****
 * OBJECT STRUCTURES
 */

struct _J2cZipInputStream
{
  GInputStream parent_instance;
  zip_t *zip;
  zip_file_t *file;
};

G_DEFINE_TYPE(J2cZipInputStream, j2c_zip_input_stream, G_TYPE_INPUT_STREAM)

/****
 * PRIV METHOD PROTOTYPES
 */

/* GObjectClass */
static void
j2c_zip_input_stream_set_property (GObject *object, guint property_id,
				   const GValue *value, GParamSpec *pspec);
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
j2c_zip_input_stream_open (GFile *file, guint64 index, GError **error)
{
  gchar *path = g_file_get_path (file);
  gint ze = ZIP_ER_OK;

  zip_t *zip = zip_open (path, ZIP_RDONLY, &ze);
  if (!zip)
    {
      j2c_zip_input_stream_set_error_from_code (ze, g_file_get_path (file), error);
      return NULL;
    }

  zip_file_t *zfile = zip_fopen_index (zip, index, ZIP_FL_UNCHANGED);

  return g_object_new (J2C_TYPE_ZIP_INPUT_STREAM, "zip", zip, "file", zfile, NULL);
}

static void
j2c_zip_input_stream_class_init (J2cZipInputStreamClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->set_property = j2c_zip_input_stream_set_property;
  object_class->dispose = j2c_zip_input_stream_dispose;

  g_object_class_install_property (
      object_class, 1,
      g_param_spec_pointer ("zip", "zip", "zip_t for the zip file", G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (
      object_class, 2,
      g_param_spec_pointer ("file", "file", "zip_file_t for the file in a zip", G_PARAM_CONSTRUCT_ONLY));

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
    zip_fclose (self->file);
  if (self->zip)
    zip_close (self->zip);

  self->file = NULL;
  self->zip = NULL;
}

/****
 * PRIV METHOD DEFINITIONS
 */



static void
j2c_zip_input_stream_set_property (GObject *object, guint property_id,
				   const GValue *value, GParamSpec *pspec)
{
  if (property_id == 1)
    {
      J2cZipInputStream *self = J2C_ZIP_INPUT_STREAM (object);
      zip_t *zip = g_value_get_pointer (value);

      if (self->zip == NULL)
	self->zip = zip;
      else if (self->zip != zip)
	g_warning ("J2cZipInputStream::zip should only be set once.");
    }
  else
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

static gssize
j2c_zip_input_stream_read_fn (GInputStream *stream, void *buffer, gsize count,
			      GCancellable *cancellable, GError **error)
{
  J2cZipInputStream *self = J2C_ZIP_INPUT_STREAM (stream);

  zip_int64_t ret = zip_fread (self->file, buffer, count);
  if (ret != count)
    {
      zip_error_t *zer = zip_file_get_error (self->file);
      j2c_zip_input_stream_set_error (zer, NULL, error);
    }

  return (gssize) ret;
}
static gssize
j2c_zip_input_stream_skip (GInputStream *stream, gsize count,
			   GCancellable *cancellable, GError **error)
{
    gchar *buf = g_malloc (count);
    gssize ret = j2c_zip_input_stream_read_fn (stream, buf, count, cancellable, error);
    g_free (buf);

    return ret;
}

static gboolean
j2c_zip_input_stream_close_fn (GInputStream *stream, GCancellable *cancellable,
			       GError **error)
{
  J2cZipInputStream *self = J2C_ZIP_INPUT_STREAM (stream);

  if (self->file)
    {
      gint zer = zip_fclose (self->file);
      if (zer != ZIP_ER_OK)
	{
	  j2c_zip_input_stream_set_error_from_code (zer, NULL, error);
	  return FALSE;
	}

      self->file = NULL;
    }

  if (self->zip)
    {
      if (0 != zip_close (self->zip))
	{
	  zip_error_t *zer = zip_get_error (self->zip);
	  j2c_zip_input_stream_set_error (zer, NULL, error);
	  return FALSE;
	}

      self->zip = NULL;
    }

  return TRUE;
}

/****
 * PUBLIC METHODS
 */

gboolean
j2c_zip_input_stream_set_error (zip_error_t *zer, gchar *path, GError **error)
{
  g_return_val_if_fail(NULL != error, FALSE);
  g_return_val_if_fail(NULL != zer, FALSE);

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
  g_return_val_if_fail(NULL != error, FALSE);
  g_return_val_if_fail(zer != ZIP_ER_OK, FALSE);

  zip_error_t *ze = g_malloc (sizeof(zip_error_t));
  zip_error_init_with_code (ze, zer);

  gboolean ret = j2c_zip_input_stream_set_error (ze, NULL, error);

  zip_error_fini (ze);

  return ret;
}
