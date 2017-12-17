#ifndef __COMPILATION_UNIT_CLASS_H__
#define __COMPILATION_UNIT_CLASS_H__

#include <glib.h>
#include <glib-object.h>

#include <j2c/compilation-unit.h>
#include <j2c/indexed-file.h>

G_BEGIN_DECLS

#define J2C_TYPE_COMPILATION_UNIT_CLASS	j2c_compilation_unit_class_get_type ()
G_DECLARE_FINAL_TYPE (J2cCompilationUnitClass, j2c_compilation_unit_class, J2C, COMPILATION_UNIT_CLASS, GObject)

J2cCompilationUnitClass *j2c_compilation_unit_class_new (J2cIndexedFile *file, GError **error);

G_END_DECLS

#endif /* __COMPILATION_UNIT_CLASS_H__ */
