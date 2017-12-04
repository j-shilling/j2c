#include <j2c/constant-pool.h>

/****
  FINAL TYPE DEFINITIONS
 ****/

struct _J2cConstantPool
{
  GObject parent;

  guint16 count;
  J2cConstandPoolItem *pool;
};

struct _J2cClassInfo
{
  J2cClassPoolItem parent;

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
  J2cClassPoolItem parent;

  guint16 string_index;
};

struct _J2cIntegerInfo
{
  J2cClassPoolItem parent;
  gint32 value;
};

struct _J2cFloatInfo
{
  J2cClassPoolItem parent;
  gfloat value
}; 

struct _J2cLongInfo
{
  J2cClassPoolItem parent;
  gint64 value
}; 

struct _J2cDoubleInfo
{
  J2cClassPoolItem parent;
  gdouble value
}; 

struct _J2cNameAndTypeInfo
{
  J2cClassPoolItem parent;
  guint16 name_index;
  guint16 descriptor_index;
};

struct _J2cUtf8Info
{
  J2cClassPoolItem parent;
  guint16 length;
  gchar *bytes;
};

struct _J2cMethodHandleInfo
{
  J2cClassPoolItem parent;
  guint8 reference_kind;
  guint16 reference_index;
};

struct _J2cMethodTypeInfo
{
  J2cClassPoolItem parent;
  guint16 descriptor_index;
};

struct _J2cInvokeDynamicInfo
{
  J2cClassPoolItem parent;
  guint16 bootstrap_method_attr_index;
  guint16 name_and_type_index;
};

/****
  DERIVABLE PRIV STRUCTURES
 ****/

typedef struct
{
  guint8 tag;
} J2cClassPoolItemPrivate;

typedef struct
{
  guint16 class_index;
  guint16 name_and_type_index;
} J2cFieldrefInfoPriv;

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
static void j2c_constant_pool_utf8_dispose (GObject *object);

static void j2c_constant_pool_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_constant_pool_item_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_fieldref_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_string_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_integer_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_float_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_long_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_double_iynfo_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_name_and_type_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_utf8_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_method_handle_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_method_type_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_invoke_dynamic_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void j2c_constant_pool_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_constant_pool_item_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_fieldref_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_string_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_integer_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_float_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_long_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_double_iynfo_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_name_and_type_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_utf8_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_method_handle_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_method_type_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_invoke_dynamic_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

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
							"long and double noted in §4.4.5."
							0, G_MAXUINT16,
							1,
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_POOL,
				   g_param_spec_boxed (J2C_CONSTANT_POOL_PROP_POOL,
							"pool",
							"The array of constant pool entries.",
							G_TYPE_PTR_ARRAY,
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));

}

