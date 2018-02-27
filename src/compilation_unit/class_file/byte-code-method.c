#include <j2c/byte-code-method.h>
#include <j2c/object-array.h>
#include <j2c/attributes.h>
#include <j2c/opcodes.h>
#include <j2c/descriptors.h>
#include <j2c/logger.h>
#include <j2c/constant-pool-item.h>

struct _J2cByteCodeMethod
{
  J2cMethod parent;

  /* From J2cMethodInfo */
  guint16 access_flags;
  guint16 name_index;
  guint16 descriptor_index;
  J2cObjectArray *method_attributes;

  /* From J2cAttributeCode */
  guint16 max_stack;
  guint16 max_locals;
  J2cObjectArray *exception_table; /* J2cExceptionInfo[] */
  J2cObjectArray *code_attributes; /* J2cAttribute*[] */

  /* From constructor */
  J2cConstantPool *constant_pool;
  GPtrArray *instructions; /* J2cByteInstruction[] */
};

G_DEFINE_TYPE(J2cByteCodeMethod, j2c_byte_code_method, J2C_TYPE_METHOD)

enum
{
  PROP_ACCESS_FLAGS = 1,
  PROP_NAME_INDEX,
  PROP_DESCRIPTOR_INDEX,
  PROP_METHOD_ATTRIBUTES,
  PROP_MAX_STACK,
  PROP_MAX_LOCALS,
  PROP_EXCEPTION_TABLE,
  PROP_CODE_ATTRIBUTES,
  PROP_CONSTANT_POOL,
  PROP_INSTRUCTIONS,
  N_PROPERTIES
};

static GParamSpec *object_properties[N_PROPERTIES] = { NULL, };

