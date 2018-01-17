#ifndef __ZIP_H__
#define __ZIP_H__

#include <glib.h>
#include <gio/gio.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_ZIP_ERROR	g_quark_from_static_string ("j2c-zip")
typedef enum
{
  J2C_ZIP_OK,
  J2C_ZIP_MULTIDISK_ERROR,
  J2C_ZIP_RENAME_ERROR,
  J2C_ZIP_CLOSE_ERROR,
  J2C_ZIP_SEEK_ERROR,
  J2C_ZIP_READ_ERROR,
  J2C_ZIP_WRITE_ERROR,
  J2C_ZIP_CRC_ERROR,
  J2C_ZIP_ZIPCLOSED_ERROR,
  J2C_ZIP_NOENT_ERROR,
  J2C_ZIP_EXISTS_ERROR,
  J2C_ZIP_OPEN_ERROR,
  J2C_ZIP_TMPOPEN_ERROR,
  J2C_ZIP_ZLIB_ERROR,
  J2C_ZIP_MEMORY_ERROR,
  J2C_ZIP_CHANGED_ERROR,
  J2C_ZIP_COMPNOTSUPP_ERROR,
  J2C_ZIP_EOF_ERROR,
  J2C_ZIP_INVAL_ERROR,
  J2C_ZIP_NOZIP_ERROR,
  J2C_ZIP_INTERNAL_ERROR,
  J2C_ZIP_INCONS_ERROR,
  J2C_ZIP_REMOVE_ERROR,
  J2C_ZIP_DELETED_ERROR,
  J2C_ZIP_ENCRNOTSUPP_ERROR,
  J2C_ZIP_RDONLY_ERROR,
  J2C_ZIP_NOPASSWD_ERROR,
  J2C_ZIP_WRONGPASSWD_ERROR,
  J2C_ZIP_OPNOTSUPP_ERROR,
  J2C_ZIP_INUSE_ERROR,
  J2C_ZIP_TELL_ERROR,
  J2C_ZIP_COMPRESSED_DATA_ERROR
} J2cZipError;

#define J2C_TYPE_ZIP  j2c_zip_get_type ()
G_DECLARE_FINAL_TYPE (J2cZip, j2c_zip, J2C, ZIP, GObject)
#define J2C_TYPE_ZIP_FILE  j2c_zip_file_get_type ()
G_DECLARE_FINAL_TYPE (J2cZipFile, j2c_zip_file, J2C, ZIP_FILE, GObject)

J2cZip *j2c_zip_new (GFile *file, GError **error);
gchar *j2c_zip_get_name (J2cZip *self, guint64 index, GError **error);
gboolean j2c_zip_close (J2cZip *self, GError **error);

J2cZipFile *j2c_zip_file_open (J2cZip *zip, guint64 index, GError **error);
gssize j2c_zip_file_read (J2cZipFile *self, void *buffer, gsize size, GError **error);
gboolean j2c_zip_file_close (J2cZipFile *self, GError **error);

G_END_DECLS

#endif /* __ZIP_H__ */
