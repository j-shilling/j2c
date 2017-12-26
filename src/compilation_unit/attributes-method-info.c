
#include <j2c/attributes.h>

struct _J2cAttributeCode
{
  GObject parent;

  guint16 max_stack;
  guint16 max_locals;

  GByteArray *code;
  GPtrArray *exception_table; /* J2cExceptionInfo[] */
  GPtrArray *attributes; /* J2cAttribute*[] */
};

G_DEFINE_TYPE(J2cAttributeCode, j2c_attribute_code, G_TYPE_OBJECT)

struct _J2cExceptionInfo
{
  GObject parent;

  guint16 start_pc;
  guint16 end_pc;
  guint16 handler_pc;
  guint16 catch_type;
};

G_DEFINE_TYPE (J2cExceptionInfo, j2c_exception_info, G_TYPE_OBJECT)

struct _J2cAttributeExceptions
{
  GObject parent;

  GArray *exception_index_table; /* guint16[] */
};

G_DEFINE_TYPE(J2cAttributeExceptions, j2c_attribute_exceptions, G_TYPE_OBJECT)

struct _J2cAttributeRuntimeVisibleParameterAnnotations
{
  GObject parent;

  GPtrArray *parameter_annotations; /* J2cParameterAnnotation[] */
};

G_DEFINE_TYPE(J2cAttributeRuntimeVisibleParameterAnnotations, j2c_attribute_runtime_visible_parameter_annotations, G_TYPE_OBJECT)

struct _J2cAttributeRuntimeInvisibleParameterAnnotations
{
  GObject parent;

  GPtrArray *parameter_annotations; /* J2cParameterAnnotation[] */
};

G_DEFINE_TYPE(J2cAttributeRuntimeInvisibleParameterAnnotations, j2c_attribute_runtime_invisible_parameter_annotations, G_TYPE_OBJECT)

struct _J2cParameterAnnotations
{
  GObject parent;

  GPtrArray *annotations; /* J2cAnnotation[] */
};

G_DEFINE_TYPE (J2cParameterAnnotations, j2c_parameter_annotations, G_TYPE_OBJECT)

struct _J2cAttributeMethodParameters
{
  GObject parent;

  GPtrArray *parameters; /* J2cParameter[] */
};

G_DEFINE_TYPE (J2cAttributeMethodParameters, j2c_attribute_method_parameters, G_TYPE_OBJECT)

struct _J2cParameter
{
  GObject parent;

  guint16 name_index;
  guint16 access_flags;
};

G_DEFINE_TYPE (J2cParameter, j2c_parameter, G_TYPE_OBJECT)

struct _J2cAttributeAnnotationDefault
{
  GObject parent;

  GBytes *default_value;
};

G_DEFINE_TYPE (J2cAttributeAnnotationDefault, j2c_attribute_annotation_default, G_TYPE_OBJECT)

/****
  PROPERTIES
 ****/

enum
{
  PROP_MAX_STACK = 1,
  PROP_MAX_LOCALS,
  PROP_CODE,
  PROP_EXCEPTION_TABLE,
  PROP_ATTRIBUTES,
  PROP_START_PC,
  PROP_END_PC,
  PROP_HANDLER_PC,
  PROP_CATCH_TYPE,
  PROP_EXCEPTION_INDEX_TABLE,
  PROP_PARAMETER_ANNOTATIONS,
  PROP_ANNOTATIONS,
  PROP_PARAMETERS,
  PROP_NAME_INDEX,
  PROP_ACCESS_FLAGS,
  PROP_DEFAULT_VALUE
};

/****
  PRIVATE METHOD PROTOTYPES
 ****/

static void j2c_attribute_code_dispose (GObject *object);
static void j2c_attribute_exceptions_dispose (GObject *object);
static void j2c_attribute_runtime_visible_parameter_annotations_dispose (GObject *object);
static void j2c_attribute_runtime_invisible_parameter_annotations_dispose (GObject *object);
static void j2c_parameter_annotations_dispose (GObject *object);
static void j2c_attribute_method_parameters_dispose (GObject *object);
static void j2c_attribute_annotation_default_dispose (GObject *object);

static void j2c_attribute_code_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_exception_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_exceptions_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_runtime_visible_parameter_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_runtime_invisible_parameter_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_parameter_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_method_parameters_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_parameter_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_annotation_default_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void j2c_attribute_code_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_exception_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_exceptions_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_runtime_visible_parameter_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_runtime_invisible_parameter_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_parameter_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_method_parameters_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_parameter_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_annotation_default_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/****
  CONSTRUCTION METHODS
 ****/

