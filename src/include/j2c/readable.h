#ifndef READABLE_H_
#define READABLE_H_

#include <glib.h>
#include <gio/gio.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_TYPE_READABLE	j2c_readable_get_type ()
#define J2C_TYPE_READABLE_FILE	j2c_readable_file_get_type ()
G_DECLARE_INTERFACE (J2cReadable, j2c_readable, J2C, READABLE, GObject)
G_DECLARE_FINAL_TYPE (J2cReadableFile, j2c_readable_file, J2C, READABLE_FILE, GObject)

struct _J2cReadableInterface
{
  GTypeInterface parent_interface;

  GInputStream *(*read) (J2cReadable *self, GError **error);
  const gchar * (*name) (J2cReadable *self);
};

J2cReadable *j2c_readable_new (GFile *file);
GInputStream *j2c_readable_read (J2cReadable *self, GError **error);
const gchar * j2c_readable_name (J2cReadable *self);

G_END_DECLS

#endif /* READABLE_H_ */