static void
j2c_byte_code_method_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cByteCodeMethod *self = J2C_BYTE_CODE_METHOD (object);

  switch (property_id)
    {
      case PROP_ACCESS_FLAGS:
        self->access_flags = g_value_get_uint (value);
        break;
      case PROP_NAME_INDEX:
        self->name_index = g_value_get_uint (value);
        break;
      case PROP_DESCRIPTOR_INDEX:
        self->descriptor_index = g_value_get_uint (value);
        break;
      case PROP_METHOD_ATTRIBUTES:
        if (self->method_attributes)
          g_object_unref (self->method_attributes);
        self->method_attributes = g_value_dup_object (value);
        break;
      case PROP_MAX_STACK:
        self->max_stack = g_value_get_uint (value);
        break;
      case PROP_MAX_LOCALS:
        self->max_locals = g_value_get_uint (value);
        break;
      case PROP_EXCEPTION_TABLE:
        if (self->exception_table)
          g_object_unref (self->exception_table);
        self->exception_table = g_value_dup_object (value);
        break;
      case PROP_CODE_ATTRIBUTES:
        if (self->code_attributes)
          g_object_unref (self->code_attributes);
        self->code_attributes = g_value_dup_object (value);
        break;
      case PROP_CONSTANT_POOL:
        if (self->constant_pool)
          g_object_unref (self->constant_pool);
        self->constant_pool = g_value_dup_object (value);
        break;
      case PROP_INSTRUCTIONS:
        if (self->instructions)
          g_ptr_array_unref (self->instructions);
        self->instructions = g_ptr_array_ref (g_value_get_boxed (value));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_byte_code_method_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cByteCodeMethod *self = J2C_BYTE_CODE_METHOD (object);

  switch (property_id)
    {
      case PROP_ACCESS_FLAGS:
        g_value_set_uint (value, self->access_flags);
        break;
      case PROP_NAME_INDEX:
        g_value_set_uint (value, self->name_index);
        break;
      case PROP_DESCRIPTOR_INDEX:
        g_value_set_uint (value, self->descriptor_index);
        break;
      case PROP_METHOD_ATTRIBUTES:
        g_value_set_object (value, self->method_attributes);
        break;
      case PROP_MAX_STACK:
        g_value_set_uint (value, self->max_stack);
        break;
      case PROP_MAX_LOCALS:
        g_value_set_uint (value, self->max_locals);
        break;
      case PROP_EXCEPTION_TABLE:
        g_value_set_object (value, self->exception_table);
        break;
      case PROP_CODE_ATTRIBUTES:
        g_value_set_object (value, self->code_attributes);
        break;
      case PROP_CONSTANT_POOL:
        g_value_set_object (value, self->constant_pool);
        break;
      case PROP_INSTRUCTIONS:
        g_value_set_boxed (value, self->instructions);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_byte_code_method_dispose (GObject *object)
{
  J2cByteCodeMethod *self = J2C_BYTE_CODE_METHOD (object);

  if (self->method_attributes)
    g_clear_object (&self->method_attributes);

  if (self->exception_table)
    g_clear_object (&self->exception_table);

  if (self->code_attributes)
    g_clear_object (&self->code_attributes);

  if (self->constant_pool)
    g_clear_object (&self->constant_pool);

  if (self->instructions)
    {
      for (gint i = 0; i < self->instructions->len; i ++)
        if (self->instructions->pdata[i])
          g_object_unref (self->instructions->pdata[i]);
      g_ptr_array_free (self->instructions, TRUE);
      self->instructions = NULL;
    }

  G_OBJECT_CLASS (j2c_byte_code_method_parent_class)->dispose (object);
}

static const gchar *
j2c_byte_code_method_get_descriptor (J2cMethod *self)
{
  g_return_val_if_fail (self != NULL, NULL);

  GError *error = NULL;
  J2cConstantPoolItem *item =
    j2c_constant_pool_get (J2C_BYTE_CODE_METHOD (self)->constant_pool,
			   J2C_BYTE_CODE_METHOD (self)->descriptor_index,
			   &error);
  if (error)
    {
      j2c_logger_warning ("%s", error->message);
      g_error_free (error);
      return NULL;
    }

  const gchar *ret = j2c_utf8_info_string (J2C_UTF8_INFO (item));
  g_object_unref (item);

  return ret;
}

static const gchar *
j2c_byte_code_method_get_java_name (J2cMethod *self)
{
  g_return_val_if_fail (self != NULL, NULL);

  GError *error = NULL;
  J2cConstantPoolItem *item =
    j2c_constant_pool_get (J2C_BYTE_CODE_METHOD (self)->constant_pool,
			   J2C_BYTE_CODE_METHOD (self)->name_index,
			   &error);
  if (error)
    {
      j2c_logger_warning ("%s", error->message);
      g_error_free (error);
      return NULL;
    }

  const gchar *ret = j2c_utf8_info_string (J2C_UTF8_INFO (item));
  g_object_unref (item);

  return ret;
}

static guint16
j2c_byte_code_method_get_access_flags (J2cMethod *self)
{
  g_return_val_if_fail (self != NULL, 0);

  return J2C_BYTE_CODE_METHOD(self)->access_flags;
}

static gchar **
j2c_byte_code_method_get_reference_types (J2cMethod *self, gchar **param_types, GError **error)
{
  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (J2C_IS_BYTE_CODE_METHOD (self), NULL);

  const gchar *descriptor = j2c_method_get_descriptor (self);

  size_t len = 0;
  gchar **ret = g_malloc0 (sizeof (gchar *) * (len + 1));

  gchar *returns = j2c_descriptor_get_return_type (descriptor);
  while (j2c_descriptor_is_array_type (returns))
    {
      gchar *tmp = returns;
      returns = j2c_descriptor_get_array_content_type (tmp);
      g_free (tmp);
    }
  if (j2c_descriptor_is_reference_type (returns))
    {
      len ++;
      gchar **tmp = g_malloc0 (sizeof (gchar *)*(len+1));
      for (gint i = 0; i < (len - 1); i++)
	tmp[i] = ret[i];
      tmp[len - 1] = returns;
      g_free (ret);
      ret = tmp;
    }
  else
    {
      g_free (returns);
    }

  gchar **args = j2c_descriptor_get_params (descriptor);
  for (gchar **arg = args; arg && *arg; arg ++)
    {
      while (j2c_descriptor_is_array_type (*arg))
        {
          gchar *tmp = *arg;
          *arg = j2c_descriptor_get_array_content_type (tmp);
          g_free (tmp);
        }
      if (j2c_descriptor_is_reference_type (*arg))
	{
	  len ++;
	  gchar **tmp = g_malloc0 (sizeof (gchar*)*(len + 1));
	  for (gint i = 0; i < (len - 1); i ++)
	    tmp[i] = ret[i];
	  tmp[len-1] = *arg;
	  g_free (ret);
	  ret = tmp;
	}
      else
	{
	  g_free (*arg);
	}
    }
  if (args) g_free (args);

  return ret;
}

static void
j2c_byte_code_method_class_init (J2cByteCodeMethodClass *klass)
{
  J2cMethodClass *method_class      = J2C_METHOD_CLASS (klass);
  method_class->get_descriptor      = j2c_byte_code_method_get_descriptor;
  method_class->get_java_name       = j2c_byte_code_method_get_java_name;
  method_class->get_access_flags    = j2c_byte_code_method_get_access_flags;
  method_class->get_reference_types = j2c_byte_code_method_get_reference_types;

  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_byte_code_method_set_property;
  object_class->get_property = j2c_byte_code_method_get_property;
  object_class->dispose      = j2c_byte_code_method_dispose;

  object_properties[PROP_ACCESS_FLAGS] =
    g_param_spec_uint ("access-flags",
                       "access-flags",
                       "access-flags",
                       0, G_MAXUINT16,
                       0,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  object_properties[PROP_NAME_INDEX] =
    g_param_spec_uint ("name-index",
                       "name-index",
                       "name-index",
                       0, G_MAXUINT16,
                       0,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  object_properties[PROP_DESCRIPTOR_INDEX] =
    g_param_spec_uint ("descriptor-index",
                       "descriptor-index",
                       "descriptor-index",
                       0, G_MAXUINT16,
                       0,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  object_properties[PROP_METHOD_ATTRIBUTES] =
    g_param_spec_object ("method-attributes",
                         "method-attributes",
                         "method-attributes",
                         J2C_TYPE_OBJECT_ARRAY,
                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  object_properties[PROP_MAX_STACK] =
    g_param_spec_uint ("max-stack",
                       "max-stack",
                       "max-stack",
                       0, G_MAXUINT16,
                       0,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  object_properties[PROP_MAX_LOCALS] =
    g_param_spec_uint ("max-locals",
                       "max-locals",
                       "max-locals",
                       0, G_MAXUINT16,
                       0,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  object_properties[PROP_EXCEPTION_TABLE] =
    g_param_spec_object ("exception-table",
                         "exception-table",
                         "exception-table",
                         J2C_TYPE_OBJECT_ARRAY,
                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  object_properties[PROP_CODE_ATTRIBUTES] =
    g_param_spec_object ("code-attributes",
                         "code-attributes",
                         "code-attributes",
                         J2C_TYPE_OBJECT_ARRAY,
                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  object_properties[PROP_CONSTANT_POOL] =
    g_param_spec_object ("constant-pool",
                         "constant-pool",
                         "constant-pool",
                         J2C_TYPE_CONSTANT_POOL,
                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  object_properties[PROP_INSTRUCTIONS] =
    g_param_spec_boxed ("instructions",
                        "instructions",
                        "instructions",
                        G_TYPE_PTR_ARRAY,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (object_class, N_PROPERTIES, object_properties);
}

static void
j2c_byte_code_method_init (J2cByteCodeMethod *self)
{
  self->method_attributes = NULL;
  self->exception_table = NULL;
  self->code_attributes = NULL;
  self->constant_pool = NULL;
  self->instructions = NULL;
}

J2cByteCodeMethod *
j2c_byte_code_method_new (gchar *type, J2cMethodInfo *info, J2cConstantPool *constant_pool, GError **error)
{
  g_return_val_if_fail (info != NULL, NULL);
  g_return_val_if_fail (constant_pool != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (type != NULL && *type != '\0', NULL);

  J2cByteCodeMethod *ret = NULL;
  GError *tmp_error = NULL;

  GValue val = G_VALUE_INIT;
  g_value_init (&val, G_TYPE_UINT);

  g_object_get_property (G_OBJECT (info), METHOD_OR_FIELD_PROP_ACCESS_FLAGS, &val);
  guint16 access_flags = g_value_get_uint (&val);

  g_object_get_property (G_OBJECT (info), METHOD_OR_FIELD_PROP_NAME_INDEX, &val);
  guint16 name_index = g_value_get_uint (&val);

  g_object_get_property (G_OBJECT (info), METHOD_OR_FIELD_PROP_DESCRIPTOR_INDEX, &val);
  guint16 descriptor_index = g_value_get_uint (&val);

  g_value_unset (&val);
  g_value_init (&val, J2C_TYPE_OBJECT_ARRAY);

  g_object_get_property (G_OBJECT (info), METHOD_OR_FIELD_PROP_ATTRIBUTES, &val);
  J2cObjectArray *method_attributes = g_value_dup_object (&val);

  J2cObjectArray *exception_table = NULL;
  J2cObjectArray *code_attributes = NULL;
  GPtrArray *instructions = NULL;

  J2cAttributeCode *code = NULL;
  for (gint i = 0; i < j2c_object_array_length(method_attributes); i++)
    {
      gpointer attr = j2c_object_array_get (method_attributes, i);
      if (J2C_IS_ATTRIBUTE_CODE (attr))
        code = attr;
      else
        g_object_unref (attr);
    }
  if (!code)
    {
      // set error
      goto end;
    }

  g_object_get_property (G_OBJECT (code), J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE, &val);
  exception_table = g_value_dup_object (&val);

  g_object_get_property (G_OBJECT (code), J2C_ATTRIBUTE_PROP_ATTRIBUTES, &val);
  code_attributes = g_value_dup_object (&val);

  g_value_unset (&val);
  g_value_init (&val, G_TYPE_UINT);

  g_object_get_property (G_OBJECT (code), J2C_ATTRIBUTE_PROP_MAX_STACK, &val);
  guint16 max_stack = g_value_get_uint (&val);

  g_object_get_property (G_OBJECT (code), J2C_ATTRIBUTE_PROP_MAX_LOCALS, &val);
  guint16 max_locals = g_value_get_uint (&val);

  g_value_unset (&val);
  g_value_init (&val, G_TYPE_BYTES);

  g_object_get_property (G_OBJECT (code), J2C_ATTRIBUTE_PROP_CODE, &val);
  GBytes *bytes = g_value_get_boxed (&val);
  g_value_unset (&val);

  GInputStream *code_stream = g_memory_input_stream_new_from_bytes (bytes);
  gsize bytes_size = g_bytes_get_size (bytes);
  g_bytes_unref (bytes);

  instructions = g_ptr_array_new ();
  while (g_seekable_tell (G_SEEKABLE (code_stream)) < bytes_size)
    {
      J2cByteInstruction *instruction = j2c_byte_instruction_new_from_stream (code_stream, &tmp_error);
      if (tmp_error)
        {
          for (gint i = 0; i < instructions->len; i ++)
            if (instructions->pdata[i])
              g_object_unref (instructions->pdata[i]);
          g_ptr_array_free (instructions, TRUE);
          goto end;
        }
      else
        {
          g_ptr_array_add (instructions, instruction);
          for (gint i = 0; i < j2c_byte_instruction_operand_size (instruction); i ++)
            g_ptr_array_add (instructions, NULL);
        }
    }

  ret = g_object_new (J2C_TYPE_BYTE_CODE_METHOD,
		      J2C_METHOD_PROP_DEFINING_TYPE, type,
                      "access-flags", access_flags,
                      "name-index", name_index,
                      "descriptor-index", descriptor_index,
                      "method-attributes", method_attributes,
                      "max-stack", max_stack,
                      "max-locals", max_locals,
                      "exception-table", exception_table,
                      "code-attributes", code_attributes,
                      "constant-pool", constant_pool,
                      "instructions", instructions,
                      NULL);
end:
  if (tmp_error)
    g_propagate_error (error, tmp_error);

  if (code)
    g_object_unref (code);
  if (method_attributes)
    g_object_unref (method_attributes);
  if (exception_table)
    g_object_unref (exception_table);
  if (code_attributes)
    g_object_unref (code_attributes);

  return ret;
}
