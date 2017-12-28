#include <j2c/field-info.h>
#include <j2c/attributes.h>

struct _J2cFieldInfo
{
  guint16 access_flags;
  guint16 name_index;
  guint16 descriptor_index;
  GPtrArray *attributes;
};

G_DEFINE_TYPE (J2cFieldInfo, j2c_field_info, G_TYPE_OBJECT)

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

static gchar const *const ACCESS_FLAGS = "access-flags";
static gchar const *const NAME_INDEX = "name-index";
static gchar const *const DESCRIPTOR_INDEX = "descriptor-index";
static gchar const *const ATTRIBUTES = "attributes";

/****
  PRIVATE METHOD PROTOTYPES
 ****/

static void j2c_field_info_dispose (GObject *object);
static void j2c_field_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_field_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/****
  CONSTRUCTION METHODS
 ****/

J2cFieldInfo *
j2c_field_info_from_class_file (GDataInputStream *in, J2cConstantPool *cp, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (in != NULL, NULL);
  g_return_val_if_fail (cp != NULL, NULL);

  GError *tmp_error = NULL;

  guint16 access_flags = 0;
  guint16 name_index = 0;
  guint16 descriptor_index = 0;
  GPtrArray *attributes = NULL;

  access_flags = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  descriptor_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  guint16 attributes_count = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  attributes = g_ptr_array_sized_new (attributes_count);
  g_ptr_array_set_free_func (attributes, g_object_unref);
  for (gint i = 0; i < attributes_count; i ++)
    {
      gpointer attribute = j2c_read_attribute (J2C_TYPE_FIELD_INFO, in, cp, &tmp_error);
      if (tmp_error)
        goto error;
      else
        g_ptr_array_add (attributes, attribute);
    }

success:
  return g_object_new (J2C_TYPE_FIELD_INFO,
                       ACCESS_FLAGS, access_flags,
                       NAME_INDEX, name_index,
                       DESCRIPTOR_INDEX, descriptor_index,
                       ATTRIBUTES, attributes);
error:
  if (tmp_error)
    g_propagate_error (error, tmp_error);
  if (attributes)
    g_ptr_array_unref (attributes);
  return NULL;
}

static void
j2c_field_info_class_init (J2cFieldInfoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_field_info_set_property;
  object_class->get_property = j2c_field_info_get_property;
  object_class->dispose      = j2c_field_info_dispose;

  obj_properties[PROP_ACCESS_FLAGS] =
    g_param_spec_uint (ACCESS_FLAGS,
                       ACCESS_FLAGS,
                       "The value of the access_flags item is a "
                       "mask of flags used to denote access permission "
                       "to and properties of this field.",
                       0, G_MAXUINT16,
                       0,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_NAME_INDEX] =
    g_param_spec_uint (NAME_INDEX,
                       NAME_INDEX,
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
    g_param_spec_uint (DESCRIPTOR_INDEX,
                       DESCRIPTOR_INDEX,
                       "The value of the descriptor_index item must be "
                       "a valid index into the constant_pool table. The "
                       "constant_pool entry at that index must be a "
                       "CONSTANT_Utf8_info structure (§4.4.7) which "
                       "represents a valid field descriptor (§4.3.2). ",
                       0, G_MAXUINT16,
                       0,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_ATTRIBUTES] =
    g_param_spec_boxed (ATTRIBUTES,
                        ATTRIBUTES,
                        "Attributes associated with this field.",
                        G_TYPE_PTR_ARRAY,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

static void
j2c_field_info_init (J2cFieldInfo *self)
{
  return;
}


/****
  DESTRUCTION METHODS
 ****/

static void
j2c_field_info_dispose (GObject *object)
{
  J2cFieldInfo *self = J2C_FIELD_INFO (object);

  if (self->attributes)
    {
      g_ptr_array_unref (self->attributes);
      self->attributes = NULL;
    }

  G_OBJECT_CLASS (j2c_field_info_parent_class)->dispose (object);
}

/****
  SETTERS AND GETTERS
 ****/

static void
j2c_field_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cFieldInfo *self = J2C_FIELD_INFO (object);

  switch (property_id)
  {
    case PROP_ACCESS_FLAGS:
      self->access_flags = (guint16) g_value_get_uint (value);
      break;
    case PROP_NAME_INDEX:
      self->name_index = (guint16) g_value_get_uint (value);
      break;
    case PROP_DESCRIPTOR_INDEX:
      self->descriptor_index = (guint16) g_value_get_uint (value);
      break;
    case PROP_ATTRIBUTES:
      if (self->attributes)
        g_ptr_array_unref (self->attributes);
      self->attributes = g_value_get_boxed (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
j2c_field_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cFieldInfo *self = J2C_FIELD_INFO (object);

  switch (property_id)
  {
    case PROP_ACCESS_FLAGS:
      g_value_set_uint (value, (guint) self->access_flags);
      break;
    case PROP_NAME_INDEX:
      g_value_set_uint (value, (guint) self->name_index);
      break;
    case PROP_DESCRIPTOR_INDEX:
      g_value_set_uint (value, (guint) self->descriptor_index);
      break;
    case PROP_ATTRIBUTES:
      g_value_set_boxed (value, self->attributes);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}
