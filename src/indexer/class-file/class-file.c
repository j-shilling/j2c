#include "../include/indexer/class-file.h"

struct _J2cClassFile
{
  GObject parent_instance;

  GFile *file;
  gchar *name;
};

/* structures related to object properties */
enum
{
  PROP_FILE,
  PROP_NAME,
  N_PROPERTIES
};

static const gchar *const J2C_CLASS_FILE_PROPERTY_FILE = "class-file::file";
static const gchar *const J2C_CLASS_FILE_PROPERTY_NAME = "class-file::name";
static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void j2c_class_file_dispose (GObject *object);

/* private methods */
static void j2c_class_file_initable_interface_init (GInitableIface *iface);
static void j2c_class_file_set_property (GObject *object,
					 guint property_id,
					 const GValue *value,
					 GParamSpec *pspec);

static void j2c_class_file_get_property (GObject *object,
					 guint property_id,
					 GValue *value,
					 GParamSpec *pspec);

static GFile *j2c_class_file_get_file (J2cClassFile *self);
static gchar *j2c_class_file_get_name (J2cClassFile *self);
static GDataInputStream *j2c_class_file_read (J2cClassFile *self,
					 GCancellable *cancellable,
					 GError **error);

static gboolean j2c_class_file_initable_init (GInitable *initable,
					      GCancellable *cancellable,
					      GError **error);

/* Error Code Quark */
GQuark
j2c_class_file_quark (void)
{
  return g_quark_from_static_string ("j2c-class-file");
}

G_DEFINE_TYPE_WITH_CODE (J2cClassFile, j2c_class_file, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
						j2c_class_file_initable_interface_init));

/****
  CONSTRUCTION METHODS
 ****/

J2cClassFile *
j2c_class_file_new (GFile *file, GError **error)
{
  return g_initable_new (J2C_TYPE_CLASS_FILE,
			 NULL,
			 error,
			 J2C_CLASS_FILE_PROPERTY_FILE,
			 file,
			 NULL);
}

static void
j2c_class_file_class_init (J2cClassFileClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = j2c_class_file_set_property;
  object_class->get_property = j2c_class_file_get_property;
  object_class->dispose = j2c_class_file_dispose;

  obj_properties[PROP_FILE] =
    g_param_spec_object (J2C_CLASS_FILE_PROPERTY_FILE,
			 "file",
			 "Identifier for the local .class file",
			 G_TYPE_FILE,
			 G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_FILE] =
    g_param_spec_string (J2C_CLASS_FILE_PROPERTY_NAME,
			 "name",
			 "Fully-quallified name of the object in the classfile"
			 " (e.g. java.lang.Optional)",
			 NULL,
			 G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (object_class,
				     N_PROPERTIES,
				     obj_properties);
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

static gboolean
j2c_class_file_initable_init (GInitable *initable,
			      GCancellable *cancellable,
			      GError **error)
{
  J2cClassFile *self = J2C_CLASS_FILE (initable);
  GDataInputStream *in = j2c_class_file_read (self, cancellable, error);
  g_return_val_if_fail (NULL != in, FALSE);

  /* Check if this is a class file */
  guint32 magic = g_data_input_stream_read_uint32 (in, cancellable, error);
  if (error)
    {
      g_object_unref (in);
      return FALSE;
    }

  if (magic != J2C_CLASS_FILE_MAGIC)
    {
      g_set_error (error,
		   J2C_CLASS_FILE_ERROR,
	           J2C_CLASS_FILE_NOT_A_CLASS_FILE_ERROR,
		   "This is not a class file.");
      g_object_unref (in);
      return FALSE;
    }

  /* Initialize name */
  
  /* End */
  g_object_unref (in);
  return TRUE;
}


/****
  DESTRUCTION METHODS
 ****/

static void
j2c_class_file_dispose (GObject *object)
{
  J2cClassFile *self = J2C_CLASS_FILE (object);

  g_clear_object (&self->file);
  g_free (self->name);
  self->name = NULL;

  G_OBJECT_CLASS (j2c_class_file_parent_class)->dispose (object);
}
  

/****
  PRIV METHOD IMPLEMENTATIONS
 ****/

static void
j2c_class_file_set_property (GObject *object,
	     		     guint property_id,
			     const GValue *value,
			     GParamSpec *pspec)
{
  J2cClassFile *self = J2C_CLASS_FILE (object);

  switch (property_id)
    {
    case PROP_FILE:
      if (self->file)
	{
	  g_warning ("%s can only be set once.", J2C_CLASS_FILE_PROPERTY_FILE);
	  break;
	}

      self->file = g_value_dup_object (value);
      break;

    case PROP_NAME:
      if (self->name)
	{
	  g_warning ("%s can only be set once.", J2C_CLASS_FILE_PROPERTY_NAME);
	  break;
	}

      self->name = g_value_dup_string (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_class_file_get_property (GObject *object,
			      guint property_id,
			      GValue *value,
			      GParamSpec *pspec)
{
  J2cClassFile *self = J2C_CLASS_FILE (object);

  switch (property_id)
    {
    case PROP_FILE:
      g_value_set_object (value, self->file);
      break;

    case PROP_NAME:
      g_value_set_string (value, self->name);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static GFile *
j2c_class_file_get_file (J2cClassFile *self)
{
  g_return_val_if_fail (NULL != self, NULL);

  GValue val = G_VALUE_INIT;
  g_value_init (&val, G_TYPE_FILE);

  g_object_get_property (G_OBJECT(self),
			 J2C_CLASS_FILE_PROPERTY_FILE,
			 &val);
  GFile *ret = g_value_dup_object (&val);
  g_value_unset (&val);
  return ret;
}

static gchar *
j2c_class_file_get_name (J2cClassFile *self)
{
  g_return_val_if_fail (NULL != self, NULL);

  GValue val = G_VALUE_INIT;
  g_value_init (&val, G_TYPE_STRING);

  g_object_get_property (G_OBJECT(self),
			 J2C_CLASS_FILE_PROPERTY_NAME,
			 &val);
  gchar *ret = g_value_dup_string (&val);
  g_value_unset (&val);
  return ret;
}

static GDataInputStream *
j2c_class_file_read (J2cClassFile *self,
		     GCancellable *cancellable,
		     GError **error)
{
  g_return_val_if_fail (NULL != self, NULL);

  GFile *file = j2c_class_file_get_file (self);
  g_return_val_if_fail (NULL != file, NULL);

  GFileInputStream *filein = g_file_read (file, cancellable, error);
  g_return_val_if_fail (NULL != filein, NULL);
  g_object_unref (file);

  GDataInputStream *datain = g_data_input_stream_new (G_INPUT_STREAM(filein));
  g_object_unref (filein);
  g_return_val_if_fail (NULL != datain, NULL);

  g_data_input_stream_set_byte_order (datain,
				      G_DATA_STREAM_BYTE_ORDER_BIG_ENDIAN);

  return datain;
}
