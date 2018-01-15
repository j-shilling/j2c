#include <j2c/compilation-unit.h>
#include <j2c/compilation-unit-class.h>

G_DEFINE_INTERFACE (J2cCompilationUnit, j2c_compilation_unit, G_TYPE_OBJECT);

static void
j2c_compilation_unit_default_init (J2cCompilationUnitInterface *iface)
{
  iface->name = 0;
}

J2cCompilationUnit *
j2c_compilation_unit_new (J2cIndexedFile *file, GError **error)
{
  g_return_val_if_fail (file != NULL, NULL);
  J2cFileType type = j2c_indexed_file_get_file_type (file);

  if (type == J2C_FILE_TYPE_CLASS)
    return J2C_COMPILATION_UNIT (j2c_compilation_unit_class_new (file, error));
  else
    return NULL;
}

gchar *
j2c_compilation_unit_name (J2cCompilationUnit *self)
{
  g_return_val_if_fail (J2C_IS_COMPILATION_UNIT (self), NULL);
  J2cCompilationUnitInterface *iface = J2C_COMPILATION_UNIT_GET_IFACE (self);
  return iface->name (self);
}

J2cObjectArray *
j2c_compilation_unit_get_type_dependencies (J2cCompilationUnit *self)
{
  g_return_val_if_fail (J2C_IS_COMPILATION_UNIT (self), NULL);
  J2cCompilationUnitInterface *iface = J2C_COMPILATION_UNIT_GET_IFACE (self);
  return iface->get_type_dependencies (self);
}

J2cMethod *
j2c_compilation_unit_get_method (J2cCompilationUnit *self, gchar *java_name, gchar *descriptor)
{
  g_return_val_if_fail (self != NULL, NULL);
  J2cCompilationUnitInterface *iface = J2C_COMPILATION_UNIT_GET_IFACE (self);

  g_return_val_if_fail (iface->get_method != NULL, NULL);
  return iface->get_method (self, java_name, descriptor);
}

gchar *
j2c_compilation_unit_superclass_name (J2cCompilationUnit *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  J2cCompilationUnitInterface *iface = J2C_COMPILATION_UNIT_GET_IFACE (self);

  g_return_val_if_fail (iface->superclass_name != NULL, NULL);
  return iface->superclass_name (self);
}

gchar **
j2c_compilation_unit_interfaces (J2cCompilationUnit *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  J2cCompilationUnitInterface *iface = J2C_COMPILATION_UNIT_GET_IFACE (self);

  g_return_val_if_fail (iface->interfaces != NULL, NULL);
  return iface->interfaces (self);
}
