#ifndef __METHOD_DEPENDENCY_H__
#define __METHOD_DEPENDENCY_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_TYPE_METHOD_DEPENDENCY  j2c_method_dependency_get_type ()
G_DECLARE_FINAL_TYPE (J2cMethodDependency, j2c_method_dependency, J2C, METHOD_DEPENDENCY, GObject)

J2cMethodDependency *j2c_method_dependency_new (gchar *java_name, gchar *descriptor);

gchar *j2c_method_dependency_name (J2cMethodDependency *self);
gchar *j2c_method_dependency_descriptor (J2cMethodDependency *self);
gchar *j2c_method_dependency_defining_type (J2cMethodDependency *self);

gboolean j2c_method_dependency_is_called (J2cMethodDependency *self);
gboolean j2c_method_dependency_is_overrided (J2cMethodDependency *self);

J2cMethodDependency *j2c_method_dependency_get_overrided (J2cMethodDependency *self);
J2cMethodDependency *j2c_method_dependency_get_overrider (J2cMethodDependency *self, gchar *implementing_class_name);

void j2c_method_dependency_set_called (J2cMethodDependency *self);
void j2c_method_dependency_add_overrider (J2cMethodDependency *self, J2cMethodDependency *overrider);

G_END_DECLS

#endif /* __METHOD_DEPENDENCY_H__ */
