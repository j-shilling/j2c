#ifndef __INDEXED_FILE__
#define __INDEXED_FILE__

#include <glib-object.h>
#include <gio/gio.h>
#include <glib.h>

#include <j2c/readable.h>
#include <j2c/method.h>

#define J2C_INDEXED_FILE_ERROR	g_quark_from_static_string ("j2c-indexed-file")

typedef enum
{
  J2C_INDEXED_FILE_TYPE_ERROR
} J2cIndexedFileError;

typedef enum
{
  J2C_FILE_TYPE_RESOURCE,
  J2C_FILE_TYPE_CLASS,
  J2C_FILE_TYPE_SOURCE
} J2cFileType;

G_BEGIN_DECLS

#define J2C_TYPE_INDEXED_FILE	j2c_indexed_file_get_type()
G_DECLARE_DERIVABLE_TYPE (J2cIndexedFile, j2c_indexed_file, J2C, INDEXED_FILE, GObject);

static const gchar *J2C_INDEXED_FILE_PROPERTY_READABLE = "indexed-file-readable";

struct _J2cIndexedFileClass
{
  GObjectClass parent_class;

  gchar *(*get_name) (J2cIndexedFile *self);
  J2cFileType (*get_file_type) (J2cIndexedFile *self);

  GDataInputStream *(*read) (J2cIndexedFile *self, GError **error);
};

J2cIndexedFile *j2c_indexed_file_new (J2cReadable *readable, GError **error);

gchar *j2c_indexed_file_get_name (J2cIndexedFile *self);
J2cFileType j2c_indexed_file_get_file_type (J2cIndexedFile *self);
J2cMethod *j2c_indexed_file_get_main (J2cIndexedFile *self);

J2cReadable *j2c_indexed_file_get_readable (J2cIndexedFile *self);
GDataInputStream *j2c_indexed_file_read (J2cIndexedFile *self, GError **error);

G_END_DECLS

#endif /* __INDEXED_FILE__ */
