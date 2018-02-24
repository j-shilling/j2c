#include <j2c/constant-pool-item.h>
#include <j2c/modified-utf8.h>

#include <math.h>

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

static J2cClassInfo *j2c_class_info_new (guint16 name_index);
static J2cFieldrefInfo *j2c_fieldref_info_new (guint16 class_index, guint16 name_and_type_index);
static J2cMethodrefInfo *j2c_methodref_info_new (guint16 class_index, guint16 name_and_type_index);
static J2cInterfaceMethodrefInfo *j2c_interface_methodref_info_new (guint16 class_index, guint16 name_and_type_index);
static J2cStringInfo *j2c_string_info_new (guint16 string_index);
static J2cIntegerInfo *j2c_integer_info_new (gint32 value);
static J2cFloatInfo *j2c_float_info_new (guint32 ival, GError **error);
static J2cLongInfo *j2c_long_info_new (guint32 high_bytes, guint32 low_bytes);
static J2cDoubleInfo *j2c_double_info_new (guint32 high_bytes, guint32 low_bytes, GError **error);
static J2cNameAndTypeInfo *j2c_name_and_type_info_new (guint16 name_index, guint16 descriptor_index);
static J2cUtf8Info *j2c_utf8_info_new (guint16 len, guint8 *bytes, GError **error);
static J2cMethodHandleInfo *j2c_method_handle_info_new (guint8 reference_kind, guint16 reference_index);
static J2cMethodTypeInfo *j2c_method_type_info_new (guint16 descriptor_index);
static J2cInvokeDynamicInfo *j2c_invoke_dynamic_info_new (guint16 bootstrap_method_attr_index, guint16 name_and_type_index);

GType
j2c_constant_pool_item_tag_get_type (void)
{
  static GType type = 0;
  if (type == 0)
    type = g_enum_register_static ("J2cConstantPoolItemTag", constant_pool_item_tag_values);
  return type;
}

