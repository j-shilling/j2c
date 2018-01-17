#ifndef __JAR_MEMBER_H__
#define __JAR_MEMBER_H__

#include <glib-object.h>
#include <j2c/jar-file.h>

G_BEGIN_DECLS

#define J2C_TYPE_JAR_MEMBER   j2c_jar_member_get_type ()
G_DECLARE_FINAL_TYPE (J2cJarMember, j2c_jar_member, J2C, JAR_MEMBER, GObject);

J2cJarMember *j2c_jar_member_new (J2cJarFile *file, guint index, const gchar *jar_name);

G_END_DECLS

#endif /* __JAR_MEMBER_H__ */
