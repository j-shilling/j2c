#ifndef __MODIFIED_UTF8_H__
#define __MODIFIED_UTF8_H__

#include <glib.h>

#define J2C_UTF8_ERROR g_quark_from_static_string ("modified-utf8-error")

typedef enum
{
  J2C_INVALID_UTF8_ERROR
} J2cModifedUtf8Error;

gchar *j2c_modified_utf8_to_standard (guint8 *utf8_bytes, guint32 const length, GError **error);

#endif // __MODIFIED_UTF8_H__
