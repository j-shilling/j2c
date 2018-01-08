#include <j2c/modified-utf8.h>

gchar *
j2c_modified_utf8_to_standard (guint8 *utf8_bytes, guint32 const length, GError **error)
{
    g_return_val_if_fail (length == 0 || utf8_bytes != NULL, NULL);

    gint index = 0;
    gchar *value = g_strdup ("");
    while (index < length)
    {
        if (utf8_bytes[index] == 0 || (utf8_bytes[index] >= 0xf0 && utf8_bytes[index] <= 0xff))
        {
            g_set_error (error,
            J2C_UTF8_ERROR,
            J2C_INVALID_UTF8_ERROR,
            "No byte in a modified utf8 string may be 0 or in the range "
            "0xf0 to 0xff. This byte is %xd", utf8_bytes[index]);
            goto error;
        }

        gunichar c;
        if ((utf8_bytes[index] & 0x80) == 0)
        {
            c = utf8_bytes[index];
            index ++;
        }
        else if (index + 1 < length
                 && (utf8_bytes[index] & 0xe0) == 0xc0
                 && (utf8_bytes[index+1] & 0xc0) == 0x80)
        {
            c = ((utf8_bytes[index] & 0x1f) << 6) + (utf8_bytes[index+1] & 0x3f);
            index += 2;
        }
        else if (index + 2 < length
                 && (utf8_bytes[index] & 0xf0) == 0xe0
                 && (utf8_bytes[index+1] & 0xc0) == 0x80
                 && (utf8_bytes[index+2] & 0xc0) == 0x80)
        {
            c = ((utf8_bytes[index] & 0xf) << 12) + ((utf8_bytes[index+1] & 0x3f) << 6) + (utf8_bytes[index+2] & 0x3f);
            index += 3;
        }
        else if (index + 5 < length
                 && utf8_bytes[index] == 0xed
                 && (utf8_bytes[index+1] & 0xf0) == 0xa0
                 && (utf8_bytes[index+2] & 0xc0) == 0x80
                 && utf8_bytes[index+3] == 0xed
                 && (utf8_bytes[index+4] & 0xf0) == 0xb0
                 && (utf8_bytes[index+5] & 0xc0) == 0x80)
        {
            c = 0x10000 + ((utf8_bytes[index + 1] & 0x0f) << 16)
                + ((utf8_bytes[index + 2] & 0x3f) << 10)
                + ((utf8_bytes[index + 3] & 0x0f) << 6)
                + (utf8_bytes[index + 5] & 0x3f);
            index += 8;
        }
        else
        {
            g_set_error (error,
                         J2C_UTF8_ERROR,
                         J2C_INVALID_UTF8_ERROR,
                         "Cannot parse utf8 bytes %xd %xd %xd %xd %xd %xd",
                         utf8_bytes[index],
                         index + 1 < length ? utf8_bytes[index+1] : 0,
                         index + 2 < length ? utf8_bytes[index+2] : 0,
                         index + 3 < length ? utf8_bytes[index+3] : 0,
                         index + 4 < length ? utf8_bytes[index+4] : 0,
                         index + 5 < length ? utf8_bytes[index+5] : 0);

            goto error;
        }

        gchar *temp = g_strdup_printf ("%s%c", value, c);
        g_free (value);
        value = temp;
    }

    gchar *normal = g_utf8_normalize (value, -1, G_NORMALIZE_DEFAULT);
    if (!normal)
    {
        g_set_error (error,
                     J2C_UTF8_ERROR,
                     J2C_INVALID_UTF8_ERROR,
                     "Could not normalize utf8 string.");
        goto error;
    }

    g_free (value);

    return normal;

error:
  g_free (value);
  return NULL;
}
