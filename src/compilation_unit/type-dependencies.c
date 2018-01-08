#include <j2c/type-dependencies.h>

struct _J2cTypeDependencies
{
  GObject parent;

  gchar *this_type;
  gchar *super_class;
  gchar **interfaces;
  gchar **field_types;
};

G_DEFINE_TYPE(J2cTypeDependencies, j2c_type_dependencies, G_TYPE_OBJECT)

enum
{
  PROP_THIS_TYPE,
  PROP_SUPER_CLASS,
  PROP_INTERFACES,
  PROP_FIELD_TYPES,

  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static gchar const *const J2C_TYPE_DEPENDENCIES_PROP_THIS_TYPE= "type-dependencies-this-type";
static gchar const *const J2C_TYPE_DEPENDENCIES_PROP_SUPER_CLASS= "type-dependencies-super-class";
static gchar const *const J2C_TYPE_DEPENDENCIES_PROP_INTERFACES= "type-dependencies-interfaces";
static gchar const *const J2C_TYPE_DEPENDENCIES_PROP_FIELD_TYPES= "type-dependencies-field-types";

J2cTypeDependencies *
j2c_type_dependencies_new (gchar *this_type, gchar *super_class, gchar **interfaces, gchar **field_types)
{
  g_return_val_if_fail (this_type != NULL && *this_type != '\0', NULL);
  g_return_val_if_fail (super_class == NULL || *super_class != '\0', NULL);

  return g_object_new (J2C_TYPE_TYPE_DEPENDENCIES,
                       J2C_TYPE_DEPENDENCIES_PROP_THIS_TYPE, this_type,
                       J2C_TYPE_DEPENDENCIES_PROP_SUPER_CLASS, super_class,
                       J2C_TYPE_DEPENDENCIES_PROP_INTERFACES, interfaces,
                       J2C_TYPE_DEPENDENCIES_PROP_FIELD_TYPES, field_types,
                       NULL);
}

gchar *
j2c_type_dependencies_get_this_type (J2cTypeDependencies *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  return g_strdup (self->this_type);
}

gchar *
j2c_type_dependencies_get_super_class (J2cTypeDependencies *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  return g_strdup (self->super_class);
}

gchar **
j2c_type_dependencies_get_interfaces (J2cTypeDependencies *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  return g_strdupv (self->interfaces);
}

gchar **
j2c_type_dependencies_get_field_types (J2cTypeDependencies *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  return g_strdupv (self->field_types);
}

static void
j2c_type_dependencies_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cTypeDependencies *self = J2C_TYPE_DEPENDENCIES (object);

  switch (property_id)
    {
      case PROP_THIS_TYPE:
        if (self->this_type)
          g_free (self->this_type);
        self->this_type = g_value_dup_string (value);
        break;
      case PROP_SUPER_CLASS:
        if (self->super_class)
          g_free (self->super_class);
        self->super_class = g_value_dup_string (value);
        break;
      case PROP_INTERFACES:
        if (self->interfaces)
          g_strfreev (self->interfaces);
        gchar **interfaces = g_value_get_boxed (value);
        self->interfaces = g_strdupv (interfaces);
        break;
      case PROP_FIELD_TYPES:
        if (self->field_types)
          g_strfreev (self->field_types);
        gchar **field_types = g_value_get_boxed (value);
        self->field_types = g_strdupv (field_types);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_type_dependencies_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cTypeDependencies *self = J2C_TYPE_DEPENDENCIES (object);

  switch (property_id)
    {
      case PROP_THIS_TYPE:
        g_value_set_string (value, self->this_type);
        break;
      case PROP_SUPER_CLASS:
        g_value_set_string (value, self->super_class);
        break;
      case PROP_INTERFACES:
        g_value_set_boxed (value, self->interfaces);
        break;
      case PROP_FIELD_TYPES:
        g_value_set_boxed (value, self->field_types);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_type_dependencies_dispose (GObject *object)
{
  J2cTypeDependencies *self = J2C_TYPE_DEPENDENCIES (object);

  if (self->this_type)
    {
      g_free (self->this_type);
      self->this_type = NULL;
    }

  if (self->super_class)
    {
      g_free (self->super_class);
      self->super_class = NULL;
    }

  if (self->interfaces)
    {
      g_strfreev (self->interfaces);
      self->interfaces = NULL;
    }

  if (self->field_types)
    {
      g_strfreev (self->field_types);
      self->field_types = NULL;
    }

  G_OBJECT_CLASS (j2c_type_dependencies_parent_class)->dispose (object);
}

static void
j2c_type_dependencies_class_init (J2cTypeDependenciesClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = j2c_type_dependencies_set_property;
  object_class->get_property = j2c_type_dependencies_get_property;
  object_class->dispose      = j2c_type_dependencies_dispose;

  obj_properties[PROP_THIS_TYPE] =
    g_param_spec_string (J2C_TYPE_DEPENDENCIES_PROP_THIS_TYPE,
                         J2C_TYPE_DEPENDENCIES_PROP_THIS_TYPE,
                         "Fully qualified java name of this type",
                         "",
                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  obj_properties[PROP_SUPER_CLASS] =
    g_param_spec_string (J2C_TYPE_DEPENDENCIES_PROP_SUPER_CLASS,
                         J2C_TYPE_DEPENDENCIES_PROP_SUPER_CLASS,
                         "Fully qualified java name of this type's superclass",
                         "",
                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  obj_properties[PROP_INTERFACES] =
    g_param_spec_boxed (J2C_TYPE_DEPENDENCIES_PROP_INTERFACES,
                        J2C_TYPE_DEPENDENCIES_PROP_INTERFACES,
                        "Required interface types",
                        G_TYPE_STRV,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  obj_properties[PROP_FIELD_TYPES] =
    g_param_spec_boxed (J2C_TYPE_DEPENDENCIES_PROP_FIELD_TYPES,
                        J2C_TYPE_DEPENDENCIES_PROP_FIELD_TYPES,
                        "Required interface types",
                        G_TYPE_STRV,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

static void
j2c_type_dependencies_init (J2cTypeDependencies *self)
{
  return;
}
