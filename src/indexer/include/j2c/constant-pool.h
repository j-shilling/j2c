#ifndef __CONSTANT_POOL_H__
#define __CONSTANT_POOL_H__

#include <glib.h>
#include <glib-object.h>

#include <j2c/class-file.h>

/* Tag values identify types of structures in the constant pool */
static const guint8 CONSTANT_Invalid            =  0;
static const guint8 CONSTANT_Class              =  7;
static const guint8 CONSTANT_Fieldref           =  9;
static const guint8 CONSTANT_Methodref          = 10;
static const guint8 CONSTANT_InterfaceMethodref = 11;
static const guint8 CONSTANT_String             =  8;
static const guint8 CONSTANT_Integer            =  3;
static const guint8 CONSTANT_Float              =  4;
static const guint8 CONSTANT_Long               =  5;
static const guint8 CONSTANT_Double             =  6;
static const guint8 CONSTANT_NameAndType        = 12;
static const guint8 CONSTANT_Utf8               =  1;
static const guint8 CONSTANT_MethodHandle       = 15;
static const guint8 CONSTANT_MethodType         = 16;
static const guint8 CONSTANT_InvokeDynamic      = 18;

/* GObject property names */
static const gchar * J2C_CONSTANT_POOL_PROP_POOL                        = "constant-pool-pool";
static const gchar * J2C_CONSTANT_POOL_PROP_SIZE                        = "cosntant-pool-size";
static const gchar * J2C_CONSTANT_POOL_PROP_TAG                         = "constant-pool-tag";
static const gchar * J2C_CONSTANT_POOL_PROP_COUNT                       = "constant-pool-count";
static const gchar * J2C_CONSTANT_POOL_PROP_NAME_INDEX                  = "constant-pool-name-index";
static const gchar * J2C_CONSTANT_POOL_PROP_CLASS_INDEX                 = "constant-pool-class-index";
static const gchar * J2C_CONSTANT_POOL_PROP_NAME_AND_TYPE_INDEX         = "constant-pool-name-and-type-index";
static const gchar * J2C_CONSTANT_POOL_PROP_STRING_INDEX                = "constant-pool-string-index";
static const gchar * J2C_CONSTANT_POOL_PROP_VALUE                       = "constant-pool-value";
static const gchar * J2C_CONSTANT_POOL_PROP_DESCRIPTOR_INDEX            = "constant-pool-descriptor-index";
static const gchar * J2C_CONSTANT_POOL_PROP_LENGTH                      = "constant-pool-length";
static const gchar * J2C_CONSTANT_POOL_PROP_BYTES                       = "constant-pool-bytes";
static const gchar * J2C_CONSTANT_POOL_PROP_REFERENCE_KIND              = "constant-pool-refrence-kind";
static const gchar * J2C_CONSTANT_POOL_PROP_REFERENCE_INDEX             = "constant-pool-reference-index";
static const gchar * J2C_CONSTANT_POOL_PROP_BOOTSTRAP_METHOD_ATTR_INDEX = "constant-pool-bootstrap-method-attr-index";

typedef enum
{
  J2C_CONSTANT_POOL_NULL_ERROR,
  J2C_CONSTANT_POOL_INDEX_ERROR,
  J2C_CONSTANT_POOL_TYPE_ERROR,
  J2C_CONSTANT_POOL_INVALID_TAG_ERROR,
  J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
  J2C_CONSTANT_POOL_INVALID_UTF8_ERROR
} J2cConstantPoolError;

#define J2C_CONSTANT_POOL_ERROR	g_quark_from_static_string ("j2c-constant-pool")

G_BEGIN_DECLS

#define J2C_TYPE_CONSTANT_POOL j2c_constant_pool_get_type ()
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

G_DECLARE_FINAL_TYPE (J2cConstantPool, j2c_constant_pool, J2C, CONSTANT_POOL, GObject);

G_DECLARE_DERIVABLE_TYPE (J2cConstantPoolItem, j2c_constant_pool_item, J2C, CONSTANT_POOL_ITEM, GObject);

struct _J2cConstantPoolItemClass
{
  GObjectClass parent_class;
};

/* Class Pool Item Child Types */
G_DECLARE_FINAL_TYPE (J2cClassInfo, j2c_class_info, J2C, CLASS_INFO, J2cConstantPoolItem);

G_DECLARE_DERIVABLE_TYPE (J2cFieldrefInfo, j2c_fieldref_info, J2C, FIELDREF_INFO, J2cConstantPoolItem);
struct _J2cFieldrefInfoClass
{
  J2cConstantPoolItemClass parent_class;
};
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

J2cConstantPool *j2c_constant_pool_new (J2cClassFile *file, GError **error);
J2cConstantPoolItem *j2c_constant_pool_get_item (J2cConstantPool *self, guint16 index, GError **error);
gsize j2c_constant_pool_size (J2cConstantPool *self);
J2cConstantPoolItem *j2c_constant_pool_get (J2cConstantPool *self, const guint16 index, GError **error);
gchar *j2c_constant_pool_get_string (J2cConstantPool *self, const guint16 index, GError **error);

G_END_DECLS

#endif /* __CONSTANT_POOL_H___ */
