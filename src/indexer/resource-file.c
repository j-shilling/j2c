#include <j2c/resource-file.h>

struct _J2cResourceFile
{
  J2cIndexedFile parent_instance;
};

/* Private methods */
static gchar *j2c_resource_file_get_name (J2cIndexedFile *self);
static J2cFileType j2c_resource_file_get_file_type (J2cIndexedFile *self);
static GDataInputStream *j2c_resource_file_read (J2cIndexedFile *self, GError **error);

G_DEFINE_TYPE (J2cResourceFile, j2c_resource_file, J2C_TYPE_INDEXED_FILE)

/****
 * CONSTRUCTION METHODS
 */

J2cResourceFile *
j2c_resource_file_new (J2cReadable *readable)
{
  return g_object_new (J2C_TYPE_RESOURCE_FILE,
		       J2C_INDEXED_FILE_PROPERTY_READABLE, readable,
		       NULL);
}

static void
j2c_resource_file_class_init (J2cResourceFileClass *klass)
{
  J2cIndexedFileClass *ifclass = J2C_INDEXED_FILE_CLASS (klass);

  ifclass->get_file_type = j2c_resource_file_get_file_type;
  ifclass->get_name = j2c_resource_file_get_name;
  ifclass->read = j2c_resource_file_read;
}

static void
j2c_resource_file_init (J2cResourceFile *self)
{
  return;
}

/****
 * INDEXED FILE METHODS
 */

static gchar *
j2c_resource_file_get_name (J2cIndexedFile *self)
{
  g_return_val_if_fail (NULL != self, NULL);

  J2cReadable *readable = j2c_indexed_file_get_readable (self);
  gchar *ret = g_strdup (j2c_readable_name (readable));
  g_object_unref (readable);

  return ret;
}

static J2cFileType
j2c_resource_file_get_file_type (J2cIndexedFile *self)
{
  return J2C_FILE_TYPE_RESOURCE;
}

static GDataInputStream *
j2c_resource_file_read (J2cIndexedFile *self, GError **error)
{
    g_return_val_if_fail (NULL != self, NULL);
    J2cReadable *readable = j2c_indexed_file_get_readable (self);
    GInputStream *in = j2c_readable_read (readable, error);
    g_return_val_if_fail (NULL != in, NULL);

    GDataInputStream *datain = g_data_input_stream_new (in);
    g_object_unref (in);
    g_object_unref (readable);

    return datain;
}
