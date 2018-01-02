#include <j2c/compilation-unit-class.h>
#include <j2c/constant-pool.h>
#include <j2c/method-or-field-info.h>
#include <j2c/attributes.h>
#include <j2c/logger.h>
#include <j2c/object-array.h>

#include <gio/gio.h>

struct _J2cCompilationUnitClass
{
  GObject parent_instance;
  gchar *name;

  guint16 minor_version;
  guint16 major_version;
  J2cConstantPool *constant_pool;
  guint16 access_flags;
  guint16 this_class;
  guint16 super_class;

  GArray    *interfaces;
  J2cObjectArray *fields;
  J2cObjectArray *methods;
  J2cObjectArray *attributes;
};

/* GObjectClass methods */
static void j2c_compilation_unit_class_constructed (GObject *obj);
static void j2c_compilation_unit_class_dispose (GObject *object);
static void j2c_compilation_unit_class_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_compilation_unit_class_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/* J2cCompilationUnit methods */
static void j2c_compilation_unit_interface_init (J2cCompilationUnitInterface *iface);
static gchar const *j2c_compilation_unit_class_name (J2cCompilationUnit *compilation_unit);

G_DEFINE_TYPE_WITH_CODE (J2cCompilationUnitClass, j2c_compilation_unit_class, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (J2C_TYPE_COMPILATION_UNIT, j2c_compilation_unit_interface_init))

/****
  PROPERTIES
 ****/