J2cConstantPoolItem *
j2c_constant_pool_item_new (GDataInputStream *in, GError **error)
{
  g_return_val_if_fail (in != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  GError *tmp_error = NULL;
  gpointer ret;

  guint8 tag = g_data_input_stream_read_byte (in, NULL, &tmp_error);
  if (tmp_error)
    {
      g_propagate_error (error, tmp_error);
      return NULL;
    }

  switch (tag)
    {
      guint16 name_index, class_index, name_and_type_index,
	      string_index, descriptor_index, len, reference_index, bootstrap_method_attr_index;
      gint32 value;
      guint32 ival, high_bytes, low_bytes;
      guint8 reference_kind, *bytes;

    case CONSTANT_Class:
	{
	  name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_class_info_new (name_index);
	}
    case CONSTANT_Fieldref:
	{
	  class_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    name_and_type_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_fieldref_info_new (class_index, name_and_type_index);
	}
    case CONSTANT_Methodref:
	{
	  class_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    name_and_type_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_methodref_info_new (class_index, name_and_type_index);
	}
    case CONSTANT_InterfaceMethodref:
	{
	  class_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    name_and_type_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_interface_methodref_info_new (class_index, name_and_type_index);
	}
    case CONSTANT_String:
	{
	  string_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_string_info_new (string_index);
	}
    case CONSTANT_Integer:
	{
	  value = g_data_input_stream_read_int32 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_integer_info_new (value);
	}
    case CONSTANT_Float:
	{
	  ival = g_data_input_stream_read_uint32 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_float_info_new (ival, &tmp_error);
	}
    case CONSTANT_Long:
	{
	  high_bytes = g_data_input_stream_read_uint32 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    low_bytes = g_data_input_stream_read_uint32 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_long_info_new (high_bytes, low_bytes);
	}
    case CONSTANT_Double:
        {
	  high_bytes = g_data_input_stream_read_uint32 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    low_bytes = g_data_input_stream_read_uint32 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_double_info_new (high_bytes, low_bytes, &tmp_error);
	}
    case CONSTANT_NameAndType:
	{
	  name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    descriptor_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_name_and_type_info_new (name_index, descriptor_index);
	}
    case CONSTANT_Utf8:
	{
	  len = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    {
	      bytes = g_malloc (len);
	      g_input_stream_read (G_INPUT_STREAM (in),
				   bytes,
				   len,
				   NULL,
				   &tmp_error);
	      if (!tmp_error)
		ret = j2c_utf8_info_new (len, bytes, &tmp_error);

	      g_free (bytes);
	    }
	}
    case CONSTANT_MethodHandle:
	{
	  reference_kind = g_data_input_stream_read_byte (in, NULL, &tmp_error);
	  if (!tmp_error)
	    reference_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_method_handle_info_new (reference_kind, reference_index);
	}
    case CONSTANT_MethodType:
	{
          descriptor_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_method_type_info_new (descriptor_index);
	}
    case CONSTANT_InvokeDynamic:
	{
	  bootstrap_method_attr_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    name_and_type_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
	  if (!tmp_error)
	    ret = j2c_invoke_dynamic_info_new (bootstrap_method_attr_index, name_and_type_index);
	}
    default:
	{
          g_set_error (error,
	               J2C_CONSTANT_POOL_ITEM_ERROR,
	               J2C_CONSTANT_POOL_INVALID_TAG_ERROR,
	               "%u is not a valid constant pool tag",
	               bytes[0]);
	  return NULL;
	}
    }

  if (tmp_error)
    {
      g_propagate_error (error, tmp_error);
      return NULL;
    }

  return J2C_CONSTANT_POOL_ITEM (ret);
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
j2c_class_info_new (guint16 name_index)
{
  J2cClassInfo *ret = g_object_new (J2C_TYPE_CLASS_INFO, NULL);
  ret->name_index = name_index;
  return ret;
}

static J2cFieldrefInfo *
j2c_fieldref_info_new (guint16 class_index, guint16 name_and_type_index)
{
  J2cFieldrefInfo *ret = g_object_new (J2C_TYPE_FIELDREF_INFO, NULL);
  ret->class_index = class_index;
  ret->name_and_type_index = name_and_type_index;
  return ret;
}

static J2cMethodrefInfo *
j2c_methodref_info_new (guint16 class_index, guint16 name_and_type_index)
{
  J2cMethodrefInfo *ret = g_object_new (J2C_TYPE_METHODREF_INFO, NULL);
  ret->class_index = class_index;
  ret->name_and_type_index = name_and_type_index;
  return ret;
}

static J2cInterfaceMethodrefInfo *
j2c_interface_methodref_info_new (guint16 class_index, guint16 name_and_type_index)
{
  J2cInterfaceMethodrefInfo *ret = g_object_new (J2C_TYPE_INTERFACE_METHODREF_INFO, NULL);
  ret->class_index = class_index;
  ret->name_and_type_index = name_and_type_index;
  return ret;
}

static J2cStringInfo *
j2c_string_info_new (guint16 string_index)
{
  J2cStringInfo *ret = g_object_new (J2C_TYPE_STRING_INFO, NULL);
  ret->string_index = string_index;
  return ret;
}

static J2cIntegerInfo *
j2c_integer_info_new (gint32 value)
{
  J2cIntegerInfo *ret = g_object_new (J2C_TYPE_INTEGER_INFO, NULL);
  ret->value = value;
  return ret;
}

static J2cFloatInfo *
j2c_float_info_new (guint32 ival, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  gfloat fval;
  if (ival == 0x7f800000)
    {
#ifdef INFINITY
      fval = INFINITY;
#else
      g_set_error (error,
		   J2C_CONSTANT_POOL_ITEM_ERROR,
		   J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
		   "This platform does not support INFINITY fval.");
      return NULL;
#endif
    }
  else if (ival == 0xff800000)
    {
#ifdef INFINITY
      fval = -INFINITY;
#else
      g_set_error (error,
		   J2C_CONSTANT_POOL_ITEM_ERROR,
		   J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
		   "This platform does not support INFINITY fval.");
      return NULL;
#endif
    }
  else if ((ival >= 0x7f800001 && ival <= 0x7fffffff) || (ival >= 0xff800001 && ival <= 0xffffffff))
    {
#ifdef NAN
      fval = NAN;
#else
      g_set_error (error,
		   J2C_CONSTANT_POOL_ITEM_ERROR,
		   J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
		   "This platform does not support NAN fval.");
      return NULL;
#endif
    }
  else
    {
      gint s = ((ival >> 31) == 0) ? 1 : -1;
      gint e = ((ival >> 23) & 0xff);
      gint m = (e == 0) ?
	       (ival & 0x7fffff) << 1 :
	       (ival & 0x7fffff) | 0x800000;

      fval = s * m * powf (2.0, ((gfloat) e) - 150.0);
    }

  J2cFloatInfo *ret = g_object_new (J2C_TYPE_FLOAT_INFO, NULL);
  ret->value = fval;
  return ret;
}

static J2cLongInfo *
j2c_long_info_new (guint32 high_bytes, guint32 low_bytes)
{
  J2cLongInfo *ret = g_object_new (J2C_TYPE_LONG_INFO, NULL);
  ret->value = ((gint64) high_bytes << 32) | low_bytes;
  return ret;
}

static J2cDoubleInfo *
j2c_double_info_new (guint32 high_bytes, guint32 low_bytes, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  gint64 bits = ((gint64) high_bytes << 32) + low_bytes;
  gdouble value;
  if (bits == 0x7ff0000000000000L)
    {
#ifdef INFINITY
      value = INFINITY;
#else
      g_set_error (error,
		   J2C_CONSTANT_POOL_ITEM_ERROR,
		   J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
		   "This platform does not support INFINITY value.");
      return NULL;
#endif
    }
  else if (bits == 0xfff0000000000000L)
    {
#ifdef INFINITY
      value = -INFINITY;
#else
      g_set_error (error,
		   J2C_CONSTANT_POOL_ITEM_ERROR,
		   J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
		   "This platform does not support INFINITY value.");
      return NULL;
#endif
    }
  else if ((bits >= 0x7ff0000000000001L && bits <= 0x7fffffffffffffffL) || (bits >= 0xfff0000000000001L && bits <= 0xffffffffffffffffL))
    {
#ifdef NAN
      value = NAN;
#else
      g_set_error (error,
		   J2C_CONSTANT_POOL_ITEM_ERROR,
		   J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
		   "This platform does not support NAN value.");
      return NULL;
#endif
    }
  else
    {
      gint s = ((bits >> 63) == 0) ? 1 : -1;
      gint e = (gint)((bits >> 52) & 0x7ffL);
      gint m = (e == 0) ?
	       (bits & 0xfffffffffffffL) << 1 :
	       (bits & 0xfffffffffffffL) | 0x10000000000000L;

      value = s * m * pow (2.0, ((gdouble) e) - 1075.0);
    }
  J2cDoubleInfo *ret = g_object_new (J2C_TYPE_DOUBLE_INFO, NULL);
  ret->value = value;
  return ret;
}

static J2cNameAndTypeInfo *
j2c_name_and_type_info_new (guint16 name_index, guint16 descriptor_index)
{
  J2cNameAndTypeInfo *ret = g_object_new (J2C_TYPE_NAME_AND_TYPE_INFO, NULL);
  ret->name_index = name_index;
  ret->descriptor_index = descriptor_index;
  return ret;
}

static J2cUtf8Info *
j2c_utf8_info_new (guint16 len, guint8 *bytes, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  GError *tmp_error = NULL;

  gchar *str;

  if (len == 0)
    {
      str = g_strdup ("");
    }
  else
    {
      str = j2c_modified_utf8_to_standard (bytes, len, &tmp_error);
      if (tmp_error)
	{
	  g_propagate_error (error, tmp_error);
	  return NULL;
	}
    }

  J2cUtf8Info *ret = g_object_new (J2C_TYPE_UTF8_INFO, NULL);
  ret->value = str;
  return ret;
}

static J2cMethodHandleInfo *
j2c_method_handle_info_new (guint8 reference_kind, guint16 reference_index)
{
  J2cMethodHandleInfo *ret = g_object_new (J2C_TYPE_METHOD_HANDLE_INFO, NULL);
  ret->reference_kind = reference_kind;
  ret->reference_index = reference_index;
  return ret;
}

static J2cMethodTypeInfo *
j2c_method_type_info_new (guint16 descriptor_index)
{
  J2cMethodTypeInfo *ret = g_object_new (J2C_TYPE_METHOD_TYPE_INFO, NULL);
  ret->descriptor_index = descriptor_index;
  return ret;
}

static J2cInvokeDynamicInfo *
j2c_invoke_dynamic_info_new (guint16 bootstrap_method_attr_index, guint16 name_and_type_index)
{
  J2cInvokeDynamicInfo *ret = g_object_new (J2C_TYPE_INVOKE_DYNAMIC_INFO, NULL);
  ret->bootstrap_method_attr_index = bootstrap_method_attr_index;
  ret->name_and_type_index = name_and_type_index;
  return ret;
}

guint16
j2c_class_info_name_index (J2cClassInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->name_index;
}

guint16
j2c_fieldref_info_class_index (J2cFieldrefInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->class_index;
}

guint16
j2c_fieldref_info_name_and_type_index (J2cFieldrefInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->name_and_type_index;
}

guint16
j2c_methodref_info_class_index (J2cMethodrefInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->class_index;
}

guint16
j2c_methodref_info_name_and_type_index (J2cMethodrefInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->name_and_type_index;
}

guint16
j2c_interface_methodref_info_class_index (J2cInterfaceMethodrefInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->class_index;
}

guint16
j2c_interface_methodref_info_name_and_type_index (J2cInterfaceMethodrefInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->name_and_type_index;
}

guint16
j2c_string_info_string_index (J2cStringInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->string_index;
}

gint32
j2c_integer_info_value (J2cIntegerInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->value;
}

gfloat
j2c_float_info_value (J2cFloatInfo *self)
{
  g_return_val_if_fail (self, 0.0);
  return self->value;
}

gint64
j2c_long_info_value (J2cLongInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->value;
}

gdouble
j2c_double_info_value (J2cDoubleInfo *self)
{
  g_return_val_if_fail (self, 0.0);
  return self->value;
}

guint16
j2c_name_and_type_info_name_index (J2cNameAndTypeInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->name_index;
}

guint16
j2c_name_and_type_info_descriptor_index (J2cNameAndTypeInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->descriptor_index;
}

const gchar *
j2c_utf8_info_string (J2cUtf8Info *self)
{
  g_return_val_if_fail (self, NULL);
  return self->value;
}

guint8
j2c_method_handle_info_reference_kind (J2cMethodHandleInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->reference_kind;
}

guint16
j2c_method_handle_info_reference_index (J2cMethodHandleInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->reference_index;
}

guint16
j2c_method_type_info_descriptor_index (J2cMethodTypeInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->descriptor_index;
}

guint16
j2c_invoke_dynamic_info_bootstrap_method_attr_index (J2cInvokeDynamicInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->bootstrap_method_attr_index;
}

guint16
j2c_invoke_dynamic_info_name_and_type_idnex (J2cInvokeDynamicInfo *self)
{
  g_return_val_if_fail (self, 0);
  return self->name_and_type_index;
}
