#include <j2c/constant-pool-item.h>

static GEnumValue constant_pool_item_tag_values[] =
{
  {  7, "CONSTANT_class",              "Class"              },
  {  9, "CONSTANT_Fieldref",           "Fieldref"           },
  { 10, "CONSTANT_Methodref",          "Methodref"          },
  { 11, "CONSTANT_InterfaceMethodref", "InterfaceMethodref" },
  {  8, "CONSTANT_String",             "String"             },
  {  3, "CONSTANT_Integer",            "Integer"            },
  {  4, "CONSTANT_Float",              "Float"              },
  {  5, "CONSTANT_Long",               "Long"               },
  {  6, "CONSTANT_Double",             "Double"             },
  { 12, "CONSTANT_NameAndType",        "NameAndType"        },
  {  1, "CONSTANT_Utf8",               "Utf8"               },
  { 15, "CONSTANT_MethodHandle",       "MethodHandle"       },
  { 16, "CONSTANT_MethodType",         "MethodType"         },
  { 18, "CONSTANT_InvokeDynamic",      "InvokeDynamic"      },
  0
};

struct _J2cClassInfo
{
  J2cConstantPoolItem parent;

  guint16 name_index;
};

struct _J2cFieldrefInfo
{
  J2cConstantPoolItem parent;

  guint16 class_index;
  guint16 name_and_type_index;
};

struct _J2cMethodrefInfo
{
  J2cConstantPoolItem parent;

  guint16 class_index;
  guint16 name_and_type_index;
};

struct _J2cInterfaceMethodrefInfo
{
  J2cConstantPoolItem parent;

  guint16 class_index;
  guint16 name_and_type_index;
};

struct _J2cStringInfo
{
  J2cConstantPoolItem parent;

  guint16 string_index;
};

struct _J2cIntegerInfo
{
  J2cConstantPoolItem parent;

  gint32 value;
};

struct _J2cFloatInfo
{
  J2cConstantPoolItem parent;

  gfloat value;
};

struct _J2cLongInfo
{
  J2cConstantPoolItem parent;

  gint64 value;
};

struct _J2cDoubleInfo
{
  J2cConstantPoolItem parent;

  gdouble value;
};

struct _J2cNameAndTypeInfo
{
  J2cConstantPoolItem parent;

  guint16 name_index;
  guint16 descriptor_index;
};

struct _J2cUtf8Info
{
  J2cConstantPoolItem parent;

  gchar *value;
};

struct _J2cMethodHandleInfo
{
  J2cConstantPoolItem parent;

  guint8 reference_kind;
  guint16 reference_index;
};

struct _J2cMethodTypeInfo
{
  J2cConstantPoolItem parent;

  guint16 descriptor_index;
};

struct _J2cInvokeDynamicInfo
{
  J2cConstantPoolItem parent;

  guint16 bootstrap_method_attr_index;
  guint16 name_and_type_index;
};

G_DEFINE_TYPE (J2cConstantPoolItem, j2c_constant_pool_item, G_TYPE_OBJECT)
G_DEFINE_TYPE (J2cClassInfo, j2c_class_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cFieldrefInfo, j2c_fieldref_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cMethodrefInfo, j2c_methodref_info, J2C_TYPE_FIELDREF_INFO)
G_DEFINE_TYPE (J2cInterfaceMethodrefInfo, j2c_interface_methodref_info, J2C_TYPE_FIELDREF_INFO)
G_DEFINE_TYPE (J2cStringInfo, j2c_string_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cIntegerInfo, j2c_integer_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cFloatInfo, j2c_float_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cLongInfo, j2c_long_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cDoubleInfo, j2c_double_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cNameAndTypeInfo, j2c_name_and_type_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cUtf8Info, j2c_utf8_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cMethodHandleInfo, j2c_method_handle_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cMethodTypeInfo, j2c_method_type_info, J2C_TYPE_CONSTANT_POOL_ITEM)
G_DEFINE_TYPE (J2cInvokeDynamicInfo, j2c_invoke_dynamic_info, J2C_TYPE_CONSTANT_POOL_ITEM)

static void j2c_utf8_info_dispose (GObject *object);

