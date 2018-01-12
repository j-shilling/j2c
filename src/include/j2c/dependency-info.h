#ifndef __DEPENDENCY_INFO_H__
#define __DEPENDENCY_INFO_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_TYPE_DEPENDENCY_INFO    j2c_dependency_info_get_type ()
G_DECLARE_FINAL_TYPE (J2cDependencyInfo, j2c_dependency_info, J2C, DEPENDENCY_INFO, GObject)

void j2c_dependency_info_requires_type (J2cDependencyInfo *self, gchar *java_name);

G_END_DECLS

#endif // __DEPENDENCY_INFO_H__
