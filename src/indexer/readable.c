#include <j2c/readable.h>

/****
 * READABLE
 */
G_DEFINE_INTERFACE (J2cReadable, j2c_readable, G_TYPE_OBJECT)

static void
j2c_readable_default_init (J2cReadableInterface *iface)
{
  g_return_if_fail (NULL != iface);
  iface->read = 0;
}

GInputStream *
j2c_readable_read (J2cReadable *self, GError **error)
{
  J2cReadableInterface *iface = J2C_READABLE_GET_IFACE (self);
  return iface->read (self, error);
}

const gchar *
j2c_readable_name (J2cReadable *self)
{
  J2cReadableInterface *iface = J2C_READABLE_GET_IFACE (self);
  return iface->name (self);
}

J2cReadable *
j2c_readable_new (GFile *file)
{
  return g_object_new (J2C_TYPE_READABLE_FILE, "file", file, NULL);
}

/****
 * READABLE_FILE
 */

struct _J2cReadableFile
{
  GObject parent_instance;
  GFile *file;
};

static void
j2c_readable_file_readable_interface_init (J2cReadableInterface *iface);

G_DEFINE_TYPE_WITH_CODE (J2cReadableFile, j2c_readable_file, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (J2C_TYPE_READABLE,
						j2c_readable_file_readable_interface_init))

static GInputStream *
j2c_readable_file_read (J2cReadable *_self, GError **error)
{
  g_return_val_if_fail (NULL != _self, NULL);
  J2cReadableFile *self = J2C_READABLE_FILE (_self);
  return G_INPUT_STREAM (g_file_read(self->file, NULL, error));
}

static const gchar *
j2c_readable_file_name (J2cReadable *_self)
{
  g_return_val_if_fail (NULL != _self, NULL);
  J2cReadableFile *self = J2C_READABLE_FILE (_self);
  return g_file_get_path (self->file);
}

static void
j2c_readable_file_readable_interface_init (J2cReadableInterface *iface)
{
  iface->read = j2c_readable_file_read;
  iface->name = j2c_readable_file_name;
}

static void
j2c_readable_file_set_property (GObject *object, guint property_id,
			       const GValue *value, GParamSpec *pspec)
{
  if (property_id == 1)
    {
      J2cReadableFile *self = J2C_READABLE_FILE (object);
      if (self->file)
	g_warning ("ReadableFile::file can only be set once.");
      else
	self->file = g_value_dup_object (value);
    }
  else
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_readable_file_get_property (GObject *object, guint property_id,
			        GValue *value, GParamSpec *pspec)
{
  if (property_id == 1)
    {
      J2cReadableFile *self = J2C_READABLE_FILE (object);
      g_value_set_object (value, self->file);
    }
  else
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_readable_file_init (J2cReadableFile *self)
{
  return;
}

static void
j2c_readable_file_dispose (GObject *object)
{
  J2cReadableFile *self = J2C_READABLE_FILE (object);
  g_clear_object (&self->file);
  G_OBJECT_CLASS (j2c_readable_file_parent_class)->dispose (object);
}

static void
j2c_readable_file_class_init (J2cReadableFileClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_readable_file_set_property;
  object_class->get_property = j2c_readable_file_get_property;
  object_class->dispose = j2c_readable_file_dispose;

  g_object_class_install_property (object_class, 1,
				   g_param_spec_object ("file",
							"file",
							"file",
							G_TYPE_FILE,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

