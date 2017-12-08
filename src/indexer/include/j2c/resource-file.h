#ifndef __RESOURCE_FILE_H__
#define __RESOURCE_FILE_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <glib.h>

#include <j2c/indexed-file.h>
#include <j2c/readable.h>

G_BEGIN_DECLS

#define J2C_TYPE_RESOURCE_FILE j2c_resource_file_get_type ()
G_DECLARE_FINAL_TYPE (J2cResourceFile, j2c_resource_file, J2C, RESOURCE_FILE, J2cIndexedFile);

J2cResourceFile *j2c_resource_file_new (J2cReadable *readable);

G_END_DECLS

#endif /* __RESOURCE_FILE_H__ */
