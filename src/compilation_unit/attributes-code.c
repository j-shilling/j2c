#include <j2c/attributes.h>

struct _J2cAttributeLineNumberTable
{
  GObject parent;

  GPtrArray *line_number_table; /* J2cLineNumber[] */
};

G_DEFINE_TYPE(J2cAttributeLineNumberTable, j2c_attribute_line_number_table, G_TYPE_OBJECT)

struct _J2cLineNumber
{
  GObject parent;

  guint16 start_pc;
  guint16 line_number;
};

G_DEFINE_TYPE (J2cLineNumber, j2c_line_number, G_TYPE_OBJECT)

struct _J2cAttributeLocalVariableTable
{
  GObject parent;

  GPtrArray *local_variable_table; /* J2cLocalVariable[] */
};

G_DEFINE_TYPE(J2cAttributeLocalVariableTable, j2c_attribute_local_variable_table, G_TYPE_OBJECT)

struct _J2cLocalVariable
{
  GObject parent;

  guint16 start_pc;
  guint16 length;
  guint16 name_index;
  guint16 descriptor_index;
  guint16 index;
};

G_DEFINE_TYPE(J2cLocalVariable, j2c_local_variable, G_TYPE_OBJECT)

struct _J2cAttributeLocalVariableTypeTable
{
  GObject parent;

  GPtrArray *local_variable_type_table; /* J2cLocalVariableType[] */
};

G_DEFINE_TYPE(J2cAttributeLocalVariableTypeTable, j2c_attribute_local_variable_type_table, G_TYPE_OBJECT)

struct _J2cLocalVariableType
{
  GObject parent;

  guint16 start_pc;
  guint16 length;
  guint16 name_index;
  guint16 signature_index;
  guint16 index;
};

G_DEFINE_TYPE(J2cLocalVariableType, j2c_local_variable_type, G_TYPE_OBJECT)

struct _J2cAttributeStackMapTable
{
  GObject parent;

  GPtrArray *entries; /* J2cStackMapFrame[] */
};

G_DEFINE_TYPE(J2cAttributeStackMapTable, j2c_attribute_stack_map_table, G_TYPE_OBJECT)

struct _J2cStackMapFrame
{
  GObject parent;

  guint8 tag;
  guint16 offset_delta;
  GPtrArray *locals; /* J2cVerificationTypeInfo[] */
  GPtrArray *stack; /* J2cVerificationTypeInfo[] */
};

G_DEFINE_TYPE(J2cStackMapFrame, j2c_stack_map_frame, G_TYPE_OBJECT)

struct _J2cVerificationTypeInfo
{
  GObject parent;
  guint8 tag;
  union
    {
      guint16 cpool_index;
      guint16 offset;
    };
};

G_DEFINE_TYPE(J2cVerificationTypeInfo, j2c_verification_type_info, G_TYPE_OBJECT)

enum
  {
    PROP_LINE_NUMBER_TABLE = 1,
    PROP_START_PC,
    PROP_LINE_NUMBER,
    PROP_LOCAL_VARIABLE_TABLE,
    PROP_LENGTH,
    PROP_NAME_INDEX,
    PROP_DESCRIPTOR_INDEX,
    PROP_INDEX,
    PROP_SIGNATURE_INDEX,
    PROP_LOCAL_VARIABLE_TYPE_TABLE,
    PROP_ENTRIES,
    PROP_TAG,
    PROP_OFFSET_DELTA,
    PROP_LOCALS,
    PROP_STACK,
    PROP_CPOOL_INDEX,
    PROP_OFFSET
  };

/****
  PRIVATE METHOD PROTOTYPES
 ****/

static void j2c_attribute_line_number_table_dispose (GObject *object);
static void j2c_attribute_local_variable_table_dispose (GObject *object);
static void j2c_attribute_local_variable_type_table_dispose (GObject *object);
static void j2c_attribute_stack_map_table_dispose (GObject *object);
static void j2c_stack_map_frame_dispose (GObject *object);

static void j2c_attribute_line_number_table_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_line_number_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_local_variable_table_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_local_variable_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_local_variable_type_table_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_local_variable_type_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_stack_map_table_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_stack_map_frame_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_verification_type_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void j2c_attribute_line_number_table_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_line_number_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_local_variable_table_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_local_variable_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_local_variable_type_table_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_local_variable_type_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_stack_map_table_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_stack_map_frame_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_verification_type_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/****
  CONSTRUCTION METHODS
 ****/

