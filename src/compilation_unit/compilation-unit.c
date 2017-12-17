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

gchar const *
j2c_compilation_unit_name (J2cCompilationUnit *self)
{
  g_return_val_if_fail (J2C_IS_COMPILATION_UNIT (self), NULL);
  J2cCompilationUnitInterface *iface = J2C_COMPILATION_UNIT_GET_IFACE (self);
  return iface->name (self);
}