static void
j2c_attribute_code_class_init (J2cAttributeCodeClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_code_set_property;
  object_class->get_property = j2c_attribute_code_get_property;
  object_class->dispose       = j2c_attribute_code_dispose;

  g_object_class_install_property (object_class, PROP_MAX_STACK,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_MAX_STACK,
						      J2C_ATTRIBUTE_PROP_MAX_STACK,
						      "",
						      0, G_MAXUINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_MAX_LOCALS,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_MAX_LOCALS,
						      J2C_ATTRIBUTE_PROP_MAX_LOCALS,
						      "",
						      0, G_MAXUINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_CODE,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_CODE,
						      J2C_ATTRIBUTE_PROP_CODE,
						      "",
						      G_TYPE_BYTE_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_EXCEPTION_TABLE,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE,
						      J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE,
						      "",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_ATTRIBUTES,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_ATTRIBUTES,
						      J2C_ATTRIBUTE_PROP_ATTRIBUTES,
						      "",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_exception_info_class_init (J2cExceptionInfoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_exception_info_set_property;
  object_class->get_property = j2c_exception_info_get_property;

  g_object_class_install_property (object_class, PROP_START_PC,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_START_PC,
						      J2C_ATTRIBUTE_PROP_START_PC,
						      "",
						      0, G_MAXUINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_END_PC,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_END_PC,
						      J2C_ATTRIBUTE_PROP_END_PC,
						      "",
						      0, G_MAXUINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_HANDLER_PC,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_HANDLER_PC,
						      J2C_ATTRIBUTE_PROP_HANDLER_PC,
						      "",
						      0, G_MAXUINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_CATCH_TYPE,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_CATCH_TYPE,
						      J2C_ATTRIBUTE_PROP_CATCH_TYPE,
						      "",
						      0, G_MAXUINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_exceptions_class_init (J2cAttributeExceptionsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_exceptions_set_property;
  object_class->get_property = j2c_attribute_exceptions_get_property;

  g_object_class_install_property (object_class, PROP_EXCEPTION_INDEX_TABLE,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_EXCEPTION_INDEX_TABLE,
						      J2C_ATTRIBUTE_PROP_EXCEPTION_INDEX_TABLE,
						      "",
						      G_TYPE_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_runtime_visible_parameter_annotations_class_init (J2cAttributeRuntimeVisibleParameterAnnotationsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_runtime_visible_parameter_annotations_set_property;
  object_class->get_property = j2c_attribute_runtime_visible_parameter_annotations_get_property;

  g_object_class_install_property (object_class, PROP_PARAMETER_ANNOTATIONS,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_PARAMETER_ANNOTATIONS,
						      J2C_ATTRIBUTE_PROP_PARAMETER_ANNOTATIONS,
						      "",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_runtime_invisible_parameter_annotations_class_init (J2cAttributeRuntimeInvisibleParameterAnnotationsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_runtime_invisible_parameter_annotations_set_property;
  object_class->get_property = j2c_attribute_runtime_invisible_parameter_annotations_get_property;

  g_object_class_install_property (object_class, PROP_PARAMETER_ANNOTATIONS,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_PARAMETER_ANNOTATIONS,
						      J2C_ATTRIBUTE_PROP_PARAMETER_ANNOTATIONS,
						      "",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_parameter_annotations_class_init (J2cParameterAnnotationsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_parameter_annotations_set_property;
  object_class->get_property = j2c_parameter_annotations_get_property;

  g_object_class_install_property (object_class, PROP_ANNOTATIONS,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_ANNOTATIONS,
						      J2C_ATTRIBUTE_PROP_ANNOTATIONS,
						      "",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_method_parameters_class_init (J2cAttributeMethodParametersClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_method_parameters_set_property;
  object_class->get_property = j2c_attribute_method_parameters_get_property;

  g_object_class_install_property (object_class, PROP_PARAMETERS,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_PARAMETERS,
						      J2C_ATTRIBUTE_PROP_PARAMETERS,
						      "",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_parameter_class_init (J2cParameterClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_parameter_set_property;
  object_class->get_property = j2c_parameter_get_property;

  g_object_class_install_property (object_class, PROP_NAME_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_NAME_INDEX,
						      J2C_ATTRIBUTE_PROP_NAME_INDEX,
						      "",
						      0, G_MAXUINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_ACCESS_FLAGS,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_ACCESS_FLAGS,
						      J2C_ATTRIBUTE_PROP_ACCESS_FLAGS,
						      "",
						      0, G_MAXUINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_annotation_default_class_init (J2cAttributeAnnotationDefaultClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_annotation_default_set_property;
  object_class->get_property = j2c_attribute_annotation_default_get_property;

  g_object_class_install_property (object_class, PROP_DEFAULT_VALUE,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_DEFAULT_VALUE,
						      J2C_ATTRIBUTE_PROP_DEFAULT_VALUE,
						      "",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void j2c_attribute_code_init (J2cAttributeCode* self) { return; }
static void j2c_exception_info_init (J2cExceptionInfo* self) { return; }
static void j2c_attribute_exceptions_init (J2cAttributeExceptions* self) { return; }
static void j2c_attribute_runtime_visible_parameter_annotations_init (J2cAttributeRuntimeVisibleParameterAnnotations* self) { return; }
static void j2c_attribute_runtime_invisible_parameter_annotations_init (J2cAttributeRuntimeInvisibleParameterAnnotations* self) { return; }
static void j2c_parameter_annotations_init (J2cParameterAnnotations* self) { return; }
static void j2c_attribute_method_parameters_init (J2cAttributeMethodParameters* self) { return; }
static void j2c_parameter_init (J2cParameter* self) { return; }
static void j2c_attribute_annotation_default_init (J2cAttributeAnnotationDefault *self) { return; }