static J2cClassInfo *j2c_class_info_new (guint8 *bytes);
static J2cFieldrefInfo *j2c_fieldref_info_new (guint8 *bytes);
static J2cMethodrefInfo *j2c_methodref_info_new (guint8 *bytes);
static J2cInterfaceMethodrefInfo *j2c_interface_methodref_info_new (guint8 *bytes);
static J2cStringInfo *j2c_string_info_new (guint8 *bytes);
static J2cIntegerInfo *j2c_integer_info_new (guint8 *bytes);
static J2cFloatInfo *j2c_float_info_new (guint8 *bytes);
static J2cLongInfo *j2c_long_info_new (guint8 *bytes);
static J2cDoubleInfo *j2c_double_info_new (guint8 *bytes);
static J2cNameAndTypeInfo *j2c_name_and_type_info_new (guint8 *bytes);
static J2cUtf8Info *j2c_utf8_info_new (guint8 *bytes);
static J2cMethodHandleInfo *j2c_method_handle_info_new (guint8 *bytes);
static J2cMethodTypeInfo *j2c_method_type_info_new (guint8 *bytes);
static J2cInvokeDynamicInfo *j2c_invoke_dynamic_info_new (guint8 *bytes);

GType
j2c_constant_pool_item_tag_get_type (void)
{
  static GType type = 0;
  if (type == 0)
    type = g_enum_register_static ("J2cConstantPoolItemTag", constant_pool_item_tag_values);
  return type;
}

gint
j2c_constant_pool_item_size (J2cConstantPoolItemTag const tag)
{
  switch (tag)
    {
    case CONSTANT_Class:              return  2;
    case CONSTANT_Fieldref:           return  4;
    case CONSTANT_Methodref:          return  4;
    case CONSTANT_InterfaceMethodref: return  4;
    case CONSTANT_String:             return  2;
    case CONSTANT_Integer:            return  4;
    case CONSTANT_Float:              return  4;
    case CONSTANT_Long:               return  8;
    case CONSTANT_Double:             return  8;
    case CONSTANT_NameAndType:        return  4;
    case CONSTANT_Utf8:               return -1;
    case CONSTANT_MethodHandle:       return  4;
    case CONSTANT_MethodType:         return  2;
    case CONSTANT_InvokeDynamic:      return  4;
    default:                          return -1;
    }
}

gpointer
j2c_constant_pool_item_new (guint8 *bytes)
{
  switch (bytes[0])
    {
    case CONSTANT_Class:              return j2c_class_info_new (bytes + 1);
    case CONSTANT_Fieldref:           return j2c_fieldref_info_new (bytes + 1);
    case CONSTANT_Methodref:          return j2c_methodref_info_new (bytes + 1);
    case CONSTANT_InterfaceMethodref: return j2c_interface_methodref_info_new (bytes + 1);
    case CONSTANT_String:             return j2c_string_info_new (bytes + 1);
    case CONSTANT_Integer:            return j2c_integer_info_new (bytes + 1);
    case CONSTANT_Float:              return j2c_float_info_new (bytes + 1);
    case CONSTANT_Long:               return j2c_long_info_new (bytes + 1);
    case CONSTANT_Double:             return j2c_double_info_new (bytes + 1);
    case CONSTANT_NameAndType:        return j2c_name_and_type_info_new (bytes + 1);
    case CONSTANT_Utf8:               return j2c_utf8_info_new (bytes + 1);
    case CONSTANT_MethodHandle:       return j2c_method_handle_info_new (bytes + 1);
    case CONSTANT_MethodType:         return j2c_method_type_info_new (bytes + 1);
    case CONSTANT_InvokeDynamic:      return j2c_invoke_dynamic (bytes + 1);
    default:                          NULL;
    }
}

static void
j2c_constant_pool_item_class_init (J2cConstantPoolItemClass *klass)
{
  return;
}

static void
j2c_constant_pool_item_init (J2cConstantPoolItem *self)
{
  return;
}

static void
j2c_class_info_class_init (J2cClassInfoClass *klass)
{
  return;
}

static void
j2c_class_info_init (J2cClassInfo *self)
{
  return;
}

static void
j2c_fieldref_info_class_init (J2cFieldrefInfoClass *klass)
{
  return;
}

static void
j2c_fieldref_info_init (J2cFieldrefInfo *self)
{
  return;
}

static void
j2c_methodref_info_class_init (J2cMethodrefInfoClass *klass)
{
  return;
}

static void
j2c_methodref_info_init (J2cMethodrefInfo *self)
{
  return;
}

static void
j2c_interface_methodref_info_class_init (J2cInterfaceMethodrefInfoClass *klass)
{
  return;
}

static void
j2c_interface_methodref_info_init (J2cInterfaceMethodrefInfo *self)
{
  return;
}

static void
j2c_string_info_class_init (J2cStringInfoClass *klass)
{
  return;
}

static void
j2c_string_info_init (J2cStringInfo *self)
{
  return;
}

