#include <j2c/method.h>

typedef struct
{

} J2cMethodPrivate;

G_DEFINE_ABSTRACT_TYPE (J2cMethod, j2c_method, G_TYPE_OBJECT)

static void
j2c_method_class_init (J2cMethodClass *klass)
{
  klass->get_descriptor = 0;
  klass->get_java_name = 0;
  klass->get_access_flags = 0;
}

static void
j2c_method_init (J2cMethod *self)
{
  return;
}

gchar *
j2c_method_get_descriptor (J2cMethod *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  J2cMethodClass *klass = J2C_METHOD_GET_CLASS (self);

  g_return_val_if_fail (klass->get_descriptor != NULL, NULL);
  return klass->get_descriptor (self);
}

gchar *
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
