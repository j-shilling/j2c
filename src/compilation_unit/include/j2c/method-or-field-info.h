#ifndef __FIELD_INFO_H__
#define __FIELD_INFO_H__


#include <glib.h>
#include <gio/gio.h>
#include <glib-object.h>

#include <j2c/constant-pool.h>

G_BEGIN_DECLS

#define J2C_TYPE_METHOD_OR_FIELD_INFO j2c_method_or_field_info_get_type ()
G_DECLARE_DERIVABLE_TYPE (J2cMethodOrFieldInfo, j2c_method_or_field_info, J2C, METHOD_OR_FIELD_INFO, GObject)
struct _J2cMethodOrFieldInfoClass
{
  GObjectClass parent_class;
};

#define J2C_TYPE_FIELD_INFO j2c_field_info_get_type()
G_DECLARE_FINAL_TYPE (J2cFieldInfo, j2c_field_info, J2C, FIELD_INFO, J2cMethodOrFieldInfo)
#define J2C_TYPE_METHOD_INFO j2c_method_info_get_type()
G_DECLARE_FINAL_TYPE (J2cMethodInfo, j2c_method_info, J2C, METHOD_INFO, J2cMethodOrFieldInfo)

J2cFieldInfo *j2c_field_info_from_class_file (GDataInputStream *in, J2cConstantPool *cp, GError **error);
J2cMethodInfo *j2c_method_info_from_class_file (GDataInputStream *in, J2cConstantPool *cp, GError **error);

G_END_DECLS

#endif // __FIELD_INFO_H__
