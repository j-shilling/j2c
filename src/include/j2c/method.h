#ifndef __METHOD_H__
#define __METHOD_H__

#include <glib.h>
#include <glib-object.h>
#include <j2c/dependency-info.h>

G_BEGIN_DECLS

#define J2C_TYPE_METHOD     j2c_method_get_type ()
G_DECLARE_DERIVABLE_TYPE (J2cMethod, j2c_method, J2C, METHOD, GObject)

struct _J2cMethodClass
{
  GObjectClass parent_class;

  gchar *(*get_descriptor) (J2cMethod *self);
  gchar *(*get_java_name) (J2cMethod *self);
  guint16 (*get_access_flags) (J2cMethod *self);
  J2cDependencyInfo *(*get_dependency_info) (J2cMethod *self);
};

gchar *j2c_method_get_descriptor (J2cMethod *self);
gchar *j2c_method_get_java_name (J2cMethod *self);
guint16 j2c_method_get_access_flags (J2cMethod *self);
J2cDependencyInfo *j2c_method_get_dependency_info (J2cMethod *self);

typedef enum
{
  METHOD_ACC_PUBLIC        = 0x0001,
  METHOD_ACC_PRIVATE       = 0x0002,
  METHOD_ACC_PROTECTED     = 0x0004,
  METHOD_ACC_STATIC        = 0x0008,
  METHOD_ACC_FINAL         = 0x0010,
  METHOD_ACC_SYNCHRONIZED  = 0x0020,
  METHOD_ACC_BRIDGE        = 0x0040,
  METHOD_ACC_VARARGS       = 0x0080,
  METHOD_ACC_NATIVE        = 0x0100,
  METHOD_ACC_ABSTRACT      = 0x0400,
  METHOD_ACC_STRICT        = 0x0800,
  METHOD_ACC_SYNTHETIC     = 0x1000
} J2cMethodAccessFlags;

G_END_DECLS

#endif // __METHOD_H__
