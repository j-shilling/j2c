#ifndef __JAR_FILE_PRIV_H__
#define __JAR_FILE_PRIV_H__

#include <glib.h>
#include <j2c/zip.h>
#include <j2c/jar-file.h>

J2cZip *j2c_jar_file_open (J2cJarFile *self, GError **error);

#endif /* __JAR_FILE_PRIV_H__ */
