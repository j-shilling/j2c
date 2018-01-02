#ifndef __OBJECT_ARRAY_H__
#define __OBJECT_ARRAY_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_TYPE_OBJECT_ARRAY   j2c_object_array_get_type ()
G_DECLARE_FINAL_TYPE(J2cObjectArray, j2c_object_array, J2C, OBJECT_ARRAY, GObject)

J2cObjectArray *j2c_object_array_sized_new (guint size);
J2cObjectArray *j2c_object_array_new (void);

void j2c_object_array_add (J2cObjectArray *self, gpointer item);

G_END_DECLS

#endif // __OBJECT_ARRAY_H__
