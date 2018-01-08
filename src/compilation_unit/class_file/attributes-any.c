#include <j2c/attributes.h>

struct _J2cAttributeRuntimeVisibleTypeAnnotations
{
  GObject parent;

  GPtrArray *type_annotations; /* J2cTypeAnnotation[] */
};

G_DEFINE_TYPE(J2cAttributeRuntimeVisibleTypeAnnotations, j2c_attribute_runtime_visible_type_annotations, G_TYPE_OBJECT)

struct _J2cAttributeRuntimeInvisibleTypeAnnotations
{
  GObject parent;

  GPtrArray *type_annotations; /* J2cTypeAnnotation[] */
};

G_DEFINE_TYPE(J2cAttributeRuntimeInvisibleTypeAnnotations, j2c_attribute_runtime_invisible_type_annotations, G_TYPE_OBJECT)

struct _J2cTAnnotation
{
  GObject parent;

  J2cTargetInfo *target_info;
  guint16 type_index;
  J2cTPath *type_path;
  GPtrArray *element_value_pairs; /* J2cElementValuePair[] */
};

G_DEFINE_TYPE(J2cTAnnotation, j2c_t_annotation, G_TYPE_OBJECT)

struct _J2cTargetInfo
{
  GObject parent;

  guint8 target_type;
  union
    {
      struct
        {
          guint8 type_parameter_index;
        } type_parameter_target;

      struct
        {
          guint16 supertype_index;
        } supertype_target;

      struct
        {
          guint8 type_parameter_index;
          guint8 bound_index;
        } type_parameter_bound_target;

      struct
        {
          guint8 formal_parameter_index;
        } formal_parameter_target;

      struct
        {
          guint16 throws_type_index;
        } throws_target;

      GPtrArray *localvar_target; /* J2cLocalvar[] */

      struct
        {
          guint16 exception_table_index;
        } catch_target;

      struct
        {
          guint16 offset;
        } offset_target;

      struct
        {
          guint16 offset;
          guint8 type_argument_index;
        } type_argument_target;
    };
};

G_DEFINE_TYPE (J2cTargetInfo, j2c_target_info, G_TYPE_OBJECT)

struct _J2cLocalvar
{
  GObject parent;
  guint16 start_pc;
  guint16 length;
  guint16 index;
};

G_DEFINE_TYPE(J2cLocalvar, j2c_localvar, G_TYPE_OBJECT)

struct _J2cTPath
{
  GObject parent;
  GPtrArray *path; /* J2cPath[] */
};

G_DEFINE_TYPE (J2cTPath, j2c_t_path, G_TYPE_OBJECT)

struct _J2cPath
{
  GObject parent;
  guint8 type_path_kind;
  guint8 type_argument_index;
};

G_DEFINE_TYPE (J2cPath, j2c_path, G_TYPE_OBJECT)

enum
{
  PROP_TYPE_ANNOTATIONS = 1,
  PROP_TARGET_TYPE,
  PROP_SUPERTYPE_INDEX,
  PROP_TYPE_PARAMETER_INDEX,
  PROP_BOUND_INDEX,
  PROP_FORMAL_PARAMETER_INDEX,
  PROP_THROWS_TYPE_INDEX,
  PROP_LOCALVAR_TARGET,
  PROP_EXCEPTION_TABLE_INDEX,
  PROP_OFFSET,
  PROP_TYPE_ARGUMENT_INDEX,
  PROP_TYPE_PATH,
  PROP_ELEMENT_VALUE_PAIRS,
  PROP_LENGTH,
  PROP_START_PC,
  PROP_INDEX,
  PROP_PATH,
  PROP_TYPE_PATH_KIND,
  PROP_TARGET_INFO,
  PROP_TYPE_INDEX
};

/****
  PRIVATE METHOD PROTOTYPES
 ****/

static void j2c_attribute_runtime_visible_type_annotations_dispose (GObject *object);
static void j2c_attribute_runtime_invisible_type_annotations_dispose (GObject *object);
static void j2c_t_annotation_dispose (GObject *object);
static void j2c_t_path_dispose (GObject *object);
static void j2c_target_info_dispose (GObject *object);

static void j2c_attribute_runtime_visible_type_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_runtime_invisible_type_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_t_annotation_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_localvar_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_t_path_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_path_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_target_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void j2c_attribute_runtime_visible_type_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_runtime_invisible_type_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_t_annotation_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_localvar_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_t_path_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_path_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_target_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/****
  CONSTRUCTION METHODS
 ****/

