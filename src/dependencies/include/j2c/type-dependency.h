#ifndef __TYPE_DEPENDENCY_H__
#define __TYPE_DEPENDENCY_H__

#include <glib-object.h>
#include <j2c/method-dependency.h>
#include <j2c/field-dependency.h>

G_BEGIN_DECLS

#define J2C_TYPE_TYPE_DEPENDENCY	  j2c_type_dependency_get_type ()
G_DECLARE_FINAL_TYPE (J2cTypeDependency, j2c_type_dependency, J2C, TYPE_DEPENDENCY, GObject)

J2cTypeDependency *j2c_type_dependency_new (gchar *java_name);

void j2c_type_dependency_set_referenced (J2cTypeDependency *self);
void j2c_type_dependency_set_instantiated (J2cTypeDependency *self);

void j2c_type_dependency_set_parent (J2cTypeDependency *self, J2cTypeDependency *parent);
void j2c_type_dependency_add_child (J2cTypeDependency *self, J2cTypeDependency *child);
void j2c_type_dependency_add_field (J2cTypeDependency *self, J2cFieldDependency *field);
void j2c_type_dependency_add_method (J2cTypeDependency *self, J2cMethodDependency *method);
void j2c_type_dependency_add_interface (J2cTypeDependency *self, J2cTypeDependency *interface);

gboolean j2c_type_dependency_is_referenced (J2cTypeDependency *self);
gboolean j2c_type_dependency_is_derived (J2cTypeDependency *self);
gboolean j2c_type_dependency_is_instantiated (J2cTypeDependency *self);

J2cTypeDependency *j2c_type_dependency_get_child (J2cTypeDependency *self, gchar *name);
J2cFieldDependency *j2c_type_dependency_get_field (J2cTypeDependency *self, gchar *name);
J2cMethodDependency *j2c_type_dependency_get_method (J2cTypeDependency *self, gchar *name, gchar *descriptor);
J2cTypeDependency *j2c_type_dependency_get_interface (J2cTypeDependency *self, gchar *name);

G_END_DECLS

#endif /* __TYPE_DEPENDENCY_H__ */
