#ifndef __CONSTANT_POOL_ITEM_H__
#define __CONSTANT_POOL_ITEM_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

typedef enum
{
  J2C_CONSTANT_POOL_MEM_ERROR,
  J2C_CONSTANT_POOL_INVALID_TAG_ERROR,
  J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR
} J2cConstantPoolItemError;

#define J2C_CONSTANT_POOL_ITEM_ERROR	g_quark_from_static_string ("j2c-constant-pool-item")

#define J2C_TYPE_CONSTANT_POOL_ITEM_TAG	  j2c_constant_pool_item_tag_get_type ()
#define J2C_TYPE_CONSTANT_POOL_ITEM j2c_constant_pool_item_get_type ()
#define J2C_TYPE_CLASS_INFO j2c_class_info_get_type ()
#define J2C_TYPE_FIELDREF_INFO j2c_fieldref_info_get_type ()
#define J2C_TYPE_METHODREF_INFO j2c_methodref_info_get_type ()
#define J2C_TYPE_INTERFACE_METHODREF_INFO j2c_interface_methodref_info_get_type ()
#define J2C_TYPE_STRING_INFO j2c_string_info_get_type ()
#define J2C_TYPE_INTEGER_INFO j2c_integer_info_get_type ()
#define J2C_TYPE_FLOAT_INFO j2c_float_info_get_type ()
#define J2C_TYPE_LONG_INFO j2c_long_info_get_type ()
#define J2C_TYPE_DOUBLE_INFO j2c_double_info_get_type ()
#define J2C_TYPE_NAME_AND_TYPE_INFO j2c_name_and_type_info_get_type ()
#define J2C_TYPE_UTF8_INFO j2c_utf8_info_get_type ()
#define J2C_TYPE_METHOD_HANDLE_INFO j2c_method_handle_info_get_type ()
#define J2C_TYPE_METHOD_TYPE_INFO j2c_method_type_info_get_type ()
#define J2C_TYPE_INVOKE_DYNAMIC_INFO j2c_invoke_dynamic_info_get_type ()

typedef enum
{

  CONSTANT_Class              =  7,
  CONSTANT_Fieldref           =  9,
  CONSTANT_Methodref          = 10,
  CONSTANT_InterfaceMethodref = 11,
  CONSTANT_String             =  8,
  CONSTANT_Integer            =  3,
  CONSTANT_Float              =  4,
  CONSTANT_Long               =  5,
  CONSTANT_Double             =  6,
  CONSTANT_NameAndType        = 12,
  CONSTANT_Utf8               =  1,
  CONSTANT_MethodHandle       = 15,
  CONSTANT_MethodType         = 16,
  CONSTANT_InvokeDynamic      = 18

} J2cConstantPoolItemTag;

G_DECLARE_DERIVABLE_TYPE (J2cConstantPoolItem, j2c_constant_pool_item, J2C, CONSTANT_POOL_ITEM, GObject);
struct _J2cConstantPoolItemClass
{
  GObjectClass parent_class;
};

G_DECLARE_FINAL_TYPE (J2cClassInfo, j2c_class_info, J2C, CLASS_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cFieldrefInfo, j2c_fieldref_info, J2C, FIELDREF_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cMethodrefInfo, j2c_methodref_info, J2C, METHODREF_INFO, J2cFieldrefInfo);
G_DECLARE_FINAL_TYPE (J2cInterfaceMethodrefInfo, j2c_interface_methodref_info, J2C, INTERFACE_METHODREF_INFO, J2cFieldrefInfo);
G_DECLARE_FINAL_TYPE (J2cStringInfo, j2c_string_info, J2C, STRING_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cIntegerInfo, j2c_integer_info, J2C, INTEGER_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cFloatInfo, j2c_float_info, J2C, FLOAT_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cLongInfo, j2c_long_info, J2C, LONG_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cDoubleInfo, j2c_double_info, J2C, DOUBLE_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cNameAndTypeInfo, j2c_name_and_type_info, J2C, NAME_AND_TYPE_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cUtf8Info, j2c_utf8_info, J2C, UTF8_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cMethodHandleInfo, j2c_method_handle_info, J2C, METHOD_HANDLE_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cMethodTypeInfo, j2c_method_type_info, J2C, METHOD_TYPE_INFO, J2cConstantPoolItem);
G_DECLARE_FINAL_TYPE (J2cInvokeDynamicInfo, j2c_invoke_dynamic_info, J2C, INVOKE_DYNAMIC_INFO, J2cConstantPoolItem);

GType j2c_constant_pool_item_tag_get_type (void);
J2cConstantPoolItem *j2c_constant_pool_item_new (GDataInputStream *in, GError **error);

guint16 j2c_class_info_name_index (J2cClassInfo *self);
guint16 j2c_fieldref_info_class_index (J2cFieldrefInfo *self);
guint16 j2c_fieldref_info_name_and_type_index (J2cFieldrefInfo *self);
guint16 j2c_methodref_info_class_index (J2cMethodrefInfo *self);
guint16 j2c_methodref_info_name_and_type_index (J2cMethodrefInfo *self);
guint16 j2c_interface_methodref_info_class_index (J2cInterfaceMethodrefInfo *self);
guint16 j2c_interface_methodref_info_name_and_type_index (J2cInterfaceMethodrefInfo *self);
guint16 j2c_string_info_string_index (J2cStringInfo *self);
gint32 j2c_integer_info_value (J2cIntegerInfo *self);
gfloat j2c_float_info_value (J2cFloatInfo *self);
gint64 j2c_long_info_value (J2cLongInfo *self);
gdouble j2c_double_info_value (J2cDoubleInfo *self);
guint16 j2c_name_and_type_info_name_index (J2cNameAndTypeInfo *self);
guint16 j2c_name_and_type_info_descriptor_index (J2cNameAndTypeInfo *self);
const gchar *j2c_utf8_info_string (J2cUtf8Info *self);
guint8 j2c_method_handle_info_reference_kind (J2cMethodHandleInfo *self);
guint16 j2c_method_handle_info_reference_index (J2cMethodHandleInfo *self);
guint16 j2c_method_type_info_descriptor_index (J2cMethodTypeInfo *self);
guint16 j2c_invoke_dynamic_info_bootstrap_method_attr_index (J2cInvokeDynamicInfo *self);
guint16 j2c_invoke_dynamic_info_name_and_type_index (J2cInvokeDynamicInfo *self);

G_END_DECLS

#endif /* __CONSTANT_POOL_ITEM_H__ */