static void
j2c_attribute_line_number_table_class_init (J2cAttributeLineNumberTableClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_line_number_table_set_property;
  object_class->get_property = j2c_attribute_line_number_table_get_property;
  object_class->dispose      = j2c_attribute_line_number_table_dispose;

  g_object_class_install_property (object_class, PROP_LINE_NUMBER_TABLE,
                                   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_LINE_NUMBER_TABLE,
                                                       J2C_ATTRIBUTE_PROP_LINE_NUMBER_TABLE,
                                                       "",
                                                       G_TYPE_PTR_ARRAY,
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_line_number_class_init (J2cLineNumberClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_line_number_set_property;
  object_class->get_property = j2c_line_number_get_property;

  g_object_class_install_property (object_class, PROP_START_PC,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_START_PC,
                                                      J2C_ATTRIBUTE_PROP_START_PC,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_LINE_NUMBER,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_LINE_NUMBER,
                                                      J2C_ATTRIBUTE_PROP_LINE_NUMBER,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_local_variable_table_class_init (J2cAttributeLocalVariableTableClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_local_variable_table_set_property;
  object_class->get_property = j2c_attribute_local_variable_table_get_property;
  object_class->dispose      = j2c_attribute_local_variable_table_dispose;

  g_object_class_install_property (object_class, PROP_LOCAL_VARIABLE_TABLE,
                                   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_LOCAL_VARIABLE_TABLE,
                                                       J2C_ATTRIBUTE_PROP_LOCAL_VARIABLE_TABLE,
                                                       "",
                                                       G_TYPE_PTR_ARRAY,
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_local_variable_class_init (J2cLocalVariableClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_local_variable_set_property;
  object_class->get_property = j2c_local_variable_get_property;

  g_object_class_install_property (object_class, PROP_START_PC,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_START_PC,
                                                      J2C_ATTRIBUTE_PROP_START_PC,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_LENGTH,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_LENGTH,
                                                      J2C_ATTRIBUTE_PROP_LENGTH,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_NAME_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_NAME_INDEX,
                                                      J2C_ATTRIBUTE_PROP_NAME_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_DESCRIPTOR_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_DESCRIPTOR_INDEX,
                                                      J2C_ATTRIBUTE_PROP_DESCRIPTOR_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_INDEX,
                                                      J2C_ATTRIBUTE_PROP_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_local_variable_type_table_class_init (J2cAttributeLocalVariableTypeTableClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_local_variable_type_table_set_property;
  object_class->get_property = j2c_attribute_local_variable_type_table_get_property;
  object_class->dispose      = j2c_attribute_local_variable_type_table_dispose;

  g_object_class_install_property (object_class, PROP_LOCAL_VARIABLE_TYPE_TABLE,
                                   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_LOCAL_VARIABLE_TYPE_TABLE,
                                                       J2C_ATTRIBUTE_PROP_LOCAL_VARIABLE_TYPE_TABLE,
                                                       "",
                                                       G_TYPE_PTR_ARRAY,
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_local_variable_type_class_init (J2cLocalVariableTypeClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_local_variable_type_set_property;
  object_class->get_property = j2c_local_variable_type_get_property;

  g_object_class_install_property (object_class, PROP_START_PC,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_START_PC,
                                                      J2C_ATTRIBUTE_PROP_START_PC,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_LENGTH,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_LENGTH,
                                                      J2C_ATTRIBUTE_PROP_LENGTH,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_NAME_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_NAME_INDEX,
                                                      J2C_ATTRIBUTE_PROP_NAME_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_SIGNATURE_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_SIGNATURE_INDEX,
                                                      J2C_ATTRIBUTE_PROP_SIGNATURE_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_INDEX,
                                                      J2C_ATTRIBUTE_PROP_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_stack_map_table_class_init (J2cAttributeStackMapTableClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_stack_map_table_set_property;
  object_class->get_property = j2c_attribute_stack_map_table_get_property;
  object_class->dispose      = j2c_attribute_stack_map_table_dispose;

  g_object_class_install_property (object_class, PROP_ENTRIES,
                                   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_ENTRIES,
                                                       J2C_ATTRIBUTE_PROP_ENTRIES,
                                                       "",
                                                       G_TYPE_PTR_ARRAY,
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_stack_map_frame_class_init (J2cStackMapFrameClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_stack_map_frame_set_property;
  object_class->get_property = j2c_stack_map_frame_get_property;
  object_class->dispose      = j2c_stack_map_frame_dispose;

  g_object_class_install_property (object_class, PROP_TAG,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TAG,
                                                      J2C_ATTRIBUTE_PROP_TAG,
                                                      "",
                                                      0, G_MAXUINT8,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_OFFSET_DELTA,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_OFFSET_DELTA,
                                                      J2C_ATTRIBUTE_PROP_OFFSET_DELTA,
                                                      "",
                                                      0, G_MAXUINT8,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_LOCALS,
                                   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_LOCALS,
                                                       J2C_ATTRIBUTE_PROP_LOCALS,
                                                       "",
                                                       G_TYPE_PTR_ARRAY,
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_STACK,
                                   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_STACK,
                                                       J2C_ATTRIBUTE_PROP_STACK,
                                                       "",
                                                       G_TYPE_PTR_ARRAY,
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_verification_type_info_class_init (J2cVerificationTypeInfoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_verification_type_info_set_property;
  object_class->get_property = j2c_verification_type_info_get_property;

  g_object_class_install_property (object_class, PROP_TAG,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TAG,
                                                      J2C_ATTRIBUTE_PROP_TAG,
                                                      "",
                                                      0, G_MAXUINT8,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_CPOOL_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_CPOOL_INDEX,
                                                      J2C_ATTRIBUTE_PROP_CPOOL_INDEX,
                                                      "",
                                                      0, G_MAXUINT8,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_OFFSET,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_OFFSET,
                                                      J2C_ATTRIBUTE_PROP_OFFSET,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_line_number_table_init (J2cAttributeLineNumberTable *self)
{
  return;
}

static void
j2c_line_number_init (J2cLineNumber *self)
{
  return;
}

static void
j2c_attribute_local_variable_table_init (J2cAttributeLocalVariableTable *self)
{
  return;
}

static void
j2c_local_variable_init (J2cLocalVariable *self)
{
  return;
}

static void
j2c_attribute_local_variable_type_table_init (J2cAttributeLocalVariableTypeTable *self)
{
  return;
}

static void
j2c_local_variable_type_init (J2cLocalVariableType *self)
{
  return;
}

static void
j2c_attribute_stack_map_table_init (J2cAttributeStackMapTable *self)
{
  return;
}

static void
j2c_stack_map_frame_init (J2cStackMapFrame *self)
{
  return;
}

static void
j2c_verification_type_info_init (J2cVerificationTypeInfo *self)
{
  return;
}

/****
  DESTRUCTION METHODS
 ****/

static void
j2c_attribute_line_number_table_dispose (GObject *object)
{
  J2cAttributeLineNumberTable *self = J2C_ATTRIBUTE_LINE_NUMBER_TABLE (object);

  if (self->line_number_table)
    {
      g_ptr_array_unref (self->line_number_table);
      self->line_number_table = NULL;
    }

  G_OBJECT_CLASS (j2c_attribute_line_number_table_parent_class)->dispose (object);
}

static void
j2c_attribute_local_variable_table_dispose (GObject *object)
{
  J2cAttributeLocalVariableTable *self = J2C_ATTRIBUTE_LOCAL_VARIABLE_TABLE (object);

  if (self->local_variable_table)
    {
      g_ptr_array_unref (self->local_variable_table);
      self->local_variable_table = NULL;
    }

  G_OBJECT_CLASS (j2c_attribute_local_variable_table_parent_class)->dispose (object);
}

static void
j2c_attribute_local_variable_type_table_dispose (GObject *object)
{
  J2cAttributeLocalVariableTypeTable *self = J2C_ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE (object);

  if (self->local_variable_type_table)
    {
      g_ptr_array_unref (self->local_variable_type_table);
      self->local_variable_type_table = NULL;
    }

  G_OBJECT_CLASS (j2c_attribute_local_variable_type_table_parent_class)->dispose (object);
}

static void
j2c_attribute_stack_map_table_dispose (GObject *object)
{
  J2cAttributeStackMapTable *self = J2C_ATTRIBUTE_STACK_MAP_TABLE (object);

  if (self->entries)
    {
      g_ptr_array_unref (self->entries);
      self->entries = NULL;
    }

  G_OBJECT_CLASS (j2c_attribute_stack_map_table_parent_class)->dispose (object);
}

static void
j2c_stack_map_frame_dispose (GObject *object)
{
  J2cStackMapFrame *self = J2C_STACK_MAP_FRAME (object);

  if (self->stack)
    {
      g_ptr_array_unref (self->stack);
      self->stack = NULL;
    }
  if (self->locals)
    {
      g_ptr_array_unref (self->locals);
      self->locals = NULL;
    }

  G_OBJECT_CLASS (j2c_stack_map_frame_parent_class)->dispose (object);
}

/****
  SETTERS AND GETTERS
 ****/

static void
j2c_attribute_line_number_table_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeLineNumberTable *self = J2C_ATTRIBUTE_LINE_NUMBER_TABLE (object);

  switch (property_id)
    {
      case PROP_LINE_NUMBER_TABLE:
        if (self->line_number_table)
          g_ptr_array_unref (self->line_number_table);
        self->line_number_table = g_value_get_boxed (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_line_number_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cLineNumber *self = J2C_LINE_NUMBER (object);

  switch (property_id)
    {
      case PROP_START_PC:
        self->start_pc = g_value_get_uint (value);
        break;
      case PROP_LINE_NUMBER:
        self->line_number = g_value_get_uint (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_attribute_local_variable_table_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeLocalVariableTable *self = J2C_ATTRIBUTE_LOCAL_VARIABLE_TABLE (object);

  switch (property_id)
    {
      case PROP_LOCAL_VARIABLE_TABLE:
        if (self->local_variable_table)
          g_ptr_array_unref (self->local_variable_table);
        self->local_variable_table = g_value_get_boxed (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_local_variable_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cLocalVariable *self = J2C_LOCAL_VARIABLE (object);

  switch (property_id)
    {
      case PROP_START_PC:
        self->start_pc = g_value_get_uint (value);
        break;
      case PROP_LENGTH:
        self->length = g_value_get_uint (value);
        break;
      case PROP_NAME_INDEX:
        self->name_index = g_value_get_uint (value);
        break;
      case PROP_DESCRIPTOR_INDEX:
        self->descriptor_index = g_value_get_uint (value);
        break;
      case PROP_INDEX:
        self->index = g_value_get_uint (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_attribute_local_variable_type_table_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeLocalVariableTypeTable *self = J2C_ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE (object);

  switch (property_id)
    {
      case PROP_LOCAL_VARIABLE_TYPE_TABLE:
        if (self->local_variable_type_table)
          g_ptr_array_unref (self->local_variable_type_table);
        self->local_variable_type_table = g_value_get_boxed (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_local_variable_type_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cLocalVariableType *self = J2C_LOCAL_VARIABLE_TYPE (object);

  switch (property_id)
    {
      case PROP_START_PC:
        self->start_pc = g_value_get_uint (value);
        break;
      case PROP_LENGTH:
        self->length = g_value_get_uint (value);
        break;
      case PROP_NAME_INDEX:
        self->name_index = g_value_get_uint (value);
        break;
      case PROP_SIGNATURE_INDEX:
        self->signature_index = g_value_get_uint (value);
        break;
      case PROP_INDEX:
        self->index = g_value_get_uint (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_attribute_stack_map_table_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeStackMapTable *self = J2C_ATTRIBUTE_STACK_MAP_TABLE (object);

  switch (property_id)
    {
      case PROP_ENTRIES:
        if (self->entries)
          g_ptr_array_unref (self->entries);
        self->entries = g_value_get_boxed (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_stack_map_frame_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cStackMapFrame *self = J2C_STACK_MAP_FRAME (object);

  switch (property_id)
    {
      case PROP_TAG:
        self->tag = g_value_get_uint (value);
        break;
      case PROP_OFFSET_DELTA:
        self->offset_delta = g_value_get_uint (value);
        break;
      case PROP_LOCALS:
        if (self->locals)
          g_ptr_array_unref (self->locals);
        self->locals = g_value_get_boxed (value);
        break;
      case PROP_STACK:
        if (self->stack)
          g_ptr_array_unref (self->stack);
        self->stack = g_value_get_boxed (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_verification_type_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cVerificationTypeInfo *self = J2C_VERIFICATION_TYPE_INFO (object);

  switch (property_id)
    {
      case PROP_TAG:
        self->tag = g_value_get_uint (value);
        break;
      case PROP_CPOOL_INDEX:
        self->cpool_index = g_value_get_uint (value);
        break;
      case PROP_OFFSET:
        self->offset = g_value_get_uint (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_attribute_line_number_table_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeLineNumberTable *self = J2C_ATTRIBUTE_LINE_NUMBER_TABLE (object);

  switch (property_id)
    {
      case PROP_LINE_NUMBER_TABLE:
        g_value_set_boxed (value, self->line_number_table);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_line_number_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cLineNumber *self = J2C_LINE_NUMBER (object);

  switch (property_id)
    {
      case PROP_START_PC:
        g_value_set_uint (value, self->start_pc);
        break;
      case PROP_LINE_NUMBER:
        g_value_set_uint (value, self->line_number);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_attribute_local_variable_table_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeLocalVariableTable *self = J2C_ATTRIBUTE_LOCAL_VARIABLE_TABLE (object);

  switch (property_id)
    {
      case PROP_LOCAL_VARIABLE_TABLE:
        g_value_set_boxed (value, self->local_variable_table);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_local_variable_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cLocalVariable *self = J2C_LOCAL_VARIABLE (object);

  switch (property_id)
    {
      case PROP_START_PC:
        g_value_set_uint (value, self->start_pc);
        break;
      case PROP_LENGTH:
        g_value_set_uint (value, self->length);
        break;
      case PROP_NAME_INDEX:
        g_value_set_uint (value, self->name_index);
        break;
      case PROP_DESCRIPTOR_INDEX:
        g_value_set_uint (value, self->descriptor_index);
        break;
      case PROP_INDEX:
        g_value_set_uint (value, self->index);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_attribute_local_variable_type_table_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeLocalVariableTypeTable *self = J2C_ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE (object);

  switch (property_id)
    {
      case PROP_LOCAL_VARIABLE_TYPE_TABLE:
        g_value_set_boxed (value, self->local_variable_type_table);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_local_variable_type_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cLocalVariableType *self = J2C_LOCAL_VARIABLE_TYPE (object);

  switch (property_id)
    {
      case PROP_START_PC:
        g_value_set_uint (value, self->start_pc);
        break;
      case PROP_LENGTH:
        g_value_set_uint (value, self->length);
        break;
      case PROP_NAME_INDEX:
        g_value_set_uint (value, self->name_index);
        break;
      case PROP_SIGNATURE_INDEX:
        g_value_set_uint (value, self->signature_index);
        break;
      case PROP_INDEX:
        g_value_set_uint (value, self->index);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_attribute_stack_map_table_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeStackMapTable *self = J2C_ATTRIBUTE_STACK_MAP_TABLE (object);

  switch (property_id)
    {
      case PROP_ENTRIES:
        g_value_set_boxed (value, self->entries);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_stack_map_frame_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cStackMapFrame *self = J2C_STACK_MAP_FRAME (object);

  switch (property_id)
    {
      case PROP_TAG:
        g_value_set_uint (value, self->tag);
        break;
      case PROP_OFFSET_DELTA:
        g_value_set_uint (value, self->offset_delta);
        break;
      case PROP_LOCALS:
        g_value_set_boxed (value, self->locals);
        break;
      case PROP_STACK:
        g_value_set_boxed (value, self->stack);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_verification_type_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cVerificationTypeInfo *self = J2C_VERIFICATION_TYPE_INFO (object);

  switch (property_id)
    {
      case PROP_TAG:
        g_value_set_uint (value, self->tag);
        break;
      case PROP_CPOOL_INDEX:
        g_value_set_uint (value, self->cpool_index);
        break;
      case PROP_OFFSET:
        g_value_set_uint (value, self->offset);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}
