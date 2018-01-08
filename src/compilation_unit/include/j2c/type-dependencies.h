#ifndef __TYPE_DEPENDENCIES_H__
#define __TYPE_DEPENDENCIES_H__

#include <glib-object.h>

#define J2C_TYPE_TYPE_DEPENDENCIES  j2c_type_dependencies_get_type ()
G_DECLARE_FINAL_TYPE(J2cTypeDependencies, j2c_type_dependencies, J2C, TYPE_DEPENDENCIES, GObject)

J2cTypeDependencies *j2c_type_dependencies_new (gchar *this_type, gchar *super_class, gchar **interfaces, gchar **field_types);
gchar *j2c_type_dependencies_get_this_type (J2cTypeDependencies *self);
gchar *j2c_type_dependencies_get_super_class (J2cTypeDependencies *self);
gchar **j2c_type_dependencies_get_interfaces (J2cTypeDependencies *self);
gchar **j2c_type_dependencies_get_field_types (J2cTypeDependencies *self);

#endif // __TYPE_DEPENDENCIES_H__
