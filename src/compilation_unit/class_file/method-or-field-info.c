#include <j2c/method-or-field-info.h>
#include <j2c/attributes.h>
#include <j2c/logger.h>
#include <j2c/object-array.h>

typedef struct
{
  guint16 access_flags;
  guint16 name_index;
  guint16 descriptor_index;
  J2cObjectArray *attributes;
} J2cMethodOrFieldInfoPrivate   ;

G_DEFINE_TYPE_WITH_PRIVATE (J2cMethodOrFieldInfo, j2c_method_or_field_info, G_TYPE_OBJECT)

struct _J2cFieldInfo
{
  J2cMethodOrFieldInfo parent;
};

G_DEFINE_TYPE (J2cFieldInfo, j2c_field_info, J2C_TYPE_METHOD_OR_FIELD_INFO)

struct _J2cMethodInfo
{
  J2cMethodOrFieldInfo parent;
};

G_DEFINE_TYPE (J2cMethodInfo, j2c_method_info, J2C_TYPE_METHOD_OR_FIELD_INFO)

/****
  PROPERTIES
 ****/

enum
{
  PROP_ACCESS_FLAGS = 1,
  PROP_NAME_INDEX,
  PROP_DESCRIPTOR_INDEX,
  PROP_ATTRIBUTES,

  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

/****
  PRIVATE METHOD PROTOTYPES
 ****/

static void j2c_method_or_field_info_dispose (GObject *object);
static void j2c_method_or_field_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_method_or_field_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/****
  CONSTRUCTION METHODS
 ****/

static J2cMethodOrFieldInfo *
j2c_method_or_field_info_from_class_file (GType type, GDataInputStream *in, J2cConstantPool *cp, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (in != NULL, NULL);
  g_return_val_if_fail (cp != NULL, NULL);

  GError *tmp_error = NULL;
  J2cMethodOrFieldInfo *ret = NULL;

  guint16 access_flags = 0;
  guint16 name_index = 0;
  guint16 descriptor_index = 0;
  J2cObjectArray *attributes = NULL;

  access_flags = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  descriptor_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  guint16 attributes_count = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  attributes = j2c_object_array_sized_new (attributes_count);
  for (gint i = 0; i < attributes_count; i ++)
    {
      gpointer attribute = j2c_read_attribute (type, in, cp, &tmp_error);
      if (tmp_error)
        {
          j2c_logger_warning ("Could not read attribute: %s.", tmp_error->message);
          g_error_free (tmp_error);
          tmp_error = NULL;
        }
      else
        {
          j2c_object_array_add (attributes, attribute);
          g_object_unref (attribute);
        }
    }

success:
  ret = g_object_new (type,
                       METHOD_OR_FIELD_PROP_ACCESS_FLAGS, access_flags,
                       METHOD_OR_FIELD_PROP_NAME_INDEX, name_index,
                       METHOD_OR_FIELD_PROP_DESCRIPTOR_INDEX, descriptor_index,
                       METHOD_OR_FIELD_PROP_ATTRIBUTES, attributes,
                       NULL);
error:
  if (tmp_error)
    g_propagate_error (error, tmp_error);
  if (attributes)
    g_object_unref (attributes);
  return ret;
}

J2cFieldInfo *
j2c_field_info_from_class_file (GDataInputStream *in, J2cConstantPool *cp, GError **error)
{

  GError *tmp_error = NULL;
  J2cMethodOrFieldInfo *ret =
    j2c_method_or_field_info_from_class_file (J2C_TYPE_FIELD_INFO, in, cp, &tmp_error);

  if (tmp_error)
    g_propagate_error (error, tmp_error);

  if (ret)
    return J2C_FIELD_INFO (ret);
  else
    return NULL;
}

J2cMethodInfo *
j2c_method_info_from_class_file (GDataInputStream *in, J2cConstantPool *cp, GError **error)
{

  GError *tmp_error = NULL;
  J2cMethodOrFieldInfo *ret =
    j2c_method_or_field_info_from_class_file (J2C_TYPE_METHOD_INFO, in, cp, &tmp_error);

  if (tmp_error)
    g_propagate_error (error, tmp_error);

  if (ret)
    return J2C_METHOD_INFO (ret);
  else
    return NULL;
}


static void
j2c_method_or_field_info_class_init (J2cMethodOrFieldInfoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_method_or_field_info_set_property;
  object_class->get_property = j2c_method_or_field_info_get_property;
  object_class->dispose      = j2c_method_or_field_info_dispose;

  obj_properties[PROP_ACCESS_FLAGS] =
    g_param_spec_uint (METHOD_OR_FIELD_PROP_ACCESS_FLAGS,
                       METHOD_OR_FIELD_PROP_ACCESS_FLAGS,
                       "The value of the access_flags item is a "
                       "mask of flags used to denote access permission "
                       "to and properties of this field.",
                       0, G_MAXUINT16,
                       0,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_NAME_INDEX] =
    g_param_spec_uint (METHOD_OR_FIELD_PROP_NAME_INDEX,
                       METHOD_OR_FIELD_PROP_NAME_INDEX,
                       "The value of the name_index item must be a "
                       "valid index into the constant_pool table. The "
                       "constant_pool entry at that index must be a "
                       "CONSTANT_Utf8_info structure (§4.4.7) which "
                       "represents a valid unqualified name denoting "
                       "a field",
                       0, G_MAXUINT16,
                       0,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_DESCRIPTOR_INDEX] =
    g_param_spec_uint (METHOD_OR_FIELD_PROP_DESCRIPTOR_INDEX,
                       METHOD_OR_FIELD_PROP_DESCRIPTOR_INDEX,
                       "The value of the descriptor_index item must be "
                       "a valid index into the constant_pool table. The "
                       "constant_pool entry at that index must be a "
                       "CONSTANT_Utf8_info structure (§4.4.7) which "
                       "represents a valid field descriptor (§4.3.2). ",
                       0, G_MAXUINT16,
                       0,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_ATTRIBUTES] =
    g_param_spec_object (METHOD_OR_FIELD_PROP_ATTRIBUTES,
                        METHOD_OR_FIELD_PROP_ATTRIBUTES,
                        "Attributes associated with this field.",
                        J2C_TYPE_OBJECT_ARRAY,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

static void
j2c_field_info_class_init (J2cFieldInfoClass *klass)
{
  return;
}

static void
j2c_method_info_class_init (J2cMethodInfoClass *klass)
{
  return;
}

static void
j2c_method_or_field_info_init (J2cMethodOrFieldInfo *self)
{
  return;
}

static void
j2c_field_info_init (J2cFieldInfo *self)
{
  return;
}

static void
j2c_method_info_init (J2cMethodInfo *self)
{
  return;
}

/****
  DESTRUCTION METHODS
 ****/

static void
j2c_method_or_field_info_dispose (GObject *object)
{
  J2cMethodOrFieldInfoPrivate *priv =
    j2c_method_or_field_info_get_instance_private (J2C_METHOD_OR_FIELD_INFO (object));

  if (priv->attributes)
    g_clear_object (&priv->attributes);

  G_OBJECT_CLASS (j2c_method_or_field_info_parent_class)->dispose (object);
}

/****
  SETTERS AND GETTERS
 ****/

static void
j2c_method_or_field_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cMethodOrFieldInfoPrivate *priv =
    j2c_method_or_field_info_get_instance_private (J2C_METHOD_OR_FIELD_INFO (object));

  switch (property_id)
  {
    case PROP_ACCESS_FLAGS:
      priv->access_flags = (guint16) g_value_get_uint (value);
      break;
    case PROP_NAME_INDEX:
      priv->name_index = (guint16) g_value_get_uint (value);
      break;
    case PROP_DESCRIPTOR_INDEX:
      priv->descriptor_index = (guint16) g_value_get_uint (value);
      break;
    case PROP_ATTRIBUTES:
      if (priv->attributes)
        g_object_unref (priv->attributes);
      priv->attributes = g_value_dup_object (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
j2c_method_or_field_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cMethodOrFieldInfoPrivate *priv =
    j2c_method_or_field_info_get_instance_private (J2C_METHOD_OR_FIELD_INFO (object));

  switch (property_id)
  {
    case PROP_ACCESS_FLAGS:
      g_value_set_uint (value, (guint) priv->access_flags);
      break;
    case PROP_NAME_INDEX:
      g_value_set_uint (value, (guint) priv->name_index);
      break;
    case PROP_DESCRIPTOR_INDEX:
      g_value_set_uint (value, (guint) priv->descriptor_index);
      break;
    case PROP_ATTRIBUTES:
      g_value_set_object (value, priv->attributes);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}
