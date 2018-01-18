#ifndef ZIP_INPUT_STREAM_H_
#define ZIP_INPUT_STREAM_H_

#include <glib.h>
#include <glib-object.h>
#include <j2c/zip.h>

G_BEGIN_DECLS

#define J2C_TYPE_ZIP_INPUT_STREAM	j2c_zip_input_stream_get_type ()
G_DECLARE_FINAL_TYPE (J2cZipInputStream, j2c_zip_input_stream, J2C, ZIP_INPUT_STREAM, GInputStream);

J2cZipInputStream *j2c_zip_input_stream_open (J2cZip *zip, guint64 index, GError **error);

G_END_DECLS

#endif /* ZIP_INPUT_STREAM_H_ */