enum
{
  PROP_MINOR_VERSION = 1,
  PROP_MAJOR_VERSION,
  PROP_CONSTANT_POOL,
  PROP_ACCESS_FLAGS,
  PROP_THIS_CLASS,
  PROP_SUPER_CLASS,
  PROP_INTERFACES,
  PROP_FIELDS,
  PROP_METHODS,
  PROP_ATTRIBUTES,

  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static gchar *MINOR_VERSION = "minor-version";
static gchar *MAJOR_VERSION = "major-version";
static gchar *CONSTANT_POOL = "constant-pool";
static gchar *ACCESS_FLAGS = "access-flags";
static gchar *THIS_CLASS = "this-class";
static gchar *SUPER_CLASS = "super-class";
static gchar *INTERFACES = "interfaces";
static gchar *FIELDS = "fields";
static gchar *METHODS = "methods";
static gchar *ATTRIBUTES = "attributes";

/****
  CONSTRUCTION METHODS
 ****/

J2cCompilationUnitClass *
j2c_compilation_unit_class_new (J2cIndexedFile *file, GError **error)
{
  g_return_val_if_fail (j2c_indexed_file_get_file_type (file) == J2C_FILE_TYPE_CLASS, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  J2cCompilationUnitClass *ret = NULL;
  guint16 minor_version;
  guint16 major_version;
  J2cConstantPool *constant_pool = NULL;
  guint16 access_flags;
  guint16 this_class;
  guint16 super_class;
  guint16 interfaces_count;
  GArray *interfaces = NULL;
  guint16 fields_count;
  J2cObjectArray *fields = NULL;
  guint16 methods_count;
  J2cObjectArray *methods = NULL;
  guint16 attributes_count;
  J2cObjectArray *attributes = NULL;

  GError *tmp_error = NULL;
  GDataInputStream *in = j2c_indexed_file_read (file, &tmp_error);
  if (!in)
    {
      g_propagate_error (error, tmp_error);
      return NULL;
    }

  g_input_stream_skip (G_INPUT_STREAM (in), 4, NULL, &tmp_error);
  if (tmp_error) goto error;

  minor_version = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  major_version = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  constant_pool = j2c_constant_pool_new (in, &tmp_error);
  if (tmp_error) goto error;

  access_flags = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  this_class = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  super_class = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  interfaces_count = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  interfaces = g_array_sized_new (FALSE, FALSE, sizeof (guint16), interfaces_count);
  for (gint i = 0; i < interfaces_count; i++)
    {
      guint16 val = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
      if (tmp_error) goto error;
      g_array_append_val (interfaces, val);
    }

  fields_count = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;
  fields = j2c_object_array_sized_new (fields_count);
  for (gint i = 0; i < fields_count; i ++)
    {
      J2cFieldInfo *field = j2c_field_info_from_class_file (in, constant_pool, &tmp_error);
      if (tmp_error) goto error;

      j2c_object_array_add (fields, field);
    }

  methods_count = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;
  methods = j2c_object_array_sized_new (methods_count);
  for (gint i = 0; i < methods_count; i ++)
    {
      J2cMethodInfo *method = j2c_method_info_from_class_file (in, constant_pool, &tmp_error);
      if (tmp_error) goto error;

      j2c_object_array_add (methods, method);
    }

  attributes_count = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;
  attributes = j2c_object_array_sized_new (attributes_count);
  for (gint i = 0; i < attributes_count; i++)
    {
      gpointer attribute =
        j2c_read_attribute (J2C_TYPE_COMPILATION_UNIT_CLASS, in, constant_pool, &tmp_error);
      if (tmp_error)
        {
          j2c_logger_warning ("Could not read attribute: %s.", tmp_error->message);
          g_error_free (tmp_error);
          tmp_error = NULL;
        }
      else
        {
          j2c_object_array_add (attributes, attribute);
        }
    }

success:
  ret = g_object_new (J2C_TYPE_COMPILATION_UNIT_CLASS,
		       MINOR_VERSION, minor_version,
		       MAJOR_VERSION, major_version,
		       CONSTANT_POOL, constant_pool,
		       ACCESS_FLAGS, access_flags,
		       THIS_CLASS, this_class,
		       SUPER_CLASS, super_class,
		       INTERFACES, interfaces,
		       FIELDS, fields,
		       METHODS, methods,
		       ATTRIBUTES, attributes,
		       NULL);

error:
  if (tmp_error)
    g_propagate_error (error, tmp_error);

  g_object_unref (in);
  if (interfaces)
    g_array_unref (interfaces);
  if (fields)
    g_object_unref (fields);
  if (methods)
    g_object_unref (methods);
  if (attributes)
    g_object_unref (attributes);

  return ret;
}

static void
j2c_compilation_unit_class_class_init (J2cCompilationUnitClassClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = j2c_compilation_unit_class_set_property;
  object_class->get_property = j2c_compilation_unit_class_get_property;
  object_class->constructed = j2c_compilation_unit_class_constructed;
  object_class->dispose = j2c_compilation_unit_class_dispose;

  obj_properties[PROP_MINOR_VERSION] =
    g_param_spec_uint (MINOR_VERSION,
		       MINOR_VERSION,
		       "Represents the minor version of the .class file.",
		       0, G_MAXINT16,
		       0,
		       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_MAJOR_VERSION] =
    g_param_spec_uint (MAJOR_VERSION,
		       MAJOR_VERSION,
		       "Represents the major version of the .class file.",
		       0, G_MAXINT16,
		       0,
		       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_ACCESS_FLAGS] =
    g_param_spec_uint (ACCESS_FLAGS,
		       ACCESS_FLAGS,
		       "Represents access restrictions on this type.",
		       0, G_MAXINT16,
		       0,
		       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_THIS_CLASS] =
    g_param_spec_uint (THIS_CLASS,
		       THIS_CLASS,
		       "This index of this class' J2cClassInfo in the constant pool.",
		       0, G_MAXINT16,
		       0,
		       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_SUPER_CLASS] =
    g_param_spec_uint (SUPER_CLASS,
		       SUPER_CLASS,
		       "This index of the super class' J2cClassInfo in the constant pool.",
		       0, G_MAXINT16,
		       0,
		       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  obj_properties[PROP_CONSTANT_POOL] =
    g_param_spec_object (CONSTANT_POOL,
			 CONSTANT_POOL,
			 "Pool of constant values used in this .class file.",
			 J2C_TYPE_CONSTANT_POOL,
			 G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  obj_properties[PROP_INTERFACES] =
    g_param_spec_boxed (INTERFACES,
			INTERFACES,
			"Collection of indexes pointing to a J2cClassInfo in the constant pool.",
			G_TYPE_ARRAY,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_FIELDS] =
    g_param_spec_object (FIELDS,
			FIELDS,
			"Collection of J2cFieldInfo for this type.",
			J2C_TYPE_OBJECT_ARRAY,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_METHODS] =
    g_param_spec_object (METHODS,
			METHODS,
			"Collection of J2cMethodInfo for this type.",
			J2C_TYPE_OBJECT_ARRAY,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_ATTRIBUTES] =
    g_param_spec_object (ATTRIBUTES,
			ATTRIBUTES,
			"Collection of J2cAttributeInfo for this type.",
			J2C_TYPE_OBJECT_ARRAY,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

static void
j2c_compilation_unit_class_init (J2cCompilationUnitClass *self)
{
  return;
}

static void
j2c_compilation_unit_interface_init (J2cCompilationUnitInterface *iface)
{
  iface->name = j2c_compilation_unit_class_name;
}

static void
j2c_compilation_unit_class_constructed (GObject *obj)
{
  J2cCompilationUnitClass *self = J2C_COMPILATION_UNIT_CLASS (obj);

  GError *error = NULL;
  J2cConstantPoolItem *info = j2c_constant_pool_get (self->constant_pool, self->this_class, &error);
  if (error)
    {
      g_warning ("Error getting class info %s", error->message);
      return;
    }
  g_return_if_fail (NULL != info);

  GValue val = G_VALUE_INIT;
  g_value_init (&val, G_TYPE_UINT64);
  g_object_get_property (G_OBJECT (info), J2C_CONSTANT_POOL_PROP_NAME_INDEX, &val);
  guint16 name_index = (guint16) g_value_get_uint64 (&val);
  g_value_unset (&val);
  g_object_unref (info);

  self->name = j2c_constant_pool_get_string (self->constant_pool, name_index, &error);
  if (error)
    g_warning ("Error getting type name %s", error->message);
  if (!self->name)
    self->name = g_strdup ("");
}

/****
  DESTRUCTION METHODS
 ****/

static void
j2c_compilation_unit_class_dispose (GObject *object)
{
  J2cCompilationUnitClass *self = J2C_COMPILATION_UNIT_CLASS (object);

  if (self->name)
    g_free (self->name);
  if (self->constant_pool)
    g_clear_object (&self->constant_pool);
  if (self->interfaces)
    g_array_unref (self->interfaces);
  if (self->methods)
    g_clear_object (&self->methods);
  if (self->fields)
    g_clear_object (&self->fields);
  if (self->attributes)
    g_clear_object (&self->attributes);

  self->name = NULL;

  G_OBJECT_CLASS (j2c_compilation_unit_class_parent_class)->dispose (object);
}

/****
  SETTERS AND GETTERS
 ****/

static void
j2c_compilation_unit_class_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cCompilationUnitClass *self = J2C_COMPILATION_UNIT_CLASS (object);

  switch (property_id)
    {
    case PROP_MINOR_VERSION:
      self->minor_version = (guint16) g_value_get_uint (value);
      break;

    case PROP_MAJOR_VERSION:
      self->major_version = (guint16) g_value_get_uint (value);
      break;

    case PROP_CONSTANT_POOL:
      self->constant_pool = g_value_dup_object (value);
      break;

    case PROP_ACCESS_FLAGS:
      self->access_flags = (guint16) g_value_get_uint (value);
      break;

    case PROP_THIS_CLASS:
      self->this_class = (guint16) g_value_get_uint (value);
      break;

    case PROP_SUPER_CLASS:
      self->super_class = (guint16) g_value_get_uint (value);
      break;

    case PROP_INTERFACES:
      self->interfaces = g_value_get_boxed (value);
      break;

    case PROP_FIELDS:
      self->fields = g_value_dup_object (value);
      break;

    case PROP_METHODS:
      self->methods = g_value_dup_object (value);
      break;

    case PROP_ATTRIBUTES:
      self->attributes = g_value_dup_object (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_compilation_unit_class_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cCompilationUnitClass *self = J2C_COMPILATION_UNIT_CLASS (object);

  switch (property_id)
    {
    case PROP_MINOR_VERSION:
      g_value_set_uint (value, (guint) self->minor_version);
      break;

    case PROP_MAJOR_VERSION:
      g_value_set_uint (value, (guint) self->major_version);
      break;

    case PROP_CONSTANT_POOL:
      g_value_set_object (value, self->constant_pool);
      break;

    case PROP_ACCESS_FLAGS:
      g_value_set_uint (value, (guint) self->access_flags);
      break;

    case PROP_THIS_CLASS:
      g_value_set_uint (value, (guint) self->this_class);
      break;

    case PROP_SUPER_CLASS:
      g_value_set_uint (value, (guint) self->super_class);
      break;

    case PROP_INTERFACES:
      g_value_set_boxed (value, self->interfaces);
      break;

    case PROP_FIELDS:
      g_value_set_object (value, self->fields);
      break;

    case PROP_METHODS:
      g_value_set_object (value, self->methods);
      break;

    case PROP_ATTRIBUTES:
      g_value_set_object (value, self->attributes);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

/****
  COMPILATION UNIT METHODS
 ****/

static gchar const *
j2c_compilation_unit_class_name (J2cCompilationUnit *compilation_unit)
{
  return J2C_COMPILATION_UNIT_CLASS (compilation_unit)->name;
}
