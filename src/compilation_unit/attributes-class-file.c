#include <j2c/attributes.h>

struct _J2cAttributeSourceFile
{
  GObject parent;
  guint16 source_file_index;
};

struct _J2cAttributeInnerClasses
{
  GObject parent;
  GPtrArray *classes; /* J2cInnerClassInfo[] */
};

struct _J2cInnerClassInfo
{
  GObject parent;
  guint16 inner_class_info_index;
  guint16 outer_class_info_index;
  guint16 inner_name_index;
  guint16 inner_class_access_flags;
};

struct _J2cAttributeEnclosingMethod
{
  GObject parent;
  guint16 class_index;
  guint16 method_index;
};

struct _J2cAttributeSourceDebugExtension
{
  GObject parent;
  gchar *debug_extension;
};

struct _J2cAttributeBootstrapMethods
{
  GObject parent;
  GPtrArray *bootstrap_methods; /* J2cBootstrapMethodInfo[] */
};

struct _J2cBootstrapMethodInfo
{
  GObject parent;
  guint16 bootstrap_method_ref;
  GArray *bootstrap_arguments; /* guint16[num_boostrap_arguments] */
};

G_DEFINE_TYPE (J2cAttributeSourceFile, j2c_attribute_source_file, G_TYPE_OBJECT)
G_DEFINE_TYPE (J2cAttributeInnerClasses, j2c_attribute_inner_classes, G_TYPE_OBJECT)
G_DEFINE_TYPE (J2cAttributeEnclosingMethod, j2c_attribute_enclosing_method, G_TYPE_OBJECT)
G_DEFINE_TYPE (J2cAttributeSourceDebugExtension, j2c_attribute_source_debug_extension, G_TYPE_OBJECT)
G_DEFINE_TYPE (J2cAttributeBootstrapMethods, j2c_attribute_bootstrap_methods, G_TYPE_OBJECT)
G_DEFINE_TYPE (J2cInnerClassInfo, j2c_inner_class_info, G_TYPE_OBJECT)
G_DEFINE_TYPE (J2cBootstrapMethodInfo, j2c_bootstrap_method_info, G_TYPE_OBJECT)

/****
  PROPERTIES
 ****/

enum
{
  PROP_SOURCE_FILE_INDEX = 1,
  PROP_CLASSES,
  PROP_INNER_CLASS_INFO_INDEX,
  PROP_OUTER_CLASS_INFO_INDEX,
  PROP_INNER_NAME_INDEX,
  PROP_INNER_CLASS_ACCESS_FLAGS,
  PROP_CLASS_INDEX,
  PROP_METHOD_INDEX,
  PROP_DEBUG_EXTENSION,
  PROP_BOOTSTRAP_METHODS,
  PROP_BOOTSTRAP_METHOD_REF,
  PROP_BOOTSTRAP_ARGUMENTS
};

static void j2c_attribute_inner_classes_dispose (GObject *object);
static void j2c_attribute_source_debug_extension_dispose (GObject *object);
static void j2c_attribute_bootstrap_methods_dispose (GObject *object);
static void j2c_bootstrap_method_info_dispose (GObject *object);

static void j2c_attribute_source_file_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_inner_classes_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_enclosing_method_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_source_debug_extension_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_bootstrap_methods_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_inner_class_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_bootstrap_method_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void j2c_attribute_source_file_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_inner_classes_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_enclosing_method_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_source_debug_extension_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_bootstrap_methods_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_inner_class_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_bootstrap_method_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/****
  CONSTRUCTION METHODS
 ****/