static void
j2c_constant_pool_item_class_init (J2cConstantPoolClass *klass)
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
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_class_info_class_init (J2cConstantPoolClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_class_info_set_property;
  object_class->get_property = j2c_class_info_get_property;

  g_object_class_install_property (object_class, PROP_NAME_INDEX,
				   g_param_spec_uint64 (J2C_CONSTAND_POOL_PROP_NAME_INDEX,
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
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_fieldref_info_class_init (J2cConstantPoolClass *klass)
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
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
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
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_methodref_info_class_init (J2cConstantPoolClass *klass)
{
  return;
}

static void
j2c_interface_methodref_info_class_init (J2cConstantPoolClass *klass)
{
  return;
}

static void
j2c_string_info_class_init (J2cConstantPoolClass *klass)
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
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_integer_info_class_init (J2cConstantPoolClass *klass)
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
						       G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_float_info_class_init (J2cConstantPoolClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_float_info_set_property;
  object_class->get_property = j2c_float_info_get_property;

  g_object_class_install_property (object_class, PROP_VALUE,
				   g_param_spec_float (J2C_CONSTANT_POOL_PROP_VALUE,
						       "value",
						       "The value of a data type in the constant pool.",
						       G_MINFLOAT, G_MAXFLOAT,
						       0,
						       G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_long_info_class_init (J2cConstantPoolClass *klass)
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
						       G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_double_info_class_init (J2cConstantPoolClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_double_info_set_property;
  object_class->get_property = j2c_double_info_get_property;

  g_object_class_install_property (object_class, PROP_VALUE,
				   g_param_spec_double (J2C_CONSTANT_POOL_PROP_VALUE,
							"value",
						        "The value of a data type in the constant pool.",
							G_MINDOUBLE, G_MAXDOUBLE,
							0,
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_name_and_type_info_class_init (J2cConstantPoolClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_name_and_type_set_property;
  object_class->get_property = j2c_name_and_type_get_property;

  g_object_class_install_property (object_class, PROP_NAME_INDEX,
				   g_param_spec_uint64 (J2C_CONSTAND_POOL_PROP_NAME_INDEX,
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
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_DESCRIPTOR_INDEX,
				   g_param_spec_uint64 (J2C_CONSTAND_POOL_PROP_DESCRIPTOR_INDEX,
							"descriptor_index",
							"The value of the descriptor_index item must "
							"be a valid index into the constant_pool "
							"table. The constant_pool entry at that index "
							"must be a CONSTANT_Utf8_info structure "
							"(§4.4.7) representing a valid field descriptor "
							"or method descriptor (§4.3.2, §4.3.3).",
							0, G_MAXUINT16,
							0,
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_utf8_info_class_init (J2cConstantPoolClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_utf8_info_set_property;
  object_class->get_property = j2c_utf8_info_get_property;
  object_class->dispose = j2c_utf8_info_dispose;

  g_object_class_install_property (object_class, PROP_LENGTH,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_LENGTH,
							"length",
							"The value of the length item gives "
							"the number of bytes in the bytes array "
							"(not the length of the resulting string).",
							0, G_MAXUINT16,
							0,
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_BYTES,
				   g_param_spec_string (J2C_CONSTANT_POOL_PROP_BYTES,
							"bytes",
							"bytes of the utf8 string.",
							"",
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_method_handle_info_class_init (J2cConstantPoolClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_method_handle_set_property;
  object_class->get_property = j2c_method_handle_get_property;

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
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_REFERENCE_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_REFERENCE_INDEX,
							"reference_index",
							"The value of the reference_index item must "
							"be a valid index into the constant_pool table.",
							0, G_MAXUINT16,
							0,
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_method_type_info_class_init (J2cConstantPoolClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_method_type_set_property;
  object_class->get_property = j2c_method_type_get_property;

  g_object_class_install_property (object_class, PROP_DESCRIPTOR_INDEX,
				   g_param_spec_uint64 (J2C_CONSTAND_POOL_PROP_DESCRIPTOR_INDEX,
							"descriptor_index",
							"The value of the descriptor_index item must "
							"be a valid index into the constant_pool "
							"table. The constant_pool entry at that index "
							"must be a CONSTANT_Utf8_info structure "
							"(§4.4.7) representing a valid method descriptor.",
							0, G_MAXUINT16,
							0,
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));

}

static void
j2c_invoke_dynamic_info_class_init (J2cConstantPoolClass *klass)
{
  /* Override virtual methods */
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_invoke_dynamic_set_property;
  object_class->get_property = j2c_invoke_dynamic_get_property;

  g_object_class_install_property (object_class, PROP_BOOTSTRAP_ATTR_INDEX,
				   g_param_spec_uint64 (J2C_CONSTANT_POOL_PROP_BOOSTRAP_METHOD_ATTR_INDEX,
							"bootstrap_method_attr_index",
							"The value of the bootstrap_method_attr_index "
							"item must be a valid index into the "
							"bootstrap_methods array of the bootstrap "
							"method table (§4.7.23) of this class file.",
							0, G_MAXUINT16,
							0,
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
  
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
							G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY));
}

/****
  OBJECT INIT FUNCTIONS
 ****/

static void
j2c_constant_pool_init (J2cConstantPoolClass *klass)
{
  return;
}

static void
j2c_constant_pool_item_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_class_info_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_fieldref_info_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_methodref_info_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_interface_methodref_info_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_string_info_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_integer_info_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_float_info_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_long_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_double_info_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_name_and_type_info_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_utf8_info_init (J2cConstantPoolClass *klass)
{
  return;
}

static void
j2c_method_handle_info_init (J2cConstantPoolClass *klass)
{
  return;
}

static void
j2c_method_type_info_init (J2cConstantPoolClass *klass)
{
  return;
}
static void
j2c_invoke_dynamic_info_init (J2cConstantPoolClass *klass)
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

  for (gint i = 0; i < constant_pool->count; i++)
    g_object_unref (constant_pool->pool[i]);

  g_free (constant_pool->pool);
  constant_pool->pool = NULL;
  constant_pool->count = 0;
}

static void
j2c_constant_pool_utf8_dispose (GObject *object)
{
  J2cUtf8Info *utf8_info = J2C_UTF8_INFO (object);

  if (utf8_info->bytes)
    g_free (utf8_info->bytes);

  utf8_info->bytes = NULL;
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

	GPtrArray *array = g_value_get_boxed (value);
	self->pool = g_ptr_array_free (array, FALSE);
	break;

      default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	break;
    }
}

static void
j2c_constant_pool_item_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cClassPoolItemPrivate *priv = j2c_class_pool_item_get_private (J2C_CLASS_POOL_ITEM(object));

  switch (property_id)
    {
    case PROP_TAG:
      priv->tag = (guint8) g_value_get_uint64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspsec);
      break;
    }
}

static void
j2c_fieldref_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
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

static void j2c_long_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_double_iynfo_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_name_and_type_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_utf8_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_method_handle_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_method_type_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_invoke_dynamic_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


