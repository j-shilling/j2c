#include <j2c/method.h>

typedef struct
{
  gchar *type;
} J2cMethodPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (J2cMethod, j2c_method, G_TYPE_OBJECT)

enum
{
  PROP_TYPE = 1
};

static void
j2c_method_dispose (GObject *object)
{
  J2cMethodPrivate *self = j2c_method_get_instance_private (J2C_METHOD (object));

  if (self->type)
    {
      g_free (self->type);
      self->type = NULL;
    }

  G_OBJECT_CLASS (j2c_method_parent_class)->dispose (object);
}

static void
j2c_method_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cMethodPrivate *priv = j2c_method_get_instance_private (J2C_METHOD (object));

  switch (property_id)
    {
    case PROP_TYPE:
      if (priv->type)
	g_free (priv->type);
      priv->type = g_value_dup_string (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_method_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cMethodPrivate *priv = j2c_method_get_instance_private (J2C_METHOD (object));

  switch (property_id)
    {
    case PROP_TYPE:
      g_value_set_string (value, priv->type);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_method_class_init (J2cMethodClass *klass)
{
  klass->get_descriptor = 0;
  klass->get_java_name = 0;
  klass->get_access_flags = 0;

  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->dispose = j2c_method_dispose;
  object_class->set_property = j2c_method_set_property;
  object_class->get_property = j2c_method_get_property;

  g_object_class_install_property (object_class, PROP_TYPE,
				   g_param_spec_string (J2C_METHOD_PROP_DEFINING_TYPE,
                                                        J2C_METHOD_PROP_DEFINING_TYPE,
							"",
							"",
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_method_init (J2cMethod *self)
{
  J2cMethodPrivate *priv = j2c_method_get_instance_private (self);
  priv->type = NULL;
}

const gchar *
j2c_method_get_descriptor (J2cMethod *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  J2cMethodClass *klass = J2C_METHOD_GET_CLASS (self);

  g_return_val_if_fail (klass->get_descriptor != NULL, NULL);
  return klass->get_descriptor (self);
}

const gchar *
j2c_method_get_java_name (J2cMethod *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  J2cMethodClass *klass = J2C_METHOD_GET_CLASS (self);

  g_return_val_if_fail (klass->get_java_name != NULL, NULL);
  return klass->get_java_name (self);
}

guint16
j2c_method_get_access_flags (J2cMethod *self)
{
  g_return_val_if_fail (self != NULL, 0);
  J2cMethodClass *klass = J2C_METHOD_GET_CLASS (self);

  g_return_val_if_fail (klass->get_access_flags != NULL, 0);
  return klass->get_access_flags (self);
}

gchar **
j2c_method_get_reference_types (J2cMethod *self, gchar **param_types, GError **error)
{
  g_return_val_if_fail (self != NULL, NULL);
  J2cMethodClass *klass = J2C_METHOD_GET_CLASS (self);

  g_return_val_if_fail (klass->get_reference_types != NULL, NULL);
  return klass->get_reference_types (self, param_types, error);
}

gchar *
j2c_method_get_defining_type (J2cMethod *self)
{
  g_return_val_if_fail (self != NULL, NULL);

  J2cMethodPrivate *priv = j2c_method_get_instance_private (self);
  return g_strdup (priv->type);
}
