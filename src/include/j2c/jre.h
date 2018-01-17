#ifndef __JRE_H__
#define __JRE_H__

#include <gio/gio.h>

GFile *j2c_jre_get_lib_dir (void);
GFile *j2c_jre_get_file (const gchar *name);

#endif /* __JRE_H__ */