static void
j2c_attribute_runtime_visible_type_annotations_class_init (J2cAttributeRuntimeVisibleTypeAnnotationsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_runtime_visible_type_annotations_set_property;
  object_class->get_property = j2c_attribute_runtime_visible_type_annotations_get_property;
  object_class->dispose      = j2c_attribute_runtime_visible_type_annotations_dispose;

  g_object_class_install_property (object_class, PROP_TYPE_ANNOTATIONS,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_TYPE_ANNOTATIONS,
						      J2C_ATTRIBUTE_PROP_TYPE_ANNOTATIONS,
						      "",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_runtime_invisible_type_annotations_class_init (J2cAttributeRuntimeInvisibleTypeAnnotationsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_runtime_invisible_type_annotations_set_property;
  object_class->get_property = j2c_attribute_runtime_invisible_type_annotations_get_property;
  object_class->dispose      = j2c_attribute_runtime_invisible_type_annotations_dispose;

  g_object_class_install_property (object_class, PROP_TYPE_ANNOTATIONS,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_TYPE_ANNOTATIONS,
						      J2C_ATTRIBUTE_PROP_TYPE_ANNOTATIONS,
						      "",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_t_annotation_class_init (J2cTAnnotationClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_t_annotation_set_property;
  object_class->get_property = j2c_t_annotation_get_property;
  object_class->dispose      = j2c_t_annotation_dispose;

  g_object_class_install_property (object_class, PROP_TARGET_TYPE,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TARGET_TYPE,
						              J2C_ATTRIBUTE_PROP_TARGET_TYPE,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_SUPERTYPE_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_SUPERTYPE_INDEX,
						              J2C_ATTRIBUTE_PROP_SUPERTYPE_INDEX,
						              "",
						              0, G_MAXUINT16,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_TYPE_PARAMETER_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TYPE_PARAMETER_INDEX,
						              J2C_ATTRIBUTE_PROP_TYPE_PARAMETER_INDEX,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_BOUND_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_BOUND_INDEX,
						              J2C_ATTRIBUTE_PROP_BOUND_INDEX,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_FORMAL_PARAMETER_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_FORMAL_PARAMETER_INDEX,
						              J2C_ATTRIBUTE_PROP_FORMAL_PARAMETER_INDEX,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_THROWS_TYPE_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_THROWS_TYPE_INDEX,
						              J2C_ATTRIBUTE_PROP_THROWS_TYPE_INDEX,
						              "",
						              0, G_MAXUINT16,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_LOCALVAR_TARGET,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_LOCALVAR_TARGET,
						               J2C_ATTRIBUTE_PROP_LOCALVAR_TARGET,
						               "",
						               G_TYPE_PTR_ARRAY,
						               G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_EXCEPTION_TABLE_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE_INDEX,
						              J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE_INDEX,
						              "",
						              0, G_MAXUINT16,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_OFFSET,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_OFFSET,
						              J2C_ATTRIBUTE_PROP_OFFSET,
						              "",
						              0, G_MAXUINT16,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_TYPE_ARGUMENT_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TYPE_ARGUMENT_INDEX,
						              J2C_ATTRIBUTE_PROP_TYPE_ARGUMENT_INDEX,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_TYPE_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TYPE_INDEX,
						              J2C_ATTRIBUTE_PROP_TYPE_INDEX,
						              "",
						              0, G_MAXUINT16,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_TARGET_INFO,
				   g_param_spec_object (J2C_ATTRIBUTE_PROP_TARGET_INFO,
						                J2C_ATTRIBUTE_PROP_TARGET_INFO,
						                "",
						                J2C_TYPE_TARGET_INFO,
						                G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_TYPE_PATH,
				   g_param_spec_object (J2C_ATTRIBUTE_PROP_TARGET_PATH,
						                J2C_ATTRIBUTE_PROP_TARGET_PATH,
						                "",
						                J2C_TYPE_T_PATH,
						                G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_ELEMENT_VALUE_PAIRS,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_ELEMENT_VALUE_PAIRS,
						               J2C_ATTRIBUTE_PROP_ELEMENT_VALUE_PAIRS,
						               "",
						               G_TYPE_PTR_ARRAY,
						               G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_target_info_class_init (J2cTargetInfoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_target_info_set_property;
  object_class->get_property = j2c_target_info_get_property;
  object_class->dispose      = j2c_target_info_dispose;

  g_object_class_install_property (object_class, PROP_TARGET_TYPE,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TARGET_TYPE,
						              J2C_ATTRIBUTE_PROP_TARGET_TYPE,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_SUPERTYPE_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_SUPERTYPE_INDEX,
						              J2C_ATTRIBUTE_PROP_SUPERTYPE_INDEX,
						              "",
						              0, G_MAXUINT16,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_TYPE_PARAMETER_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TYPE_PARAMETER_INDEX,
						              J2C_ATTRIBUTE_PROP_TYPE_PARAMETER_INDEX,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_BOUND_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_BOUND_INDEX,
						              J2C_ATTRIBUTE_PROP_BOUND_INDEX,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_FORMAL_PARAMETER_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_FORMAL_PARAMETER_INDEX,
						              J2C_ATTRIBUTE_PROP_FORMAL_PARAMETER_INDEX,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_THROWS_TYPE_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_THROWS_TYPE_INDEX,
						              J2C_ATTRIBUTE_PROP_THROWS_TYPE_INDEX,
						              "",
						              0, G_MAXUINT16,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_LOCALVAR_TARGET,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_LOCALVAR_TARGET,
						               J2C_ATTRIBUTE_PROP_LOCALVAR_TARGET,
						               "",
						               G_TYPE_PTR_ARRAY,
						               G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_EXCEPTION_TABLE_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE_INDEX,
						              J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE_INDEX,
						              "",
						              0, G_MAXUINT16,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_OFFSET,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_OFFSET,
						              J2C_ATTRIBUTE_PROP_OFFSET,
						              "",
						              0, G_MAXUINT16,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_TYPE_ARGUMENT_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TYPE_ARGUMENT_INDEX,
						              J2C_ATTRIBUTE_PROP_TYPE_ARGUMENT_INDEX,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_localvar_class_init (J2cLocalvarClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_localvar_set_property;
  object_class->get_property = j2c_localvar_get_property;

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

  g_object_class_install_property (object_class, PROP_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_INDEX,
						              J2C_ATTRIBUTE_PROP_INDEX,
						              "",
						              0, G_MAXUINT16,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_t_path_class_init (J2cTPathClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_t_path_set_property;
  object_class->get_property = j2c_t_path_get_property;
  object_class->dispose      = j2c_t_path_dispose;

  g_object_class_install_property (object_class, PROP_PATH,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_PATH,
						               J2C_ATTRIBUTE_PROP_PATH,
						               "",
                                       G_TYPE_PTR_ARRAY,
						               G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_path_class_init (J2cPathClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_path_set_property;
  object_class->get_property = j2c_path_get_property;

  g_object_class_install_property (object_class, PROP_TYPE_PATH_KIND,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TYPE_PATH_KIND,
						              J2C_ATTRIBUTE_PROP_TYPE_PATH_KIND,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_TYPE_ARGUMENT_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TYPE_ARGUMENT_INDEX,
						              J2C_ATTRIBUTE_PROP_TYPE_ARGUMENT_INDEX,
						              "",
						              0, G_MAXUINT8,
						              0,
						              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void j2c_attribute_runtime_visible_type_annotations_init (J2cAttributeRuntimeVisibleTypeAnnotations* self) { return; }
static void j2c_attribute_runtime_invisible_type_annotations_init (J2cAttributeRuntimeInvisibleTypeAnnotations* self) { return; }
static void j2c_t_annotation_init (J2cTAnnotation *self) { return; }
static void j2c_localvar_init (J2cLocalvar *self) { return; }
static void j2c_t_path_init (J2cTPath *self) { return; }
static void j2c_path_init (J2cPath *self) { return; }
static void j2c_target_info_init (J2cTargetInfo *self) { return; }

/****
  DESTRUCTION METHODS
 ****/

static void
j2c_attribute_runtime_visible_type_annotations_dispose (GObject *object)
{
  J2cAttributeRuntimeVisibleTypeAnnotations *self = J2C_ATTRIBUTE_RUNTIME_VISIBLE_TYPE_ANNOTATIONS (object);

  if (self->type_annotations)
    {
      g_ptr_array_unref (self->type_annotations);
      self->type_annotations = NULL;
    }

  G_OBJECT_CLASS (j2c_attribute_runtime_visible_type_annotations_parent_class)->dispose (object);
}

static void
j2c_attribute_runtime_invisible_type_annotations_dispose (GObject *object)
{
  J2cAttributeRuntimeInvisibleTypeAnnotations *self = J2C_ATTRIBUTE_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS (object);

  if (self->type_annotations)
    {
      g_ptr_array_unref (self->type_annotations);
      self->type_annotations = NULL;
    }

  G_OBJECT_CLASS (j2c_attribute_runtime_invisible_type_annotations_parent_class)->dispose (object);
}

static void
j2c_t_annotation_dispose (GObject *object)
{
  J2cTAnnotation *self = J2C_T_ANNOTATION (object);

  if (self->element_value_pairs)
    {
      g_ptr_array_unref (self->element_value_pairs);
      self->element_value_pairs = NULL;
    }

  if (self->type_path)
    g_clear_object (&self->type_path);

  if (self->target_info)
    g_clear_object (&self->target_info);

  G_OBJECT_CLASS (j2c_t_annotation_parent_class)->dispose (object);
}


static void
j2c_t_path_dispose (GObject *object)
{
  J2cTPath *self = J2C_T_PATH (object);

  if (self->path)
    {
      g_ptr_array_unref (self->path);
      self->path = NULL;
    }

  G_OBJECT_CLASS (j2c_t_path_parent_class)->dispose (object);
}

static void
j2c_target_info_dispose (GObject *object)
{
  J2cTargetInfo *self = J2C_TARGET_INFO (object);

  if (self->target_type == 0x40 || self->target_type == 0x41)
    {
      if (self->localvar_target)
        {
          g_ptr_array_unref (self->localvar_target);
          self->localvar_target = NULL;
        }
    }

  G_OBJECT_CLASS (j2c_target_info_parent_class)->dispose (object);
}

/****
  SETTERS AND GETTERS
 ****/

static void
j2c_attribute_runtime_visible_type_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeRuntimeVisibleTypeAnnotations *self = J2C_ATTRIBUTE_RUNTIME_VISIBLE_TYPE_ANNOTATIONS (object);

  switch (property_id)
    {
    case PROP_TYPE_ANNOTATIONS:
      if (self->type_annotations)
	{
	  g_ptr_array_unref (self->type_annotations);
	  self->type_annotations = NULL;
	}
      self->type_annotations = g_value_get_boxed (value);
      break;
  default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_attribute_runtime_invisible_type_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeRuntimeInvisibleTypeAnnotations *self = J2C_ATTRIBUTE_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS (object);

  switch (property_id)
    {
    case PROP_TYPE_ANNOTATIONS:
      if (self->type_annotations)
	{
	  g_ptr_array_unref (self->type_annotations);
	  self->type_annotations = NULL;
	}
      self->type_annotations = g_value_get_boxed (value);
      break;
  default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_t_annotation_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cTAnnotation *self = J2C_T_ANNOTATION (object);

  switch (property_id)
    {
      case PROP_TARGET_TYPE:
      case PROP_SUPERTYPE_INDEX:
      case PROP_TYPE_PARAMETER_INDEX:
      case PROP_BOUND_INDEX:
      case PROP_FORMAL_PARAMETER_INDEX:
      case PROP_THROWS_TYPE_INDEX:
      case PROP_LOCALVAR_TARGET:
      case PROP_EXCEPTION_TABLE_INDEX:
      case PROP_OFFSET:
        if (self->target_info)
          {
            j2c_target_info_set_property (G_OBJECT (self->target_info), property_id, value, pspec);
          }
        else
          {
            g_warning ("Cannot set a property on a null J2cTargetInfo");
          }
        break;
      case PROP_TYPE_INDEX:
        self->type_index = g_value_get_uint (value);
        break;
      case PROP_TARGET_INFO:
        if (self->target_info)
          g_object_unref (self->target_info);
        self->target_info = g_value_dup_object (value);
        break;
      case PROP_TYPE_PATH:
        if (self->type_path)
          g_object_unref (self->type_path);
        self->type_path = g_value_dup_object (value);
        break;
      case PROP_ELEMENT_VALUE_PAIRS:
        if (self->element_value_pairs)
          g_ptr_array_unref (self->element_value_pairs);
        self->element_value_pairs = g_value_get_boxed (value);
      default:
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_target_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cTargetInfo *self = J2C_TARGET_INFO (object);

  if (property_id == PROP_TARGET_TYPE)
    {
      self->target_type = g_value_get_uint (value);
      return;
    }

  switch (self->target_type)
    {
      case 0x00:
      case 0x01: /* type_parameter_target */
          switch (property_id)
            {
              case PROP_TYPE_PARAMETER_INDEX:
                self->type_parameter_target.type_parameter_index = g_value_get_uint (value);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x10: /* supertype_target */
          switch (property_id)
            {
              case PROP_SUPERTYPE_INDEX:
                self->supertype_target.supertype_index = g_value_get_uint (value);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x11:
      case 0x12: /* type_parameter_bound_target */
          switch (property_id)
            {
              case PROP_TYPE_PARAMETER_INDEX:
                self->type_parameter_bound_target.type_parameter_index = g_value_get_uint (value);
                break;
              case PROP_BOUND_INDEX:
                self->type_parameter_bound_target.bound_index = g_value_get_uint (value);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x13:
      case 0x14:
      case 0x15: /* empty_target */
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
      case 0x16: /* formal_parameter_target */
          switch (property_id)
            {
              case PROP_FORMAL_PARAMETER_INDEX:
                self->formal_parameter_target.formal_parameter_index = g_value_get_uint (value);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x17: /* throws_target */
          switch (property_id)
            {
              case PROP_THROWS_TYPE_INDEX:
                self->throws_target.throws_type_index = g_value_get_uint (value);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x40:
      case 0x41: /* localvar_target */
          switch (property_id)
            {
              case PROP_LOCALVAR_TARGET:
                if (self->localvar_target)
                  g_ptr_array_unref (self->localvar_target);
                self->localvar_target = g_value_get_boxed (value);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x42: /* catch_target */
          switch (property_id)
            {
              case PROP_EXCEPTION_TABLE_INDEX:
                self->catch_target.exception_table_index = g_value_get_uint (value);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x43:
      case 0x44:
      case 0x45:
      case 0x46: /* offset_target */
          switch (property_id)
            {
              case PROP_OFFSET:
                self->offset_target.offset = g_value_get_uint (value);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x47:
      case 0x48:
      case 0x49:
      case 0x4A:
      case 0x4B: /* type_argument_target */
          switch (property_id)
            {
              case PROP_OFFSET:
                self->type_argument_target.offset = g_value_get_uint (value);
                break;
              case PROP_TYPE_ARGUMENT_INDEX:
                self->type_argument_target.type_argument_index = g_value_get_uint (value);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      default:
        g_warning ("Cannot interpret the union in J2cTargetInfo based on the value of target_type");
    }
}

static void
j2c_localvar_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cLocalvar *self = J2C_LOCALVAR (object);

  switch (property_id)
    {
      case PROP_START_PC:
        self->start_pc = g_value_get_uint (value);
        break;
      case PROP_LENGTH:
        self->length = g_value_get_uint (value);
        break;
      case PROP_INDEX:
        self->index = g_value_get_uint (value);
        break;
      default:
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_t_path_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cTPath *self = J2C_T_PATH (object);

  switch (property_id)
    {
      case PROP_PATH:
        if (self->path)
          g_ptr_array_unref (self->path);
        self->path = g_value_get_boxed (value);
        break;
      default:
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_path_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cPath *self = J2C_PATH (object);

  switch (property_id)
    {
      case PROP_TYPE_PATH_KIND:
        self->type_path_kind = g_value_get_uint (value);
        break;
      case PROP_TYPE_ARGUMENT_INDEX:
        self->type_argument_index = g_value_get_uint (value);
        break;
      default:
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_attribute_runtime_visible_type_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeRuntimeVisibleTypeAnnotations *self = J2C_ATTRIBUTE_RUNTIME_VISIBLE_TYPE_ANNOTATIONS (object);

  switch (property_id)
    {
    case PROP_TYPE_ANNOTATIONS:
      g_value_set_boxed (value, self->type_annotations);
      break;
  default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_attribute_runtime_invisible_type_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeRuntimeInvisibleTypeAnnotations *self = J2C_ATTRIBUTE_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS (object);

  switch (property_id)
    {
    case PROP_TYPE_ANNOTATIONS:
      g_value_set_boxed (value, self->type_annotations);
      break;
  default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_t_annotation_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cTAnnotation *self = J2C_T_ANNOTATION (object);

  switch (property_id)
    {
      case PROP_TARGET_TYPE:
      case PROP_SUPERTYPE_INDEX:
      case PROP_TYPE_PARAMETER_INDEX:
      case PROP_BOUND_INDEX:
      case PROP_FORMAL_PARAMETER_INDEX:
      case PROP_THROWS_TYPE_INDEX:
      case PROP_LOCALVAR_TARGET:
      case PROP_EXCEPTION_TABLE_INDEX:
      case PROP_OFFSET:
        if (self->target_info)
          {
            j2c_target_info_get_property (G_OBJECT (self->target_info), property_id, value, pspec);
          }
        else
          {
            g_warning ("Cannot get a property from a null J2cTargetInfo");
          }
        break;
      case PROP_TARGET_INFO:
        g_value_set_object (value, self->target_info);
        break;
      case PROP_TYPE_INDEX:
        g_value_set_uint (value, self->type_index);
        break;
      case PROP_TYPE_PATH:
        g_value_set_object (value, self->type_path);
        break;
      case PROP_ELEMENT_VALUE_PAIRS:
        g_value_set_boxed (value, self->element_value_pairs);
      default:
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}


static void
j2c_localvar_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cLocalvar *self = J2C_LOCALVAR (object);

  switch (property_id)
    {
      case PROP_START_PC:
        g_value_set_uint (value, self->start_pc);
        break;
      case PROP_LENGTH:
        g_value_set_uint (value, self->length);
        break;
      case PROP_INDEX:
        g_value_set_uint (value, self->index);
        break;
      default:
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_t_path_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cTPath *self = J2C_T_PATH (object);

  switch (property_id)
    {
      case PROP_PATH:
        g_value_set_boxed (value, self->path);
        break;
      default:
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_path_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cPath *self = J2C_PATH (object);

  switch (property_id)
    {
      case PROP_TYPE_PATH_KIND:
        g_value_set_uint (value, self->type_path_kind);
        break;
      case PROP_TYPE_ARGUMENT_INDEX:
        g_value_set_uint (value, self->type_argument_index);
        break;
      default:
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_target_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cTargetInfo *self = J2C_TARGET_INFO (object);

  if (property_id == PROP_TARGET_TYPE)
    {
      g_value_set_uint (value, self->target_type);
      return;
    }

  switch (self->target_type)
    {
      case 0x00:
      case 0x01: /* type_parameter_target */
          switch (property_id)
            {
              case PROP_TYPE_PARAMETER_INDEX:
                g_value_set_uint (value, self->type_parameter_target.type_parameter_index);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x10: /* supertype_target */
          switch (property_id)
            {
              case PROP_SUPERTYPE_INDEX:
                g_value_set_uint (value, self->supertype_target.supertype_index);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x11:
      case 0x12: /* type_parameter_bound_target */
          switch (property_id)
            {
              case PROP_TYPE_PARAMETER_INDEX:
                g_value_set_uint (value, self->type_parameter_bound_target.type_parameter_index);
                break;
              case PROP_BOUND_INDEX:
                g_value_set_uint (value, self->type_parameter_bound_target.bound_index);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x13:
      case 0x14:
      case 0x15: /* empty_target */
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
      case 0x16: /* formal_parameter_target */
          switch (property_id)
            {
              case PROP_FORMAL_PARAMETER_INDEX:
                g_value_set_uint (value, self->formal_parameter_target.formal_parameter_index);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x17: /* throws_target */
          switch (property_id)
            {
              case PROP_THROWS_TYPE_INDEX:
                g_value_set_uint (value, self->throws_target.throws_type_index);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x40:
      case 0x41: /* localvar_target */
          switch (property_id)
            {
              case PROP_LOCALVAR_TARGET:
                g_value_set_boxed (value, self->localvar_target);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x42: /* catch_target */
          switch (property_id)
            {
              case PROP_EXCEPTION_TABLE_INDEX:
                g_value_set_uint (value, self->catch_target.exception_table_index);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x43:
      case 0x44:
      case 0x45:
      case 0x46: /* offset_target */
          switch (property_id)
            {
              case PROP_OFFSET:
                g_value_set_uint (value, self->offset_target.offset);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      case 0x47:
      case 0x48:
      case 0x49:
      case 0x4A:
      case 0x4B: /* type_argument_target */
          switch (property_id)
            {
              case PROP_OFFSET:
                g_value_set_uint (value, self->type_argument_target.offset);
                break;
              case PROP_TYPE_ARGUMENT_INDEX:
                g_value_set_uint (value, self->type_argument_target.type_argument_index);
                break;
              default:
                  G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            }
        break;
      default:
        g_warning ("Cannot interpret the union in J2cTargetInfo based on the value of target_type");
    }
}
