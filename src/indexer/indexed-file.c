#include <j2c/indexed-file.h>
#include <j2c/resource-file.h>
#include <j2c/class-file.h>

typedef struct
{
  J2cReadable *readable;
} J2cIndexedFilePrivate;

G_DEFINE_TYPE_WITH_PRIVATE (J2cIndexedFile, j2c_indexed_file, G_TYPE_OBJECT)

/****
 * PRIVATE PROTOTYPES
 */

static void j2c_indexed_file_dispose (GObject *object);
static void j2c_indexed_file_set_property (GObject *object,
					 guint property_id,
					 const GValue *value,
					 GParamSpec *pspec);
static void j2c_indexed_file_get_property (GObject *object,
					 guint property_id,
					 GValue *value,
					 GParamSpec *pspec);
/****
 * CONSTRUCTION
 */

static void
j2c_indexed_file_class_init (J2cIndexedFileClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_indexed_file_set_property;
  object_class->get_property = j2c_indexed_file_get_property;
  object_class->dispose = j2c_indexed_file_dispose;

  g_object_class_install_property (object_class, 1,
				   g_param_spec_object (J2C_INDEXED_FILE_PROPERTY_READABLE,
							"readable",
							"A readable object.",
							J2C_TYPE_READABLE,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_indexed_file_init (J2cIndexedFile *self)
{
  return;
}

/****
 * DESTRUCTION
 */

static void
j2c_indexed_file_dispose (GObject *object)
{
  J2cIndexedFilePrivate *priv =
      j2c_indexed_file_get_instance_private (J2C_INDEXED_FILE (object));
  g_clear_object (&priv->readable);
  G_OBJECT_CLASS (j2c_indexed_file_parent_class)->dispose (object);
}

/****
 * PRIV IMPLEMENTATIONS
 */

static void
j2c_indexed_file_set_property (GObject *object, guint property_id,
			       const GValue *value, GParamSpec *pspec)
{
  if (property_id == 1)
    {
      J2cIndexedFilePrivate *priv =
            j2c_indexed_file_get_instance_private (J2C_INDEXED_FILE (object));
      if (priv->readable)
	g_warning ("IndexFile::readable can only be set once.");
      else
	priv->readable = g_value_dup_object (value);
    }
  else
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_indexed_file_get_property (GObject *object, guint property_id,
			       GValue *value, GParamSpec *pspec)
{
  if (property_id == 1)
    {
      J2cIndexedFilePrivate *priv =
            j2c_indexed_file_get_instance_private (J2C_INDEXED_FILE (object));
      g_value_set_object (value, priv->readable);
    }
  else
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}
/****
 * PUBLIC IMPLEMENTATIONS
 */

J2cIndexedFile *
j2c_indexed_file_new (J2cReadable *readable)
{
  GError *error = NULL;
  gpointer ret = j2c_class_file_new (readable, &error);
  if (ret) return ret;

  return J2C_INDEXED_FILE (j2c_resource_file_new (readable));
}

gchar *
j2c_indexed_file_get_name (J2cIndexedFile *self)
{
  J2cIndexedFileClass *klass = J2C_INDEXED_FILE_GET_CLASS (self);
  return klass->get_name (self);
}

J2cFileType
j2c_indexed_file_get_file_type (J2cIndexedFile *self)
{
  J2cIndexedFileClass *klass = J2C_INDEXED_FILE_GET_CLASS (self);
  return klass->get_file_type (self);
}

J2cReadable *
j2c_indexed_file_get_readable (J2cIndexedFile *self)
{
  g_return_val_if_fail (NULL != self, NULL);
  J2cIndexedFilePrivate *priv =
              j2c_indexed_file_get_instance_private (self);

  return g_object_ref (priv->readable);
}

GDataInputStream *
j2c_indexed_file_read (J2cIndexedFile *self, GError **error)
{
  J2cIndexedFileClass *klass = J2C_INDEXED_FILE_GET_CLASS (self);
  return klass->read (self, error);
}
