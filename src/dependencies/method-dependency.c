#include <j2c/method-dependency.h>
#include <j2c/method-dependency-protected.h>
#include <j2c/type-dependency.h>
#include <j2c/object-array.h>

G_DEFINE_TYPE (J2cMethodDependency, j2c_method_dependency, G_TYPE_OBJECT)

static void
j2c_method_dependency_dispose (GObject *object)
{
  J2cMethodDependency *self = J2C_METHOD_DEPENDENCY (object);

  if (self->name)
    {
      g_free (self->name);
      self->name = NULL;
    }
  if (self->descriptor)
    {
      g_free (self->descriptor);
      self->descriptor = NULL;
    }

  if (self->type)
    g_clear_object (&self->type);
  if (self->overriders)
    g_clear_object (&self->overriders);
  if (self->overrided_method)
    g_clear_object (&self->overrided_method);

  G_OBJECT_CLASS (j2c_method_dependency_parent_class)->dispose (object);
}

static void
j2c_method_dependency_class_init (J2cMethodDependencyClass *klass)
{
  G_OBJECT_CLASS (klass)->dispose = j2c_method_dependency_dispose;
}

static void
j2c_method_dependency_init (J2cMethodDependency *self)
{
  self->name = NULL;
  self->descriptor = NULL;
  self->type = NULL;
  self->overriders = j2c_object_array_new ();
  self->overrided_method = NULL;

  self->called = FALSE;
  self->overrided = FALSE;
}

J2cMethodDependency *
j2c_method_dependency_new (gchar *java_name, gchar *descriptor)
{
  J2cMethodDependency *ret = g_object_new (J2C_TYPE_METHOD_DEPENDENCY, NULL);
  ret->name = g_strdup (java_name);
  ret->descriptor = g_strdup (descriptor);
  return ret;
}

gchar *j2c_method_dependency_name (J2cMethodDependency *self);
gchar *j2c_method_dependency_descriptor (J2cMethodDependency *self);
gchar *j2c_method_dependency_defining_type (J2cMethodDependency *self);

gboolean j2c_method_dependency_is_called (J2cMethodDependency *self);
gboolean j2c_method_dependency_is_overrided (J2cMethodDependency *self);

J2cMethodDependency *j2c_method_dependency_get_overrided (J2cMethodDependency *self);
J2cMethodDependency *j2c_method_dependency_get_overrider (J2cMethodDependency *self, gchar *implementing_class_name);

void
j2c_method_dependency_set_called (J2cMethodDependency *self)
{
  g_return_if_fail (self != NULL);
  self->called = TRUE;
}

void
j2c_method_dependency_add_overrider (J2cMethodDependency *self, J2cMethodDependency *overrider)
{
  g_return_if_fail (self != NULL);
  g_return_if_fail (overrider != NULL);

  for (gint i = 0; i < j2c_object_array_length (self->overriders); i ++)
    {
      J2cMethodDependency *item = j2c_object_array_get (self->overriders, i);
      if (0 == g_strcmp0 (overrider->name, item->name) && 0 == g_strcmp0 (overrider->descriptor, item->descriptor))
	{
	  g_object_unref (item);
	  return;
	}
      g_object_unref (item);
    }

  j2c_object_array_add (self->overriders, overrider);
  if (overrider->type)
    g_object_unref (overrider->type);
  overrider->type = g_object_ref (self);
}


