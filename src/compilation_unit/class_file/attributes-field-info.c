#include <j2c/attributes.h>

struct _J2cAttributeConstantValue
{
  GObject parent;
  guint16 constant_value_index;
};

G_DEFINE_TYPE (J2cAttributeConstantValue, j2c_attribute_constant_value, G_TYPE_OBJECT)

/****
  PROPERTIES
 ****/

enum
{
  PROP_CONSTANT_VALUE_INDEX = 1,
};

static void j2c_attribute_constant_value_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_constant_value_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/****
  CONSTRUCTION METHODS
 ****/

static void
j2c_attribute_constant_value_class_init (J2cAttributeConstantValueClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_constant_value_set_property;
  object_class->get_property = j2c_attribute_constant_value_get_property;

  g_object_class_install_property (object_class, PROP_CONSTANT_VALUE_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_CONSTANT_VALUE_INDEX,
						      J2C_ATTRIBUTE_PROP_CONSTANT_VALUE_INDEX,
						      "",
						      0, G_MAXINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void j2c_attribute_constant_value_init (J2cAttributeConstantValue *self) { return; }

/****
  SETTERS AND GETTERS
 ****/

static void
j2c_attribute_constant_value_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeConstantValue *self = J2C_ATTRIBUTE_CONSTANT_VALUE (object);
  switch (property_id)
    {
    case PROP_CONSTANT_VALUE_INDEX:
      self->constant_value_index = (guint16) g_value_get_uint (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_attribute_constant_value_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeConstantValue *self = J2C_ATTRIBUTE_CONSTANT_VALUE (object);
  switch (property_id)
    {
    case PROP_CONSTANT_VALUE_INDEX:
      g_value_set_uint (value, (guint) self->constant_value_index);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

