#ifndef __CLASS_FILE_H__
#define __CLASS_FILE_H__

#include <glib-object.h>
#include <glib.h>

#include <j2c/indexed-file.h>
#include <j2c/readable.h>

/* CLASS FILE CONSTANT VALUES */

static const guint32 J2C_CLASS_FILE_MAGIC = 0xCAFEBABE;

G_BEGIN_DECLS

#define J2C_TYPE_CLASS_FILE j2c_class_file_get_type ()
G_DECLARE_FINAL_TYPE (J2cClassFile, j2c_class_file, J2C, CLASS_FILE, J2cIndexedFile);

J2cClassFile *j2c_class_file_new (J2cReadable *readable, GError **error);

G_END_DECLS

#endif /* __CLASS_FILE_H__ */
