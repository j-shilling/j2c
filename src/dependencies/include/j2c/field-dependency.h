#ifndef __FIELD_DEPENDENCY_H__
#define __FIELD_DEPENDENCY_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_TYPE_FIELD_DEPENDENCY j2c_field_dependency_get_type ()
G_DECLARE_FINAL_TYPE (J2cFieldDependency, j2c_field_dependency, J2C, FIELD_DEPENDENCY, GObject)

J2cFieldDependency *j2c_field_dependency_new (gchar *name);

gboolean j2c_field_dependency_is_read (J2cFieldDependency *self);
gboolean j2c_field_dependency_is_written (J2cFieldDependency *self);

void j2c_field_dependency_set_written (J2cFieldDependency *self);
void j2c_field_dependency_set_read (J2cFieldDependency *self);

G_END_DECLS

#endif /* __FIELD_DEPENDENCY_H__ */
