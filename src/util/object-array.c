#include <j2c/object-array.h>

struct _J2cObjectArray
{
  GObject parent;

  GType type;
  GMutex *mutex;

  GPtrArray *array;
};

G_DEFINE_TYPE(J2cObjectArray, j2c_object_array, G_TYPE_OBJECT)

static void j2c_object_array_dispose (GObject *object);
static void j2c_object_array_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_object_array_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
j2c_object_array_class_init (J2cObjectArrayClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_object_array_set_property;
  object_class->get_property = j2c_object_array_get_property;
  object_class->dispose       = j2c_object_array_dispose;

  g_object_class_install_property (object_class, 1,
                                   g_param_spec_boxed ("object-array-array",
                                                       "object-array-array",
                                                       "",
                                                       G_TYPE_PTR_ARRAY,
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_object_array_init (J2cObjectArray *self)
{
  self->type = G_TYPE_OBJECT;
  self->mutex = g_malloc (sizeof (GMutex));
  g_mutex_init (self->mutex);
  self->array = NULL;
}

static void
j2c_object_array_dispose (GObject *object)
{
  J2cObjectArray *self = J2C_OBJECT_ARRAY (object);

  if (!self->mutex)
    return;
  g_mutex_lock (self->mutex);

  if (self->array)
    {
      g_ptr_array_free (self->array, TRUE);
      self->array = NULL;
    }

  g_mutex_unlock (self->mutex);
  g_mutex_clear (self->mutex);
  self->mutex = NULL;
}

static void
j2c_object_array_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cObjectArray *self = J2C_OBJECT_ARRAY (object);

  if (property_id == 1)
    {
      if (self->array)
        g_ptr_array_free (self->array, TRUE);
      self->array = g_value_get_boxed (value);
    }
  else
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
j2c_object_array_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cObjectArray *self = J2C_OBJECT_ARRAY (object);

  if (property_id == 1)
    {
      g_value_set_boxed (value, self->array);
    }
  else
    {
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

J2cObjectArray *
j2c_object_array_sized_new (guint size)
{
  GPtrArray *array = g_ptr_array_new_full (size, g_object_unref);

  return g_object_new (J2C_TYPE_OBJECT_ARRAY,
                       "object-array-array", array,
                       NULL);
}

J2cObjectArray *
j2c_object_array_new (void)
{
  GPtrArray *array = g_ptr_array_new_with_free_func (g_object_unref);

  return g_object_new (J2C_TYPE_OBJECT_ARRAY,
                       "object-array-array", array,
                       NULL);
}

void
j2c_object_array_add (J2cObjectArray *self, gpointer item)
{
  g_return_if_fail (self != NULL);
  g_return_if_fail (item != NULL);
  g_return_if_fail (G_IS_OBJECT (item));

  g_mutex_lock (self->mutex);
  g_ptr_array_add (self->array, g_object_ref(item));
  g_mutex_unlock (self->mutex);
}

guint
j2c_object_array_length (J2cObjectArray *self)
{
  g_return_val_if_fail (self != NULL, 0);
  g_return_val_if_fail (self->array != NULL, 0);

  g_mutex_lock (self->mutex);
  guint ret = self->array->len;
  g_mutex_unlock (self->mutex);

  return ret;
}

gpointer
j2c_object_array_get (J2cObjectArray *self, const guint index)
{
  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->array != NULL, NULL);
  g_return_val_if_fail (index < self->array->len, NULL);

  g_mutex_lock (self->mutex);
  gpointer ret = self->array->pdata[index];
  g_mutex_unlock (self->mutex);

  return ret;
}
