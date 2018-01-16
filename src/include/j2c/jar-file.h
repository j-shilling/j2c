#ifndef JAR_FILE_H_
#define JAR_FILE_H_

#include <glib.h>
#include <gio/gio.h>
#include <glib-object.h>

#include <j2c/readable.h>
#include <zip.h>

G_BEGIN_DECLS

#define J2C_JAR_FILE_ERROR	g_quark_from_static_string ("j2c-jar-file")

typedef enum
{
  J2C_JAR_FILE_NOT_JAR_ERROR
} J2cJarFileError;

#define J2C_TYPE_JAR_FILE	j2c_jar_file_get_type ()
#define J2C_TYPE_JAR_MEMBER	j2c_jar_member_get_type ()
G_DECLARE_FINAL_TYPE (J2cJarFile, j2c_jar_file, J2C, JAR_FILE, GObject);
G_DECLARE_FINAL_TYPE (J2cJarMember, j2c_jar_member, J2C, JAR_MEMBER, GObject);

J2cJarFile *j2c_jar_file_new (GFile *file, GError **error);
J2cReadable **j2c_jar_file_expand (J2cJarFile *self);
gboolean j2c_jar_file_close (J2cJarFile *self, GError **error);
zip_t *j2c_jar_file_open (J2cJarFile *self, GError **error);


G_END_DECLS

#endif /* JAR_FILE_H_ */
