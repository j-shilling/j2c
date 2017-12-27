#include <j2c/constant-pool.h>

#include <math.h>

/****
  DERIVABLE PRIV STRUCTURES
 ****/

typedef struct
{
  guint8 tag;
} J2cConstantPoolItemPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (J2cConstantPoolItem, j2c_constant_pool_item, G_TYPE_OBJECT)

typedef struct
{
  guint16 class_index;
  guint16 name_and_type_index;
} J2cFieldrefInfoPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (J2cFieldrefInfo, j2c_fieldref_info, J2C_TYPE_CONSTANT_POOL_ITEM)

/****
  FINAL TYPE DEFINITIONS
 ****/

struct _J2cConstantPool
{
  GObject parent;

  guint16 count;
  GPtrArray *pool;
};

struct _J2cClassInfo
{
  J2cConstantPoolItem parent;

  guint16 name_index;
};

struct _J2cMethodrefInfo
{
  J2cFieldrefInfo parent;
};

struct _J2cInterfaceMethodrefInfo
{
  J2cFieldrefInfo parent;
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
  gchar *bytes;
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


/****
  DEFINE TYPES
 ****/


G_DEFINE_TYPE (J2cConstantPool, j2c_constant_pool, G_TYPE_OBJECT)
G_DEFINE_TYPE (J2cClassInfo, j2c_class_info, J2C_TYPE_CONSTANT_POOL_ITEM)
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

/****
  PROPERTY DEFINITIONS
 ****/

enum
{
  PROP_COUNT = 1,
  PROP_POOL,
  PROP_TAG,
  PROP_NAME_INDEX,
  PROP_CLASS_INDEX,
  PROP_NAME_AND_TYPE_INDEX,
  PROP_STRING_INDEX,
  PROP_VALUE,
  PROP_DESCRIPTOR_INDEX,
  PROP_LENGTH,
  PROP_BYTES,
  PROP_REFERENCE_KIND,
  PROP_REFERENCE_INDEX,
  PROP_BOOTSTRAP_METHOD_ATTR_INDEX
};

/****
  Private Virtual Method Definitions
 ****/

static void j2c_constant_pool_dispose (GObject *object);
static void j2c_utf8_info_dispose (GObject *object);

static void j2c_constant_pool_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_constant_pool_item_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_class_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_fieldref_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_string_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_integer_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_float_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_long_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_double_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_name_and_type_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_utf8_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_method_handle_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_method_type_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_invoke_dynamic_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void j2c_constant_pool_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_constant_pool_item_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_class_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_fieldref_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_string_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_integer_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_float_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_long_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_double_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_name_and_type_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_utf8_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_method_handle_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_method_type_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_invoke_dynamic_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);


static void j2c_constant_pool_ptr_array_free (gpointer data);
static void j2c_constant_pool_ptr_array_add (GPtrArray *arr, J2cConstantPoolItem *item);

/****
  CLASS INIT FUNCTIONS
 ****/

