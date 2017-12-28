#ifndef __FIELD_INFO_H__
#define __FIELD_INFO_H__


#include <glib.h>
#include <gio/gio.h>
#include <glib-object.h>

#include <j2c/constant-pool.h>

G_BEGIN_DECLS

#define J2C_TYPE_FIELD_INFO j2c_field_info_get_type()
G_DECLARE_FINAL_TYPE (J2cFieldInfo, j2c_field_info, J2C, FIELD_INFO, GObject)

J2cFieldInfo *j2c_field_info_from_class_file (GDataInputStream *in, J2cConstantPool *cp, GError **error);

G_END_DECLS

#endif // __FIELD_INFO_H__
