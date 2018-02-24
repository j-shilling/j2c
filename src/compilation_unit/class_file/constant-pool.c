#include <j2c/constant-pool.h>
#include <j2c/constant-pool-item.h>
#include <j2c/object-array.h>

struct _J2cConstantPool
{
    GObject parent;

    guint16 count;
    J2cObjectArray *pool;
};

G_DEFINE_TYPE (J2cConstantPool, j2c_constant_pool, G_TYPE_OBJECT)

static void j2c_constant_pool_dispose (GObject *object);

/****
  CLASS INIT FUNCTIONS
 ****/

static void
j2c_constant_pool_class_init (J2cConstantPoolClass *klass)
{
    G_OBJECT_CLASS (klass)->dispose = j2c_constant_pool_dispose;
}

/****
  OBJECT INIT FUNCTIONS
 ****/

static void
j2c_constant_pool_init (J2cConstantPool *self)
{
    return;
}

/****
  DISPOSE FUNCTIONS
 ****/

static void
j2c_constant_pool_dispose (GObject *object)
{
    J2cConstantPool *constant_pool = J2C_CONSTANT_POOL (object);

    if (constant_pool->pool)
      g_clear_object (&constant_pool->pool);
    constant_pool->count = 0;

    G_OBJECT_CLASS (j2c_constant_pool_parent_class)->dispose (object);
}

/****
  PUBLIC METHODS
 ****/

J2cConstantPool *
j2c_constant_pool_new (GDataInputStream *in, GError **error)
{
    /* Stop if something is wrong with error */
    g_return_val_if_fail (error == NULL || *error == NULL, NULL);
    g_return_val_if_fail (in != NULL, NULL);

    GError *tmp_error = NULL;

    /****
      READ CONSTANT POOL
     ****/

    guint16 constant_pool_count = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);

    if (tmp_error)
      {
	g_propagate_error (error, tmp_error);
	return NULL;
      }

    J2cObjectArray *pool = j2c_object_array_sized_new ((guint) (constant_pool_count - 1));
    for (gint i = 1; i < constant_pool_count; i ++) // Stupid CP is indexed from 1
    {
      J2cConstantPoolItem *item = j2c_constant_pool_item_new (in, &tmp_error);
      if (tmp_error)
	{
	  g_object_unref (pool);
	  g_propagate_error (error, tmp_error);
	  return NULL;
	}

      if (J2C_IS_LONG_INFO (item) || J2C_IS_DOUBLE_INFO (item))
	j2c_object_array_add (pool, item);
      j2c_object_array_add (pool, item);

      g_object_unref (item);
    }

    J2cConstantPool *ret = g_object_new (J2C_TYPE_CONSTANT_POOL, NULL);
    ret->pool = pool;
    ret->count = constant_pool_count;

    return ret;
}

J2cConstantPoolItem *
j2c_constant_pool_get (J2cConstantPool *self, const guint16 index, GError **error)
{
    g_return_val_if_fail (error == NULL || *error == NULL, NULL);

    if (!self)
    {
        g_set_error (error,
                     J2C_CONSTANT_POOL_ERROR,
                     J2C_CONSTANT_POOL_NULL_ERROR,
                     "Cannot get from a null pool.");
        return NULL;
    }

    guint len = j2c_object_array_length(self->pool);
    if (index < 1 || index > len || index >= self->count)
    {
        g_set_error (error,
                     J2C_CONSTANT_POOL_ERROR,
                     J2C_CONSTANT_POOL_INDEX_ERROR,
                     "%u is not a valid index for this pool { count = %u; length = %u }",
                     index, self->count, len);
        return NULL;
    }

    J2cConstantPoolItem *ret = j2c_object_array_get (self->pool, index - 1);
    return g_object_ref(ret);
}
