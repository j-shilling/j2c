#include <j2c/class-file.h>
#include <j2c/compilation-unit.h>

#include <gio/gio.h>

struct _J2cClassFile
{
  J2cIndexedFile parent_instance;

  gchar *name;
};

/* Private methods */
static void j2c_class_file_dispose (GObject *object);
static gchar *j2c_class_file_get_name (J2cIndexedFile *self);
static J2cFileType j2c_class_file_get_file_type (J2cIndexedFile *self);
static GDataInputStream *j2c_class_file_read (J2cIndexedFile *self, GError **error);
static void j2c_class_file_initable_interface_init (GInitableIface *iface);
static gboolean j2c_class_file_initable_init (GInitable *initable,
	      				  GCancellable *cancellable,
					  GError **error);

G_DEFINE_TYPE_WITH_CODE (J2cClassFile, j2c_class_file, J2C_TYPE_INDEXED_FILE,
			 G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
						j2c_class_file_initable_interface_init))

/****
 * CONSTRUCTION METHODS
 */

J2cClassFile *
j2c_class_file_new (J2cReadable *readable, GError **error)
{
  return g_initable_new (J2C_TYPE_CLASS_FILE,
			 NULL,
			 error,
		         J2C_INDEXED_FILE_PROPERTY_READABLE, readable,
			 NULL);
}

static void
j2c_class_file_class_init (J2cClassFileClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->dispose      = j2c_class_file_dispose;

  J2cIndexedFileClass *ifclass = J2C_INDEXED_FILE_CLASS (klass);

  ifclass->get_file_type = j2c_class_file_get_file_type;
  ifclass->get_name = j2c_class_file_get_name;
  ifclass->read = j2c_class_file_read;
}

static void
j2c_class_file_init (J2cClassFile *self)
{
  return;
}

static void
j2c_class_file_initable_interface_init (GInitableIface *iface)
{
  iface->init = j2c_class_file_initable_init;
}

/****
  DISTRUCTION METHODS
 ****/

static void
j2c_class_file_dispose (GObject *object)
{
  J2cClassFile *self = J2C_CLASS_FILE (object);
  if (self->name)
    g_free (self->name);
  self->name = NULL;
  G_OBJECT_CLASS (j2c_class_file_parent_class)->dispose (object);
}

/****
  INITABLE METHODS
 ****/

static gboolean
j2c_class_file_initable_init (GInitable *initable, GCancellable *cancellable, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  GError *tmp_error = NULL;

  J2cClassFile *self = J2C_CLASS_FILE (initable);
  GDataInputStream *stream = j2c_indexed_file_read (J2C_INDEXED_FILE (self), error);
  if (*error) return FALSE;

  guint32 magic = g_data_input_stream_read_uint32 (stream, NULL, &tmp_error);
  if (tmp_error)
    {
      g_propagate_error (error, tmp_error);
      g_object_unref (stream);
      return FALSE;
    }
  g_object_unref (stream);

  if (magic != J2C_CLASS_FILE_MAGIC)
    {
      g_set_error (error,
		   J2C_INDEXED_FILE_ERROR,
		   J2C_INDEXED_FILE_TYPE_ERROR,
		   "This is not a class file.");
      g_object_unref (stream);
      return FALSE;
    }

  J2cCompilationUnit *unit = j2c_compilation_unit_new (J2C_INDEXED_FILE (self), &tmp_error);
  if (tmp_error)
    {
      g_propagate_error (error, tmp_error);
      return FALSE;
    }
  J2C_CLASS_FILE (self)->name = g_strdup (j2c_compilation_unit_name (unit));
  g_object_unref (unit);

  return TRUE;
}

/****
 * INDEXED FILE METHODS
 */

static gchar *
j2c_class_file_get_name (J2cIndexedFile *self)
{
  g_return_val_if_fail (NULL != self, NULL);
  return J2C_CLASS_FILE (self)->name;
}

static J2cFileType
j2c_class_file_get_file_type (J2cIndexedFile *self)
{
  return J2C_FILE_TYPE_CLASS;
}

static GDataInputStream *
j2c_class_file_read (J2cIndexedFile *self, GError **error)
{
    g_return_val_if_fail (NULL != self, NULL);
    J2cReadable *readable = j2c_indexed_file_get_readable (self);
    GInputStream *in = j2c_readable_read (readable, error);
    g_return_val_if_fail (NULL != in, NULL);

    GDataInputStream *datain = g_data_input_stream_new (in);
    g_object_unref (in);
    g_object_unref (readable);

    g_return_val_if_fail (NULL != datain, NULL);

    g_data_input_stream_set_byte_order (datain, G_DATA_STREAM_BYTE_ORDER_BIG_ENDIAN);
    return datain;
}
