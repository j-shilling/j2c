#ifndef __CONSTANT_POOL_H__
#define __CONSTANT_POOL_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <j2c/constant-pool-item.h>

typedef enum
{
  J2C_CONSTANT_POOL_NULL_ERROR,
  J2C_CONSTANT_POOL_INDEX_ERROR,
  J2C_CONSTANT_POOL_TYPE_ERROR,
  J2C_CONSTANT_POOL_UNKOWN_ERROR
} J2cConstantPoolError;

#define J2C_CONSTANT_POOL_ERROR	g_quark_from_static_string ("j2c-constant-pool")

G_BEGIN_DECLS

#define J2C_TYPE_CONSTANT_POOL j2c_constant_pool_get_type ()

G_DECLARE_FINAL_TYPE (J2cConstantPool, j2c_constant_pool, J2C, CONSTANT_POOL, GObject);

J2cConstantPool *j2c_constant_pool_new (GDataInputStream *in, GError **error);
J2cConstantPoolItem *j2c_constant_pool_get (J2cConstantPool *self, const guint16 index, GError **error);

G_END_DECLS

#endif /* __CONSTANT_POOL_H___ */
