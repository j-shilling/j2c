#ifndef __COMPILATION_UNIT_H__
#define __COMPILATION_UNIT_H__

#include <glib.h>
#include <glib-object.h>

#include <j2c/indexed-file.h>
#include <j2c/object-array.h>
#include <j2c/method.h>

G_BEGIN_DECLS

#define J2C_TYPE_COMPILATION_UNIT j2c_compilation_unit_get_type ()
G_DECLARE_INTERFACE (J2cCompilationUnit, j2c_compilation_unit, J2C, COMPILATION_UNIT, GObject)

struct _J2cCompilationUnitInterface
{
  GTypeInterface parent_iface;

  gchar *(*name) (J2cCompilationUnit *self);
  J2cObjectArray *(*get_type_dependencies) (J2cCompilationUnit *self);
  J2cMethod *(*get_method) (J2cCompilationUnit *self, gchar *java_name, gchar *descriptor);
  gchar *(*superclass_name) (J2cCompilationUnit *self);
  gchar **(*interfaces) (J2cCompilationUnit *self);
};

J2cCompilationUnit *j2c_compilation_unit_new (J2cIndexedFile *file, GError **error);
gchar *j2c_compilation_unit_name (J2cCompilationUnit *self);
gchar *j2c_compilation_unit_superclass_name (J2cCompilationUnit *self);
gchar **j2c_compilation_unit_interfaces (J2cCompilationUnit *self);
J2cObjectArray *j2c_compilation_unit_get_type_dependencies (J2cCompilationUnit *self);
J2cMethod *j2c_compilation_unit_get_method (J2cCompilationUnit *self, gchar *java_name, gchar *descriptor);

G_END_DECLS

#endif /* __COMPILATION_UNIT_H__ */