static void
j2c_constant_pool_class_init (J2cConstantPoolClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_constant_pool_set_property;
  object_class->get_property = j2c_constant_pool_get_property;
  object_class->dispose = j2c_constant_pool_dispose;

  g_object_class_install_property (object_class, PROP_COUNT,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_COUNT,
							"count",
							"The value of the constant_pool_count "
							"item is equal to the number of entries "
							"in the constant_pool table plus one. "
							"A constant_pool index is considered "
							"valid if it is greater than zero and "
							"less than constant_pool_count, with "
							"the exception for constants of type "
							"long and double noted in §4.4.5.",
							0, G_MAXUINT16,
							1,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_POOL,
				   g_param_spec_boxed (J2C_CONSTANT_POOL_PROP_POOL,
							"pool",
							"The array of constant pool entries.",
							G_TYPE_PTR_ARRAY,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  
}

static void
j2c_constant_pool_item_class_init (J2cConstantPoolItemClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_constant_pool_item_set_property;
  object_class->get_property = j2c_constant_pool_item_get_property;

  g_object_class_install_property (object_class, PROP_TAG,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_TAG,
							"tag",
							"A 1-byte tag identifies the type of "
							"each constant pool entry.",
							0, G_MAXUINT8,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_class_info_class_init (J2cClassInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_class_info_set_property;
  object_class->get_property = j2c_class_info_get_property;

  g_object_class_install_property (object_class, PROP_NAME_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_NAME_INDEX,
							"name_index",
							"The value of the name_index item must "
							"be a valid index into the constant_pool "
							"table. The constant_pool entry at that "
							"index must be a CONSTANT_Utf8_info "
							"structure (§4.4.7) representing a valid "
							"binary class or interface name encoded "
							"in internal form (§4.2.1).",
							0, G_MAXUINT16,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_fieldref_info_class_init (J2cFieldrefInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_fieldref_info_set_property;
  object_class->get_property = j2c_fieldref_info_get_property;

  g_object_class_install_property (object_class, PROP_CLASS_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_CLASS_INDEX,
							"class_index",
							"The value of the class_index item "
							"must be a valid index into the "
							"constant_pool table. The constant_pool "
							"entry at that index must be a "
							"CONSTANT_Class_info structure (§4.4.1) "
							"representing a class or interface type "
							"that has the field or method as a member.",
							0, G_MAXUINT16,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_NAME_AND_TYPE_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_NAME_AND_TYPE_INDEX,
							"name_and_type_index",
							"The value of the name_and_type_index item "
							"must be a valid index into the constant_pool "
							"table. The constant_pool entry at that index "
							"must be a CONSTANT_NameAndType_info structure "
							"(§4.4.6). This constant_pool entry indicates "
							"the name and descriptor of the field or method.",
							0, G_MAXUINT16,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_methodref_info_class_init (J2cMethodrefInfoClass *klass)
{
  return;
}

static void
j2c_interface_methodref_info_class_init (J2cInterfaceMethodrefInfoClass *klass)
{
  return;
}

static void
j2c_string_info_class_init (J2cStringInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_string_info_set_property;
  object_class->get_property = j2c_string_info_get_property;

  g_object_class_install_property (object_class, PROP_STRING_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_STRING_INDEX,
							"string_index",
							"The value of the string_index item "
							"must be a valid index into the "
							"constant_pool table. The constant_pool "
							"entry at that index must be a "
							"CONSTANT_Utf8_info structure (§4.4.7) "
							"representing the sequence of Unicode "
							"code points to which the String object "
							"is to be initialized.",
							0, G_MAXUINT16,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_integer_info_class_init (J2cIntegerInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_integer_info_set_property;
  object_class->get_property = j2c_integer_info_get_property;

  g_object_class_install_property (object_class, PROP_VALUE,
				   g_param_spec_int64 (J2C_CONSTANT_POOL_PROP_VALUE,
						       "value",
						       "The value of a data type in the constant pool.",
						       G_MININT32, G_MAXINT32,
						       0,
						       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_float_info_class_init (J2cFloatInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_float_info_set_property;
  object_class->get_property = j2c_float_info_get_property;

  g_object_class_install_property (object_class, PROP_VALUE,
				   g_param_spec_float (J2C_CONSTANT_POOL_PROP_VALUE,
						       "value",
						       "The value of a data type in the constant pool.",
						       -G_MAXFLOAT, G_MAXFLOAT,
						       0.0,
						       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_long_info_class_init (J2cLongInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_long_info_set_property;
  object_class->get_property = j2c_long_info_get_property;

  g_object_class_install_property (object_class, PROP_VALUE,
				   g_param_spec_int64 (J2C_CONSTANT_POOL_PROP_VALUE,
						       "value",
						       "The value of a data type in the constant pool.",
						       G_MININT64, G_MAXINT64,
						       0,
						       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_double_info_class_init (J2cDoubleInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_double_info_set_property;
  object_class->get_property = j2c_double_info_get_property;

  g_object_class_install_property (object_class, PROP_VALUE,
				   g_param_spec_double (J2C_CONSTANT_POOL_PROP_VALUE,
							"value",
						        "The value of a data type in the constant pool.",
							-G_MAXDOUBLE, G_MAXDOUBLE,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_name_and_type_info_class_init (J2cNameAndTypeInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_name_and_type_info_set_property;
  object_class->get_property = j2c_name_and_type_info_get_property;

  g_object_class_install_property (object_class, PROP_NAME_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_NAME_INDEX,
							"name_index",
							"The value of the name_index item must "
							"be a valid index into the constant_pool "
							"table. The constant_pool entry at that "
							"index must be a CONSTANT_Utf8_info "
							"structure (§4.4.7) representing "
							"either the special method name <init> (§2.9) "
							"or a valid unqualified name denoting a "
							"field or method (§4.2.2).",
							0, G_MAXUINT16,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_DESCRIPTOR_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_DESCRIPTOR_INDEX,
							"descriptor_index",
							"The value of the descriptor_index item must "
							"be a valid index into the constant_pool "
							"table. The constant_pool entry at that index "
							"must be a CONSTANT_Utf8_info structure "
							"(§4.4.7) representing a valid field descriptor "
							"or method descriptor (§4.3.2, §4.3.3).",
							0, G_MAXUINT16,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_utf8_info_class_init (J2cUtf8InfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_utf8_info_set_property;
  object_class->get_property = j2c_utf8_info_get_property;
  object_class->dispose = j2c_utf8_info_dispose;

  g_object_class_install_property (object_class, PROP_BYTES,
				   g_param_spec_string (J2C_CONSTANT_POOL_PROP_BYTES,
							"bytes",
							"bytes of the utf8 string.",
							"",
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_method_handle_info_class_init (J2cMethodHandleInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_method_handle_info_set_property;
  object_class->get_property = j2c_method_handle_info_get_property;

  g_object_class_install_property (object_class, PROP_REFERENCE_KIND,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_REFERENCE_KIND,
							"reference_kind",
							"The value of the reference_kind item "
							"must be in the range 1 to 9. The value "
							"denotes the kind of this method handle, "
							"which characterizes its bytecode "
							"behavior (§5.4.3.5).",
							0, G_MAXUINT8,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_REFERENCE_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_REFERENCE_INDEX,
							"reference_index",
							"The value of the reference_index item must "
							"be a valid index into the constant_pool table.",
							0, G_MAXUINT16,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_method_type_info_class_init (J2cMethodTypeInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_method_type_info_set_property;
  object_class->get_property = j2c_method_type_info_get_property;

  g_object_class_install_property (object_class, PROP_DESCRIPTOR_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_DESCRIPTOR_INDEX,
							"descriptor_index",
							"The value of the descriptor_index item must "
							"be a valid index into the constant_pool "
							"table. The constant_pool entry at that index "
							"must be a CONSTANT_Utf8_info structure "
							"(§4.4.7) representing a valid method descriptor.",
							0, G_MAXUINT16,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

}

static void
j2c_invoke_dynamic_info_class_init (J2cInvokeDynamicInfoClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_invoke_dynamic_info_set_property;
  object_class->get_property = j2c_invoke_dynamic_info_get_property;

  g_object_class_install_property (object_class, PROP_BOOTSTRAP_METHOD_ATTR_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_BOOTSTRAP_METHOD_ATTR_INDEX,
							"bootstrap_method_attr_index",
							"The value of the bootstrap_method_attr_index "
							"item must be a valid index into the "
							"bootstrap_methods array of the bootstrap "
							"method table (§4.7.23) of this class file.",
							0, G_MAXUINT16,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  
  g_object_class_install_property (object_class, PROP_NAME_AND_TYPE_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_NAME_AND_TYPE_INDEX,
							"name_and_type_index",
							"The value of the name_and_type_index item "
							"must be a valid index into the constant_pool "
							"table. The constant_pool entry at that index "
							"must be a CONSTANT_NameAndType_info structure "
							"(§4.4.6). This constant_pool entry indicates "
							"the name and descriptor of the method.",
							0, G_MAXUINT16,
							0,
							G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

/****
  OBJECT INIT FUNCTIONS
 ****/

static void
j2c_constant_pool_init (J2cConstantPool *self)
{
  return;
}

static void
j2c_constant_pool_item_init (J2cConstantPoolItem *self)
{
  return;
}
static void
j2c_class_info_init (J2cClassInfo *self)
{
  return;
}
static void
j2c_fieldref_info_init (J2cFieldrefInfo *self)
{
  return;
}
static void
j2c_methodref_info_init (J2cMethodrefInfo *self)
{
  return;
}
static void
j2c_interface_methodref_info_init (J2cInterfaceMethodrefInfo *self)
{
  return;
}
static void
j2c_string_info_init (J2cStringInfo *self)
{
  return;
}
static void
j2c_integer_info_init (J2cIntegerInfo *self)
{
  return;
}
static void
j2c_float_info_init (J2cFloatInfo *self)
{
  return;
}
static void
j2c_long_info_init (J2cLongInfo *self)
{
  return;
}
static void
j2c_double_info_init (J2cDoubleInfo *self)
{
  return;
}
static void
j2c_name_and_type_info_init (J2cNameAndTypeInfo *self)
{
  return;
}
static void
j2c_utf8_info_init (J2cUtf8Info *self)
{
  return;
}

static void
j2c_method_handle_info_init (J2cMethodHandleInfo *self)
{
  return;
}

static void
j2c_method_type_info_init (J2cMethodTypeInfo *self)
{
  return;
}
static void
j2c_invoke_dynamic_info_init (J2cInvokeDynamicInfo *self)
{
  return;
}

/****
  DISPOSE FUNCTIONS
 ****/

static void
j2c_constant_pool_dispose (GObject *object)
{
  J2cConstantPool *constant_pool = J2C_CONSTANT_POOL (object);

  g_ptr_array_free (constant_pool->pool, TRUE);
  constant_pool->pool = NULL;
  constant_pool->count = 0;

  G_OBJECT_CLASS (j2c_constant_pool_parent_class)->dispose (object);
}

static void
j2c_constant_pool_ptr_array_free (gpointer data)
{
  if (data)
    {
      g_return_if_fail (J2C_IS_CONSTANT_POOL_ITEM (data));
      g_object_unref (data);
    }
}

static void
j2c_utf8_info_dispose (GObject *object)
{
  J2cUtf8Info *utf8_info = J2C_UTF8_INFO (object);

  if (utf8_info->bytes)
    g_free (utf8_info->bytes);

  utf8_info->bytes = NULL;
  G_OBJECT_CLASS (j2c_utf8_info_parent_class)->dispose (object);
}

/****
  SETTER FUNCTIONS
 ****/

static void
j2c_constant_pool_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cConstantPool *self = J2C_CONSTANT_POOL (object);

  switch (property_id)
    {
      case PROP_COUNT:
	self->count = (guint16) g_value_get_uint64 (value);
	break;

      case PROP_POOL:
	if (self->pool)
	  {
	    g_warning ("Cannot set the constant_pool's array more than once.");
	    break;
	  }

	self->pool = g_value_get_boxed (value);
	break;

      default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	break;
    }
}

static void
j2c_constant_pool_item_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cConstantPoolItemPrivate *priv = j2c_constant_pool_item_get_instance_private (J2C_CONSTANT_POOL_ITEM(object));

  switch (property_id)
    {
    case PROP_TAG:
      priv->tag = (guint8) g_value_get_uint64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_class_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cClassInfo *self = J2C_CLASS_INFO (object);

  switch (property_id)
    {
    case PROP_NAME_INDEX:
      self->name_index = (guint16) g_value_get_uint64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_fieldref_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cFieldrefInfoPrivate *priv = j2c_fieldref_info_get_instance_private (J2C_FIELDREF_INFO (object));

  switch (property_id)
    {
    case PROP_CLASS_INDEX:
      priv->class_index = (guint16) g_value_get_uint64 (value);
      break;

    case PROP_NAME_AND_TYPE_INDEX:
      priv->name_and_type_index = (guint16) g_value_get_uint64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_string_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cStringInfo *self = J2C_STRING_INFO (object);

  switch (property_id)
    {
    case PROP_STRING_INDEX:
      self->string_index = (guint16) g_value_get_uint64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_integer_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cIntegerInfo *self = J2C_INTEGER_INFO (object);

  switch (property_id)
    {
    case PROP_VALUE:
      self->value = (gint32) g_value_get_int64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_float_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cFloatInfo *self = J2C_FLOAT_INFO (object);

  switch (property_id)
    {
    case PROP_VALUE:
      self->value = g_value_get_float (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_long_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cLongInfo *self = J2C_LONG_INFO (object);

  switch (property_id)
    {
    case PROP_VALUE:
      self->value = g_value_get_int64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_double_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cDoubleInfo *self = J2C_DOUBLE_INFO (object);

  switch (property_id)
    {
    case PROP_VALUE:
      self->value = g_value_get_double (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_name_and_type_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cNameAndTypeInfo *self = J2C_NAME_AND_TYPE_INFO (object);

  switch (property_id)
    {
    case PROP_NAME_INDEX:
      self->name_index = (guint16) g_value_get_uint64 (value);
      break;

    case PROP_DESCRIPTOR_INDEX:
      self->descriptor_index = (guint16) g_value_get_uint64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_utf8_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cUtf8Info *self = J2C_UTF8_INFO (object);

  switch (property_id)
    {
   case PROP_BYTES:
      self->bytes = g_value_dup_string (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_method_handle_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cMethodHandleInfo *self = J2C_METHOD_HANDLE_INFO (object);

  switch (property_id)
    {
    case PROP_REFERENCE_KIND:
      self->reference_kind = (guint8) g_value_get_uint64 (value);
      break;

    case PROP_REFERENCE_INDEX:
      self->reference_index = (guint16) g_value_get_uint64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}
static void
j2c_method_type_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cMethodTypeInfo *self = J2C_METHOD_TYPE_INFO (object);

  switch (property_id)
    {
    case PROP_DESCRIPTOR_INDEX:
      self->descriptor_index = (guint16) g_value_get_uint64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_invoke_dynamic_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cInvokeDynamicInfo *self = J2C_INVOKE_DYNAMIC_INFO (object);

  switch (property_id)
    {
    case PROP_BOOTSTRAP_METHOD_ATTR_INDEX:
      self->bootstrap_method_attr_index = (guint16) g_value_get_uint64 (value);
      break;

    case PROP_NAME_AND_TYPE_INDEX:
      self->name_and_type_index = (guint16) g_value_get_uint64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

/****
  GETTER FUNCTIONS
 ****/

static void
j2c_constant_pool_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cConstantPool *self = J2C_CONSTANT_POOL (object);

  switch (property_id)
    {
      case PROP_COUNT:
	g_value_set_uint64 (value, (guint64) self->count);
	break;

      case PROP_POOL:
	g_value_set_boxed (value, self->pool);
	break;

      default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	break;
    }
}

static void
j2c_constant_pool_item_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cConstantPoolItemPrivate *priv = j2c_constant_pool_item_get_instance_private (J2C_CONSTANT_POOL_ITEM(object));

  switch (property_id)
    {
    case PROP_TAG:
      g_value_set_uint64 (value, (guint64) priv->tag);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_class_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cClassInfo *self = J2C_CLASS_INFO (object);

  switch (property_id)
    {
    case PROP_NAME_INDEX:
      g_value_set_uint64 (value, (guint64) self->name_index);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_fieldref_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cFieldrefInfoPrivate *priv = j2c_fieldref_info_get_instance_private (J2C_FIELDREF_INFO (object));

  switch (property_id)
    {
    case PROP_CLASS_INDEX:
      g_value_set_uint64 (value, (guint64) priv->class_index);
      break;

    case PROP_NAME_AND_TYPE_INDEX:
      g_value_set_uint64 (value, (guint64) priv->name_and_type_index);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_string_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cStringInfo *self = J2C_STRING_INFO (object);

  switch (property_id)
    {
    case PROP_STRING_INDEX:
      g_value_set_uint64 (value, (guint64) self->string_index);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_integer_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cIntegerInfo *self = J2C_INTEGER_INFO (object);

  switch (property_id)
    {
    case PROP_VALUE:
      g_value_set_int64 (value, (gint64) self->value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_float_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cFloatInfo *self = J2C_FLOAT_INFO (object);

  switch (property_id)
    {
    case PROP_VALUE:
      g_value_set_float (value, self->value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_long_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cLongInfo *self = J2C_LONG_INFO (object);

  switch (property_id)
    {
    case PROP_VALUE:
      g_value_set_int64 (value, self->value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_double_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cDoubleInfo *self = J2C_DOUBLE_INFO (object);

  switch (property_id)
    {
    case PROP_VALUE:
      g_value_set_double (value, self->value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_name_and_type_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cNameAndTypeInfo *self = J2C_NAME_AND_TYPE_INFO (object);

  switch (property_id)
    {
    case PROP_NAME_INDEX:
      g_value_set_uint64 (value, (guint64) self->name_index);
      break;

    case PROP_DESCRIPTOR_INDEX:
      g_value_set_uint (value, (guint64) self->descriptor_index);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_utf8_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cUtf8Info *self = J2C_UTF8_INFO (object);

  switch (property_id)
    {
    case PROP_BYTES:
      g_value_set_string (value, self->bytes);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_method_handle_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cMethodHandleInfo *self = J2C_METHOD_HANDLE_INFO (object);

  switch (property_id)
    {
    case PROP_REFERENCE_KIND:
      g_value_set_uint64 (value, (guint64) self->reference_kind);
      break;

    case PROP_REFERENCE_INDEX:
      g_value_set_uint64 (value, (guint64) self->reference_index);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}
static void
j2c_method_type_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cMethodTypeInfo *self = J2C_METHOD_TYPE_INFO (object);

  switch (property_id)
    {
    case PROP_DESCRIPTOR_INDEX:
      g_value_set_uint64 (value, (guint64) self->descriptor_index);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_invoke_dynamic_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cInvokeDynamicInfo *self = J2C_INVOKE_DYNAMIC_INFO (object);

  switch (property_id)
    {
    case PROP_BOOTSTRAP_METHOD_ATTR_INDEX:
      g_value_set_uint64 (value, (guint64) self->bootstrap_method_attr_index);
      break;

    case PROP_NAME_AND_TYPE_INDEX:
      g_value_set_uint64 (value, (guint64) self->name_and_type_index);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_constant_pool_ptr_array_add (GPtrArray *arr, J2cConstantPoolItem *item)
{
  g_return_if_fail (NULL != arr);
  g_return_if_fail (NULL != item && J2C_IS_CONSTANT_POOL_ITEM (item));

  g_ptr_array_add (arr, item);
}
/****
  PUBLIC METHODS
 ****/

J2cConstantPool *
j2c_constant_pool_new (GDataInputStream *in, GError **error)
{
  /* Stop if something is wrong with error */
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (in != NULL, NULL);

  /****
    READ CONSTANT POOL
   ****/

  J2cConstantPool *ret = NULL;
  guint16 constant_pool_count = g_data_input_stream_read_uint16 (in, NULL, error); 

  if (*error) goto cleanup;

  GPtrArray *pool = g_ptr_array_sized_new ((guint) (constant_pool_count - 1));
  g_ptr_array_set_free_func (pool, j2c_constant_pool_ptr_array_free);
  for (gint i = 1; i < constant_pool_count; i ++)
    {
      guint8 tag = (guint8) g_data_input_stream_read_byte (in, NULL, error);
      if (*error) goto pool_error;

      guint16 name_index;
      guint16 class_index;
      guint16 name_and_type_index;
      guint16 string_index;
      guint32 bytes;
      guint32 high_bytes;
      guint32 low_bytes;
      guint16 descriptor_index;
      guint16 length;
      guint8  reference_kind;
      guint16 reference_index;
      guint16 bootstrap_method_attr_index;
      if (tag == CONSTANT_Class)
	  {
	    name_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    J2cClassInfo *info = g_object_new (J2C_TYPE_CLASS_INFO,
					       J2C_CONSTANT_POOL_PROP_TAG, tag,
					       J2C_CONSTANT_POOL_PROP_NAME_INDEX, name_index,
					       NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }

      else if (tag == CONSTANT_Fieldref)
	  {
	    class_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    name_and_type_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    J2cFieldrefInfo *info = g_object_new (J2C_TYPE_FIELDREF_INFO,
						  J2C_CONSTANT_POOL_PROP_TAG, tag,
						  J2C_CONSTANT_POOL_PROP_CLASS_INDEX, class_index,
						  J2C_CONSTANT_POOL_PROP_NAME_AND_TYPE_INDEX, name_and_type_index,
						  NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }

      else if (tag == CONSTANT_Methodref)
	  {
	    class_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    name_and_type_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    J2cFieldrefInfo *info = g_object_new (J2C_TYPE_METHODREF_INFO,
						  J2C_CONSTANT_POOL_PROP_TAG, tag,
						  J2C_CONSTANT_POOL_PROP_CLASS_INDEX, class_index,
						  J2C_CONSTANT_POOL_PROP_NAME_AND_TYPE_INDEX, name_and_type_index,
						  NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }

      else if (tag == CONSTANT_InterfaceMethodref)
	  {
	    class_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    name_and_type_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    J2cFieldrefInfo *info = g_object_new (J2C_TYPE_INTERFACE_METHODREF_INFO,
						  J2C_CONSTANT_POOL_PROP_TAG, tag,
						  J2C_CONSTANT_POOL_PROP_CLASS_INDEX, class_index,
						  J2C_CONSTANT_POOL_PROP_NAME_AND_TYPE_INDEX, name_and_type_index,
						  NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }

      else if (tag == CONSTANT_String)
	  {
	    string_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    J2cStringInfo *info = g_object_new (J2C_TYPE_STRING_INFO,
						J2C_CONSTANT_POOL_PROP_TAG, tag,
						J2C_CONSTANT_POOL_PROP_STRING_INDEX, string_index,
						NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }

      else if (tag == CONSTANT_Integer)
	  {
	    bytes = g_data_input_stream_read_uint32 (in, NULL, error);
	    if (*error) goto pool_error;

	    J2cIntegerInfo *info = g_object_new (J2C_TYPE_INTEGER_INFO,
						 J2C_CONSTANT_POOL_PROP_TAG, tag,
						 J2C_CONSTANT_POOL_PROP_VALUE, bytes,
						 NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }

      else if (tag == CONSTANT_Float)
	  {
	    bytes = g_data_input_stream_read_uint32 (in, NULL, error);
	    if (*error) goto pool_error;

	    gfloat value;
	    if (bytes == 0x7f800000)
	      {
#ifdef INFINITY
		value = INFINITY;
#else
		g_set_error (error,
			     J2C_CONSTANT_POOL_ERROR,
			     J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
			     "This platform does not support INFINITY value.");
		goto pool_error;
#endif
	      }
	    else if (bytes == 0xff800000)
	      {
#ifdef INFINITY
		value = -INFINITY;
#else
		g_set_error (error,
			     J2C_CONSTANT_POOL_ERROR,
			     J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
			     "This platform does not support INFINITY value.");
		goto pool_error;
#endif
	      }
	    else if ((bytes >= 0x7f800001 && bytes <= 0x7fffffff) || (bytes >= 0xff800001 && bytes <= 0xffffffff))
	      {
#ifdef NAN
		value = NAN;
#else
		g_set_error (error,
			     J2C_CONSTANT_POOL_ERROR,
			     J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
			     "This platform does not support NAN value.");
		goto pool_error;
#endif
	      }	
	    else
	      {
		gint s = ((bytes >> 31) == 0) ? 1 : -1;
		gint e = ((bytes >> 23) & 0xff);
		gint m = (e == 0) ?
		  (bytes & 0x7fffff) << 1 :
		  (bytes & 0x7fffff) | 0x800000;

		value = s * m * powf (2.0, ((gfloat) e) - 150.0);
	      }
	    J2cFloatInfo *info = g_object_new (J2C_TYPE_FLOAT_INFO,
						 J2C_CONSTANT_POOL_PROP_TAG, tag,
						 J2C_CONSTANT_POOL_PROP_VALUE, value,
						 NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }


      else if (tag == CONSTANT_Long)
	  {
	    high_bytes = g_data_input_stream_read_uint32 (in, NULL, error);
	    if (*error) goto pool_error;

	    low_bytes = g_data_input_stream_read_uint32 (in, NULL, error);
	    if (*error) goto pool_error;

	    gint64 value = ((gint64) high_bytes << 32) + low_bytes;
	    J2cLongInfo *info = g_object_new (J2C_TYPE_LONG_INFO,
					      J2C_CONSTANT_POOL_PROP_TAG, tag,
					      J2C_CONSTANT_POOL_PROP_VALUE, value,
					      NULL);

	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info)); /* 8-byte constants take up two entries in the constant pool */
	    i ++;	 
	  }

      else if (tag == CONSTANT_Double)
	  {
	    high_bytes = g_data_input_stream_read_uint32 (in, NULL, error);
	    if (*error) goto pool_error;

	    low_bytes = g_data_input_stream_read_uint32 (in, NULL, error);
	    if (*error) goto pool_error;

	    gint64 bits = ((gint64) high_bytes << 32) + low_bytes;
	    gdouble value;
	    if (bits == 0x7ff0000000000000L)
	      {
#ifdef INFINITY
		value = INFINITY;
#else
		g_set_error (error,
			     J2C_CONSTANT_POOL_ERROR,
			     J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
			     "This platform does not support INFINITY value.");
		goto pool_error;
#endif
	      }
	    else if (bits == 0xfff0000000000000L)
	      {
#ifdef INFINITY
		value = -INFINITY;
#else
		g_set_error (error,
			     J2C_CONSTANT_POOL_ERROR,
			     J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
			     "This platform does not support INFINITY value.");
		goto pool_error;
#endif
	      }
	    else if ((bits >= 0x7ff0000000000001L && bits <= 0x7fffffffffffffffL) || (bits >= 0xfff0000000000001L && bits <= 0xffffffffffffffffL))
	      {
#ifdef NAN
		value = NAN;
#else
		g_set_error (error,
			     J2C_CONSTANT_POOL_ERROR,
			     J2C_CONSTANT_POOL_INVALID_FLOAT_ERROR,
			     "This platform does not support NAN value.");
		goto pool_error;
#endif
	      }	
	    else
	      {
		gint s = ((bits >> 63) == 0) ? 1 : -1;
		gint e = (gint)((bits >> 52) & 0x7ffL);
		gint m = (e == 0) ?
		  (bits & 0xfffffffffffffL) << 1 :
		  (bytes & 0xfffffffffffffL) | 0x10000000000000L;

		value = s * m * pow (2.0, ((gdouble) e) - 1075.0);
	      }
	    J2cDoubleInfo *info = g_object_new (J2C_TYPE_DOUBLE_INFO,
					      J2C_CONSTANT_POOL_PROP_TAG, tag,
					      J2C_CONSTANT_POOL_PROP_VALUE, value,
					      NULL);

	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info)); /* 8-byte constants take up two entries in the constant pool */
	    i ++;
	  }

      else if (tag == CONSTANT_NameAndType)
	  {
	    name_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    descriptor_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    J2cNameAndTypeInfo *info = g_object_new (J2C_TYPE_NAME_AND_TYPE_INFO,
						     J2C_CONSTANT_POOL_PROP_TAG, tag,
						     J2C_CONSTANT_POOL_PROP_NAME_INDEX, name_index,
						     J2C_CONSTANT_POOL_PROP_DESCRIPTOR_INDEX, descriptor_index,
						     NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }

      else if (tag == CONSTANT_Utf8)
	  {
	    length = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    guint8 *utf8_bytes = g_malloc (length);
	    if (length > 0)
	      {
		if (length != g_input_stream_read (G_INPUT_STREAM (in),
						   utf8_bytes,
						   length,
						   NULL,
						   error)) {
		    g_free (utf8_bytes);
		    goto pool_error;
		}
	      }

	    gint index = 0;
	    gchar *value = g_strdup ("");
	    while (index < length)
	      {
		if (utf8_bytes[index] == 0 || (utf8_bytes[index] >= 0xf0 && utf8_bytes[index] <= 0xff))
		  {
		    g_set_error (error,
				 J2C_CONSTANT_POOL_ERROR,
				 J2C_CONSTANT_POOL_INVALID_UTF8_ERROR,
				 "No byte in a Utf8Info may be 0 or in the range "
				 "0xf0 to 0xff. This byte is %xd", utf8_bytes[index]);
		    g_free (utf8_bytes);
		    goto pool_error;
		  }

		gunichar c;
		if ((utf8_bytes[index] & 0x80) == 0)
		  {
		    c = utf8_bytes[index];
		    index ++;
		  }
		else if (index + 1 < length
			 && (utf8_bytes[index] & 0xe0) == 0xc0
			 && (utf8_bytes[index+1] & 0xc0) == 0x80)
		  {
		    c = ((utf8_bytes[index] & 0x1f) << 6) + (utf8_bytes[index+1] & 0x3f);
		    index += 2;
		  }
		else if (index + 2 < length
			 && (utf8_bytes[index] & 0xf0) == 0xe0
			 && (utf8_bytes[index+1] & 0xc0) == 0x80
			 && (utf8_bytes[index+2] & 0xc0) == 0x80)
		  {
		    c = ((utf8_bytes[index] & 0xf) << 12) + ((utf8_bytes[index+1] & 0x3f) << 6) + (utf8_bytes[index+2] & 0x3f);
		    index += 3;
		  }
		else if (index + 5 < length
			 && utf8_bytes[index] == 0xed
			 && (utf8_bytes[index+1] & 0xf0) == 0xa0
			 && (utf8_bytes[index+2] & 0xc0) == 0x80
			 && utf8_bytes[index+3] == 0xed
			 && (utf8_bytes[index+4] & 0xf0) == 0xb0
			 && (utf8_bytes[index+5] & 0xc0) == 0x80)
		  {
		    c = 0x10000 + ((utf8_bytes[index + 1] & 0x0f) << 16)
		      + ((utf8_bytes[index + 2] & 0x3f) << 10)
		      + ((utf8_bytes[index + 3] & 0x0f) << 6)
		      + (utf8_bytes[index + 5] & 0x3f);
		    index += 8;
		  }
		else
		  {
		    g_set_error (error,
				 J2C_CONSTANT_POOL_ERROR,
				 J2C_CONSTANT_POOL_INVALID_UTF8_ERROR,
				 "Cannot parse utf8 bytes %xd %xd %xd %xd %xd %xd",
				 utf8_bytes[index],
				 index + 1 < length ? utf8_bytes[index+1] : 0,
				 index + 2 < length ? utf8_bytes[index+2] : 0,
				 index + 3 < length ? utf8_bytes[index+3] : 0,
				 index + 4 < length ? utf8_bytes[index+4] : 0,
				 index + 5 < length ? utf8_bytes[index+5] : 0);

		    g_free (utf8_bytes);
		    goto pool_error;
		  }

		gchar *temp = g_strdup_printf ("%s%c", value, c);
		g_free (value);
		value = temp;
	      }

	    gchar *normal = g_utf8_normalize (value, -1, G_NORMALIZE_DEFAULT);
	    if (!normal)
	      {
		g_set_error (error,
			     J2C_CONSTANT_POOL_ERROR,
			     J2C_CONSTANT_POOL_INVALID_UTF8_ERROR,
			     "Could not normalize utf8 string.");
		g_free (value);
		goto pool_error;
	      }

	    g_free (value);

	    J2cUtf8Info *info = g_object_new (J2C_TYPE_UTF8_INFO,
					      J2C_CONSTANT_POOL_PROP_TAG, tag,
					      J2C_CONSTANT_POOL_PROP_BYTES, normal,
					      NULL);
	    g_free (normal);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }
      
      else if (tag == CONSTANT_MethodHandle)
	  {
	    reference_kind = g_data_input_stream_read_byte (in, NULL, error);
	    if (*error) goto pool_error;

	    reference_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    J2cMethodHandleInfo *info = g_object_new (J2C_TYPE_METHOD_HANDLE_INFO,
						      J2C_CONSTANT_POOL_PROP_TAG, tag,
						      J2C_CONSTANT_POOL_PROP_REFERENCE_KIND, reference_kind,
						      J2C_CONSTANT_POOL_PROP_REFERENCE_INDEX, reference_index,
						      NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }

      else if (tag == CONSTANT_MethodType)
	  {
	    descriptor_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    J2cMethodTypeInfo *info = g_object_new (J2C_TYPE_METHOD_TYPE_INFO,
						    J2C_CONSTANT_POOL_PROP_TAG, tag,
						    J2C_CONSTANT_POOL_PROP_DESCRIPTOR_INDEX, descriptor_index,
						    NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }

      else if (tag == CONSTANT_InvokeDynamic)
	  {
	    bootstrap_method_attr_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    name_and_type_index = g_data_input_stream_read_uint16 (in, NULL, error);
	    if (*error) goto pool_error;

	    J2cInvokeDynamicInfo *info = g_object_new (J2C_TYPE_INVOKE_DYNAMIC_INFO,
						    J2C_CONSTANT_POOL_PROP_TAG, tag,
						    J2C_CONSTANT_POOL_PROP_BOOTSTRAP_METHOD_ATTR_INDEX, bootstrap_method_attr_index,
						    J2C_CONSTANT_POOL_PROP_NAME_AND_TYPE_INDEX, name_and_type_index,
						    NULL);
	    j2c_constant_pool_ptr_array_add (pool, J2C_CONSTANT_POOL_ITEM (info));
	  }

      else
	  {
	    g_set_error (error,
			 J2C_CONSTANT_POOL_ERROR,
			 J2C_CONSTANT_POOL_INVALID_TAG_ERROR,
			 "%u is not a valid constant pool tag",
			 tag);
	    goto pool_error;
	  }
    }

  ret = g_object_new (J2C_TYPE_CONSTANT_POOL,
		      J2C_CONSTANT_POOL_PROP_POOL, pool,
		      J2C_CONSTANT_POOL_PROP_COUNT, constant_pool_count,
		      NULL);
  goto cleanup;

pool_error:
   g_ptr_array_free (pool, TRUE);

cleanup:
  return ret;
}

J2cConstantPoolItem *
j2c_constant_pool_get (J2cConstantPool *self, const guint16 index, GError **error)
{
  g_return_val_if_fail (error != NULL || *error != NULL, NULL);

  if (!self)
    {
      g_set_error (error,
		   J2C_CONSTANT_POOL_ERROR,
		   J2C_CONSTANT_POOL_NULL_ERROR,
		   "Cannot get from a null pool.");
      return NULL;
    }

  if (index < 1 || index > self->pool->len || index >= self->count)
    {
      g_set_error (error,
		   J2C_CONSTANT_POOL_ERROR,
		   J2C_CONSTANT_POOL_INDEX_ERROR,
		   "%u is not a valid index for this pool { count = %u; length = %u }",
		   index, self->count, self->pool->len);
      return NULL;
    }

  J2cConstantPoolItem *ret = g_object_ref (self->pool->pdata[index-1]);
  return ret;
}

gchar *
j2c_constant_pool_get_string (J2cConstantPool *self, const guint16 index, GError **error)
{
  J2cConstantPoolItem *info = j2c_constant_pool_get (self, index, error);

  if (*error)
    return NULL;

  J2cConstantPoolItemPrivate *priv = j2c_constant_pool_item_get_instance_private (J2C_CONSTANT_POOL_ITEM (info));
  if (priv->tag != CONSTANT_Utf8)
    {
      g_set_error (error,
		   J2C_CONSTANT_POOL_ERROR,
		   J2C_CONSTANT_POOL_TYPE_ERROR,
		   "Item at index %u is not a string.",
		   index);
      return NULL;
    }

  gchar *ret = g_strdup (J2C_UTF8_INFO (info)->bytes);
  g_object_unref (info);
  return ret;
}

gint
j2c_constant_pool_get_int (J2cConstantPool *self, const guint16 index, GError **error)
{
  J2cConstantPoolItem *info = j2c_constant_pool_get (self, index, error);
  if (*error)
    return 0;

  J2cConstantPoolItemPrivate *priv = j2c_constant_pool_item_get_instance_private (J2C_CONSTANT_POOL_ITEM (info));
  gint ret;
  if (priv->tag == CONSTANT_Integer)
    {
      ret = (gint) J2C_INTEGER_INFO (info)->value;
    }
  else if (priv->tag == CONSTANT_Long)
    {
      ret = (gint) J2C_LONG_INFO (info)->value;
    }
  else
    {
      g_set_error (error,
		   J2C_CONSTANT_POOL_ERROR,
		   J2C_CONSTANT_POOL_TYPE_ERROR,
		   "Item at index %u is not an integer.",
		   index);
      ret = 0;
    }

  g_object_unref (info);
  return ret;
}

gint32
j2c_constant_pool_get_int32 (J2cConstantPool *self, const guint16 index, GError **error)
{
  J2cConstantPoolItem *info = j2c_constant_pool_get (self, index, error);
  if (*error)
    return 0;

  J2cConstantPoolItemPrivate *priv = j2c_constant_pool_item_get_instance_private (J2C_CONSTANT_POOL_ITEM (info));
  gint32 ret;
  if (priv->tag == CONSTANT_Integer)
    {
      ret = (gint32) J2C_INTEGER_INFO (info)->value;
    }
  else if (priv->tag == CONSTANT_Long)
    {
      ret = (gint32) J2C_LONG_INFO (info)->value;
    }
  else
    {
      g_set_error (error,
		   J2C_CONSTANT_POOL_ERROR,
		   J2C_CONSTANT_POOL_TYPE_ERROR,
		   "Item at index %u is not an integer.",
		   index);
      ret = 0;
    }

  g_object_unref (info);
  return ret;
}

gint64
j2c_constant_pool_get_int64 (J2cConstantPool *self, const guint16 index, GError **error)
{
  J2cConstantPoolItem *info = j2c_constant_pool_get (self, index, error);
  if (*error)
    return 0;

  J2cConstantPoolItemPrivate *priv = j2c_constant_pool_item_get_instance_private (J2C_CONSTANT_POOL_ITEM (info));
  gint64 ret;
  if (priv->tag == CONSTANT_Integer)
    {
      ret = (gint64) J2C_INTEGER_INFO (info)->value;
    }
  else if (priv->tag == CONSTANT_Long)
    {
      ret = (gint64) J2C_LONG_INFO (info)->value;
    }
  else
    {
      g_set_error (error,
		   J2C_CONSTANT_POOL_ERROR,
		   J2C_CONSTANT_POOL_TYPE_ERROR,
		   "Item at index %u is not an integer or long.",
		   index);
      ret = 0;
    }

  g_object_unref (info);
  return ret;
}

gfloat
j2c_constant_pool_get_float (J2cConstantPool *self, const guint16 index, GError **error)
{
  J2cConstantPoolItem *info = j2c_constant_pool_get (self, index, error);
  if (*error)
    return 0;

  J2cConstantPoolItemPrivate *priv = j2c_constant_pool_item_get_instance_private (J2C_CONSTANT_POOL_ITEM (info));
  gfloat ret;
  if (priv->tag == CONSTANT_Float)
    {
      ret = (gfloat) J2C_FLOAT_INFO (info)->value;
    }
  else if (priv->tag == CONSTANT_Double)
    {
      ret = (gfloat) J2C_DOUBLE_INFO (info)->value;
    }
  else
    {
      g_set_error (error,
		   J2C_CONSTANT_POOL_ERROR,
		   J2C_CONSTANT_POOL_TYPE_ERROR,
		   "Item at index %u is not a float or double.",
		   index);
      ret = 0;
    }

  g_object_unref (info);
  return ret;

}

gdouble
j2c_constant_pool_get_double (J2cConstantPool *self, const guint16 index, GError **error)
{
  J2cConstantPoolItem *info = j2c_constant_pool_get (self, index, error);
  if (*error)
    return 0;

  J2cConstantPoolItemPrivate *priv = j2c_constant_pool_item_get_instance_private (J2C_CONSTANT_POOL_ITEM (info));
  gdouble ret;
  if (priv->tag == CONSTANT_Float)
    {
      ret = (gdouble) J2C_FLOAT_INFO (info)->value;
    }
  else if (priv->tag == CONSTANT_Double)
    {
      ret = (gdouble) J2C_DOUBLE_INFO (info)->value;
    }
  else
    {
      g_set_error (error,
		   J2C_CONSTANT_POOL_ERROR,
		   J2C_CONSTANT_POOL_TYPE_ERROR,
		   "Item at index %u is not a float or double.",
		   index);
      ret = 0;
    }

  g_object_unref (info);
  return ret;
}

guint8
j2c_constant_pool_get_tag (J2cConstantPool *self, const guint16 index, GError **error)
{
  J2cConstantPoolItem *info = j2c_constant_pool_get (self, index, error);
  if (*error)
    return 0;

  J2cConstantPoolItemPrivate *priv = j2c_constant_pool_item_get_instance_private (J2C_CONSTANT_POOL_ITEM (info));
  guint8 ret = priv->tag;
  g_object_unref (info);
  return ret;
}