static void
j2c_attribute_source_file_class_init (J2cAttributeSourceFileClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_source_file_set_property;
  object_class->get_property = j2c_attribute_source_file_get_property;

  g_object_class_install_property (object_class, PROP_SOURCE_FILE_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_SOURCE_FILE_INDEX,
						      J2C_ATTRIBUTE_PROP_SOURCE_FILE_INDEX,
						      "",
						      0, G_MAXINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_inner_classes_class_init (J2cAttributeInnerClassesClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_inner_classes_set_property;
  object_class->get_property = j2c_attribute_inner_classes_get_property;
  object_class->dispose      = j2c_attribute_inner_classes_dispose;

  g_object_class_install_property (object_class, PROP_CLASSES,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_CLASSES,
						      J2C_ATTRIBUTE_PROP_CLASSES,
						      "Classes",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_enclosing_method_class_init (J2cAttributeEnclosingMethodClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_enclosing_method_set_property;
  object_class->get_property = j2c_attribute_enclosing_method_get_property;

  g_object_class_install_property (object_class, PROP_CLASS_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_CLASS_INDEX,
						      J2C_ATTRIBUTE_PROP_CLASS_INDEX,
						      "",
						      0, G_MAXINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_METHOD_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_METHOD_INDEX,
						      J2C_ATTRIBUTE_PROP_METHOD_INDEX,
						      "",
						      0, G_MAXINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_source_debug_extension_class_init (J2cAttributeSourceDebugExtensionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_source_debug_extension_set_property;
  object_class->get_property = j2c_attribute_source_debug_extension_get_property;
  object_class->dispose      = j2c_attribute_source_debug_extension_dispose;

  g_object_class_install_property (object_class, PROP_DEBUG_EXTENSION,
				   g_param_spec_string (J2C_ATTRIBUTE_PROP_DEBUG_EXTENSION,
						      J2C_ATTRIBUTE_PROP_DEBUG_EXTENSION,
						      "",
						      NULL,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_bootstrap_methods_class_init (J2cAttributeBootstrapMethodsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_bootstrap_methods_set_property;
  object_class->get_property = j2c_attribute_bootstrap_methods_get_property;
  object_class->dispose      = j2c_attribute_bootstrap_methods_dispose;

  g_object_class_install_property (object_class, PROP_BOOTSTRAP_METHODS,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_BOOTSTRAP_METHODS,
						      J2C_ATTRIBUTE_PROP_BOOTSTRAP_METHODS,
						      "",
						      G_TYPE_PTR_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_inner_class_info_class_init (J2cInnerClassInfoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_inner_class_info_set_property;
  object_class->get_property = j2c_inner_class_info_get_property;

  g_object_class_install_property (object_class, PROP_INNER_CLASS_INFO_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_INNER_CLASS_INFO_INDEX,
						      J2C_ATTRIBUTE_PROP_INNER_CLASS_INFO_INDEX,
						      "",
						      0, G_MAXINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_OUTER_CLASS_INFO_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_OUTER_CLASS_INFO_INDEX,
						      J2C_ATTRIBUTE_PROP_OUTER_CLASS_INFO_INDEX,
						      "",
						      0, G_MAXINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_INNER_NAME_INDEX,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_INNER_NAME_INDEX,
						      J2C_ATTRIBUTE_PROP_INNER_NAME_INDEX,
						      "",
						      0, G_MAXINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_INNER_CLASS_ACCESS_FLAGS,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_INNER_CLASS_ACCESS_FLAGS,
						      J2C_ATTRIBUTE_PROP_INNER_CLASS_ACCESS_FLAGS,
						      "",
						      0, G_MAXINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_bootstrap_method_info_class_init (J2cBootstrapMethodInfoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_bootstrap_method_info_set_property;
  object_class->get_property = j2c_bootstrap_method_info_get_property;
  object_class->dispose      = j2c_bootstrap_method_info_dispose;

  g_object_class_install_property (object_class, PROP_BOOTSTRAP_METHOD_REF,
				   g_param_spec_uint (J2C_ATTRIBUTE_PROP_BOOTSTRAP_METHOD_REF,
						      J2C_ATTRIBUTE_PROP_BOOTSTRAP_METHOD_REF,
						      "",
						      0, G_MAXINT16,
						      0,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_BOOTSTRAP_ARGUMENTS,
				   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_BOOTSTRAP_ARGUMENTS,
						      J2C_ATTRIBUTE_PROP_BOOTSTRAP_ARGUMENTS,
						      "",
						      G_TYPE_ARRAY,
						      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}


static void j2c_attribute_source_file_init (J2cAttributeSourceFile *self) { return; }
static void j2c_attribute_inner_classes_init (J2cAttributeInnerClasses *self) { return; }
static void j2c_attribute_enclosing_method_init (J2cAttributeEnclosingMethod *self) { return; }
static void j2c_attribute_source_debug_extension_init (J2cAttributeSourceDebugExtension *self) { return; }
static void j2c_attribute_bootstrap_methods_init (J2cAttributeBootstrapMethods *self) { return; }
static void j2c_inner_class_info_init (J2cInnerClassInfo *self) { return; }
static void j2c_bootstrap_method_info_init (J2cBootstrapMethodInfo *self) { return; }

/****
  DISPOSE METHODS
 ****/

static void
j2c_attribute_inner_classes_dispose (GObject *object)
{
  J2cAttributeInnerClasses *self = J2C_ATTRIBUTE_INNER_CLASSES (object);
  if (self->classes)
    {
      g_ptr_array_unref (self->classes);
      self->classes = NULL;
    }
  G_OBJECT_CLASS (j2c_attribute_inner_classes_parent_class)->dispose (object);
}

static void
j2c_attribute_source_debug_extension_dispose (GObject *object)
{
  J2cAttributeSourceDebugExtension *self = J2C_ATTRIBUTE_SOURCE_DEBUG_EXTENSION (object);
  if (self->debug_extension)
    {
      g_free (self->debug_extension);
      self->debug_extension = NULL;
    }
  G_OBJECT_CLASS (j2c_attribute_source_debug_extension_parent_class)->dispose (object);
}

static void
j2c_attribute_bootstrap_methods_dispose (GObject *object)
{
  J2cAttributeBootstrapMethods *self = J2C_ATTRIBUTE_BOOTSTRAP_METHODS (object);
  if (self->bootstrap_methods)
    {
      g_ptr_array_unref (self->bootstrap_methods);
      self->bootstrap_methods = NULL;
    }
  G_OBJECT_CLASS (j2c_attribute_bootstrap_methods_parent_class)->dispose (object);
}

static void
j2c_bootstrap_method_info_dispose (GObject *object)
{
  J2cBootstrapMethodInfo *self = J2C_BOOTSTRAP_METHOD_INFO (object);
  if (self->bootstrap_arguments)
    {
      g_array_unref (self->bootstrap_arguments);
      self->bootstrap_arguments = NULL;
    }
  G_OBJECT_CLASS (j2c_bootstrap_method_info_parent_class)->dispose (object);
}

/****
  SETTERS AND GETTERS
 ****/

static void
j2c_attribute_source_file_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeSourceFile *self = J2C_ATTRIBUTE_SOURCE_FILE (object);
  switch (property_id)
    {
    case PROP_SOURCE_FILE_INDEX:
      self->source_file_index = (guint16) g_value_get_uint (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_attribute_inner_classes_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeInnerClasses *self = J2C_ATTRIBUTE_INNER_CLASSES (object);
  switch (property_id)
    {
    case PROP_CLASSES:
      self->classes = g_value_get_boxed (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_attribute_enclosing_method_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeEnclosingMethod *self = J2C_ATTRIBUTE_ENCLOSING_METHOD (object);
  switch (property_id)
    {
    case PROP_CLASS_INDEX:
      self->class_index = (guint16) g_value_get_uint (value);
      break;
    case PROP_METHOD_INDEX:
      self->method_index = (guint16) g_value_get_uint (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_attribute_source_debug_extension_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeSourceDebugExtension *self = J2C_ATTRIBUTE_SOURCE_DEBUG_EXTENSION (object);
  switch (property_id)
    {
    case PROP_DEBUG_EXTENSION:
      self->debug_extension = g_value_dup_string (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_attribute_bootstrap_methods_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeBootstrapMethods *self = J2C_ATTRIBUTE_BOOTSTRAP_METHODS (object);
  switch (property_id)
    {
    case PROP_BOOTSTRAP_METHODS:
      self->bootstrap_methods = g_value_get_boxed (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_inner_class_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cInnerClassInfo *self = J2C_INNER_CLASS_INFO (object);
  switch (property_id)
    {
    case PROP_INNER_CLASS_INFO_INDEX:
      self->inner_class_info_index = (guint16) g_value_get_uint (value);
      break;
    case PROP_OUTER_CLASS_INFO_INDEX:
      self->outer_class_info_index = (guint16) g_value_get_uint (value);
      break;
    case PROP_INNER_NAME_INDEX:
      self->inner_name_index = (guint16) g_value_get_uint (value);
      break;
    case PROP_INNER_CLASS_ACCESS_FLAGS:
      self->inner_class_access_flags = (guint16) g_value_get_uint (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_bootstrap_method_info_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cBootstrapMethodInfo *self = J2C_BOOTSTRAP_METHOD_INFO (object);
  switch (property_id)
    {
    case PROP_BOOTSTRAP_METHOD_REF:
      self->bootstrap_method_ref = (guint16) g_value_get_uint (value);
      break;
    case PROP_BOOTSTRAP_ARGUMENTS:
      self->bootstrap_arguments = g_value_get_boxed (value);
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_attribute_source_file_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeSourceFile *self = J2C_ATTRIBUTE_SOURCE_FILE (object);
  switch (property_id)
    {
    case PROP_SOURCE_FILE_INDEX:
      g_value_set_uint (value, (guint) self->source_file_index);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_attribute_inner_classes_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeInnerClasses *self = J2C_ATTRIBUTE_INNER_CLASSES (object);
  switch (property_id)
    {
    case PROP_CLASSES:
      g_value_set_boxed (value, self->classes);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_attribute_enclosing_method_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeEnclosingMethod *self = J2C_ATTRIBUTE_ENCLOSING_METHOD (object);
  switch (property_id)
    {
    case PROP_CLASS_INDEX:
      g_value_set_uint (value, (guint) self->class_index);
      break;
    case PROP_METHOD_INDEX:
      g_value_set_uint (value, (guint) self->method_index);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_attribute_source_debug_extension_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeSourceDebugExtension *self = J2C_ATTRIBUTE_SOURCE_DEBUG_EXTENSION (object);
  switch (property_id)
    {
    case PROP_DEBUG_EXTENSION:
      g_value_set_string (value, self->debug_extension);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_attribute_bootstrap_methods_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeBootstrapMethods *self = J2C_ATTRIBUTE_BOOTSTRAP_METHODS (object);
  switch (property_id)
    {
    case PROP_BOOTSTRAP_METHODS:
      g_value_set_boxed (value, self->bootstrap_methods);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_inner_class_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cInnerClassInfo *self = J2C_INNER_CLASS_INFO (object);
  switch (property_id)
    {
    case PROP_INNER_CLASS_INFO_INDEX:
      g_value_set_uint (value, self->inner_class_info_index);
      break;
    case PROP_OUTER_CLASS_INFO_INDEX:
      g_value_set_uint (value, (guint) self->outer_class_info_index);
      break;
    case PROP_INNER_NAME_INDEX:
      g_value_set_uint (value, (guint) self->inner_name_index);
      break;
    case PROP_INNER_CLASS_ACCESS_FLAGS:
      g_value_set_uint (value, (guint) self->inner_class_access_flags);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_bootstrap_method_info_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cBootstrapMethodInfo *self = J2C_BOOTSTRAP_METHOD_INFO (object);
  switch (property_id)
    {
    case PROP_BOOTSTRAP_METHOD_REF:
      g_value_set_uint (value, (guint) self->bootstrap_method_ref);
      break;
    case PROP_BOOTSTRAP_ARGUMENTS:
      g_value_set_boxed (value, self->bootstrap_arguments);
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}
