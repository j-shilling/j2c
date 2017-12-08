#ifndef ZIP_INPUT_STREAM_H_
#define ZIP_INPUT_STREAM_H_

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <zip.h>

G_BEGIN_DECLS

#define J2C_ZIP_ERROR	g_quark_from_static_string ("j2c-zip-input-stream")

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

#define J2C_TYPE_ZIP_INPUT_STREAM	j2c_zip_input_stream_get_type ()
G_DECLARE_FINAL_TYPE (J2cZipInputStream, j2c_zip_input_stream, J2C, ZIP_INPUT_STREAM, GInputStream);

J2cZipInputStream *j2c_zip_input_stream_open (GFile *file, guint64 index, GError **error);

GQuark
j2c_zip_input_stream_quark (void);

gboolean
j2c_zip_input_stream_set_error (zip_error_t *zer, gchar *path, GError **error);
gboolean
j2c_zip_input_stream_set_error_from_code (const gint zer, gchar *path, GError **error);

G_END_DECLS

#endif /* ZIP_INPUT_STREAM_H_ */
