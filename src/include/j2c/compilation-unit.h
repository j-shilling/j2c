#ifndef __COMPILATION_UNIT_H__
#define __COMPILATION_UNIT_H__

#include <glib.h>
#include <glib-object.h>

#include <j2c/indexed-file.h>
#include <j2c/object-array.h>

G_BEGIN_DECLS

#define J2C_TYPE_COMPILATION_UNIT j2c_compilation_unit_get_type ()
G_DECLARE_INTERFACE (J2cCompilationUnit, j2c_compilation_unit, J2C, COMPILATION_UNIT, GObject)

struct _J2cCompilationUnitInterface
{
  GTypeInterface parent_iface;

  gchar const *(*name) (J2cCompilationUnit *self);
  J2cObjectArray *(*get_type_dependencies) (J2cCompilationUnit *self);
};

J2cCompilationUnit *j2c_compilation_unit_new (J2cIndexedFile *file, GError **error);
gchar const *j2c_compilation_unit_name (J2cCompilationUnit *self);
J2cObjectArray *j2c_compilation_unit_get_type_dependencies (J2cCompilationUnit *self);

G_END_DECLS

#endif /* __COMPILATION_UNIT_H__ */
