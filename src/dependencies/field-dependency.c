#include <j2c/field-dependency.h>
#include <j2c/type-dependency.h>

struct _J2cFieldDependency
{
  GObject parent;

  gchar *name;
  J2cTypeDependency *container;

  gboolean read;
  gboolean written;
};
