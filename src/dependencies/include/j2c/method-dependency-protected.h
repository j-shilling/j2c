#ifndef __METHOD_DEPENDENCY_PROTECTED_H__
#define __METHOD_DEPENDENCY_PROTECTED_H__

#include <j2c/type-dependency.h>
#include <j2c/object-array.h>

struct _J2cMethodDependency
{
  GObject parent;

  gchar *name;
  gchar *descriptor;

  gboolean called;
  gboolean overrided;

  J2cTypeDependency *type;
  J2cObjectArray *overriders;
  J2cMethodDependency *overrided_method;
};

#endif