static void
j2c_integer_info_class_init (J2cIntegerInfoClass *klass)
{
  return;
}

static void
j2c_integer_info_init (J2cIntegerInfo *self)
{
  return;
}

static void
j2c_float_info_class_init (J2cFloatInfoClass *klass)
{
  return;
}

static void
j2c_float_info_init (J2cFloatInfo *self)
{
  return;
}

static void
j2c_long_info_class_init (J2cLongInfoClass *klass)
{
  return;
}

static void
j2c_long_info_init (J2cLongInfo *self)
{
  return;
}

static void
j2c_double_info_class_init (J2cDoubleInfoClass *klass)
{
  return;
}

static void
j2c_double_info_init (J2cDoubleInfo *self)
{
  return;
}

static void
j2c_name_and_type_info_class_init (J2cNameAndTypeInfoClass *klass)
{
  return;
}

static void
j2c_name_and_type_info_init (J2cNameAndTypeInfo *self)
{
  return;
}

static void
j2c_utf8_info_class_init (J2cUtf8InfoClass *klass)
{
  G_OBJECT_CLASS (klass)->dispose = j2c_utf8_info_dispose;
}

static void
j2c_utf8_info_init (J2cUtf8Info *self)
{
  return;
}

static void
j2c_method_handle_info_class_init (J2cMethodHandleInfoClass *klass)
{
  return;
}

static void
j2c_method_handle_info_init (J2cMethodHandleInfo *self)
{
  return;
}

static void
j2c_method_type_info_class_init (J2cMethodTypeInfoClass *klass)
{
  return;
}

static void
j2c_method_type_info_init (J2cMethodTypeInfo *self)
{
  return;
}

static void
j2c_invoke_dynamic_info_class_init (J2cInvokeDynamicInfoClass *klass)
{
  return;
}

static void
j2c_invoke_dynamic_info_init (J2cInvokeDynamicInfo *self)
{
  return;
}

static void
j2c_utf8_info_dispose (GObject *object)
{
  J2cUtf8Info *self = J2C_UTF8_INFO (object);

  if (self->value)
    {
      g_free (self->value);
      self->value = NULL;
    }

  G_OBJECT_CLASS (j2c_utf8_info_parent_class)->dispose (object);
}

static J2cClassInfo *
j2c_class_info_new (guint8 *bytes)
{
  J2cClassInfo *ret = g_object_new (J2C_TYPE_CLASS_INFO, NULL);
  ret->name_index = (bytes[0] << 8) || bytes[1];
}

static J2cFieldrefInfo *
j2c_fieldref_info_new (guint8 *bytes)
{
  J2cFieldrefInfo *ret = g_object_new (J2C_TYPE_FIELDREF_INFO, NULL);
  ret->class_index = (bytes[0] << 8) || bytes[1];
  ret->name_and_type_index = (bytes[2] << 8) || bytes[3];
}

static J2cMethodrefInfo *
j2c_methodref_info_new (guint8 *bytes)
{
  J2cMethodrefInfo *ret = g_object_new (J2C_TYPE_METHODREF_INFO, NULL);
  ret->class_index = (bytes[0] << 8) || bytes[1];
  ret->name_and_type_index = (bytes[2] << 8) || bytes[3];
}

static J2cInterfaceMethodrefInfo *
j2c_interface_methodref_info_new (guint8 *bytes)
{
  J2cInterfaceMethodrefInfo *ret = g_object_new (J2C_TYPE_INTERFACE_METHODREF_INFO, NULL);
  ret->class_index = (bytes[0] << 8) || bytes[1];
  ret->name_and_type_index = (bytes[2] << 8) || bytes[3];
}

static J2cStringInfo *j2c_string_info_new (guint8 *bytes);
static J2cIntegerInfo *j2c_integer_info_new (guint8 *bytes);
static J2cFloatInfo *j2c_float_info_new (guint8 *bytes);
static J2cLongInfo *j2c_long_info_new (guint8 *bytes);
static J2cDoubleInfo *j2c_double_info_new (guint8 *bytes);
static J2cNameAndTypeInfo *j2c_name_and_type_info_new (guint8 *bytes);
static J2cUtf8Info *j2c_utf8_info_new (guint8 *bytes);
static J2cMethodHandleInfo *j2c_method_handle_info_new (guint8 *bytes);
static J2cMethodTypeInfo *j2c_method_type_info_new (guint8 *bytes);
static J2cInvokeDynamicInfo *j2c_invoke_dynamic_info_new (guint8 *bytes);
