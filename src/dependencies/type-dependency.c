#include <j2c/type-dependency.h>
#include <j2c/object-array.h>
#include <j2c/method-dependency-protected.h>

struct _J2cTypeDependency
{
  GObject parent;

  gchar *java_name;

  gboolean referenced;
  gboolean derived;
  gboolean instantiated;

  J2cTypeDependency *superclass;

  J2cObjectArray *methods;
  J2cObjectArray *fields;
  J2cObjectArray *children;
  J2cObjectArray *interfaces;
};

G_DEFINE_TYPE (J2cTypeDependency, j2c_type_dependency, G_TYPE_OBJECT)

static void
j2c_type_dependency_dispose (GObject *object)
{
  J2cTypeDependency *self = J2C_TYPE_DEPENDENCY (object);

  if (self->java_name)
    {
      g_free (self->java_name);
      self->java_name = NULL;
    }

  if (self->superclass)
    g_clear_object (&self->superclass);
  if (self->methods)
    g_clear_object (&self->methods);
  if (self->fields)
    g_clear_object (&self->fields);
  if (self->children)
    g_clear_object (&self->children);
  if (self->interfaces)
    g_clear_object (&self->interfaces);

  G_OBJECT_CLASS (j2c_type_dependency_parent_class)->dispose (object);
}

static void
j2c_type_dependency_class_init (J2cTypeDependencyClass *klass)
{
  G_OBJECT_CLASS(klass)->dispose = j2c_type_dependency_dispose;
}

static void
j2c_type_dependency_init (J2cTypeDependency *self)
{
  self->java_name = NULL;
  self->referenced = FALSE;
  self->derived = FALSE;
  self->instantiated = FALSE;
  self->superclass = NULL;
  self->methods = j2c_object_array_new ();
  self->fields = j2c_object_array_new ();
  self->children = j2c_object_array_new ();
  self->interfaces = j2c_object_array_new ();
}

J2cTypeDependency *
j2c_type_dependency_new (gchar *java_name)
{
  J2cTypeDependency *ret = g_object_new (J2C_TYPE_TYPE_DEPENDENCY, NULL);
  ret->java_name = g_strdup (java_name);
  return ret;
}

void
j2c_type_dependency_set_referenced (J2cTypeDependency *self)
{
  g_return_if_fail (NULL != self);

  self->referenced = TRUE;
}

void
j2c_type_dependency_set_instantiated (J2cTypeDependency *self)
{
  g_return_if_fail (NULL != self);

  self->instantiated = TRUE;
}

void
j2c_type_dependency_set_parent (J2cTypeDependency *self, J2cTypeDependency *parent)
{
  g_return_if_fail (NULL != self);
  g_return_if_fail (NULL != parent);

  if (self->superclass)
    g_object_unref (self->superclass);
  self->superclass = g_object_ref (parent);

  gint len = j2c_object_array_length (parent->children);
  for (gint i = 0; i < len; i ++)
    {
      J2cTypeDependency *child = j2c_object_array_get (parent->children, i);
      if (0 == g_strcmp0 (self->java_name, child->java_name))
	{
	  g_object_unref (child);
	  return;
	}
      g_object_unref (child);
    }

  j2c_object_array_add (parent->children, self);
}

void j2c_type_dependency_add_child (J2cTypeDependency *self, J2cTypeDependency *child);
void j2c_type_dependency_add_field (J2cTypeDependency *self, J2cFieldDependency *field);

void
j2c_type_dependency_add_method (J2cTypeDependency *self, J2cMethodDependency *method)
{
  g_return_if_fail (self != NULL);
  g_return_if_fail (method != NULL);

  for (gint i = 0; i < j2c_object_array_length (self->methods); i ++)
    {
      J2cMethodDependency *item = j2c_object_array_get (self->methods, i);
      if (0 == g_strcmp0 (item->name, method->name) && 0 == g_strcmp0 (item->descriptor, method->descriptor))
	{
	  g_object_unref (item);
	  return;
	}
      g_object_unref (item);
    }

  j2c_object_array_add (self->methods, method);
  method->type = g_object_ref (self);
}

void
j2c_type_dependency_add_interface (J2cTypeDependency *self, J2cTypeDependency *interface)
{
  g_return_if_fail (self != NULL);
  g_return_if_fail (interface != NULL);

  for (gint i = 0; i < j2c_object_array_length (self->interfaces); i ++)
    {
      J2cTypeDependency *item = j2c_object_array_get (self->interfaces, i);
      if (0 == g_strcmp0 (item->java_name, interface->java_name))
	{
	  g_object_unref (item);
	  return;
	}
      g_object_unref (item);
    }

  j2c_object_array_add (self->interfaces, interface);
  j2c_object_array_add (interface->children, self);
}

gboolean j2c_type_dependency_is_referenced (J2cTypeDependency *self);
gboolean j2c_type_dependency_is_derived (J2cTypeDependency *self);
gboolean j2c_type_dependency_is_instantiated (J2cTypeDependency *self);

J2cTypeDependency *j2c_type_dependency_get_child (J2cTypeDependency *self, gchar *name);
J2cFieldDependency *j2c_type_dependency_get_field (J2cTypeDependency *self, gchar *name);

J2cMethodDependency *
j2c_type_dependency_get_method (J2cTypeDependency *self, gchar *name, gchar *descriptor)
{
  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (name != NULL, NULL);
  g_return_val_if_fail (descriptor != NULL, NULL);

  for (gint i = 0; i < j2c_object_array_length (self->methods); i ++)
    {
      J2cMethodDependency *method = j2c_object_array_get (self->methods, i);
      if (0 == g_strcmp0 (method->name, name) && 0 == g_strcmp0 (method->descriptor, descriptor))
	return method;
      g_object_unref (method);
    }

  return NULL;
}

J2cTypeDependency *j2c_type_dependency_get_interface (J2cTypeDependency *self, gchar *name);

