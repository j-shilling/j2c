#ifndef __DEPENDENCY_INFO_H__
#define __DEPENDENCY_INFO_H__

#include <glib.h>
#include <j2c/method.h>

void j2c_dependency_info_init (gint max_threads);

void j2c_dependency_info_add_method_call (J2cMethod *method, gchar **param_types);
void j2c_dependency_info_add_reference (gchar *type_descriptor);
void j2c_dependency_info_lock (void);

#endif // __DEPENDENCY_INFO_H__
