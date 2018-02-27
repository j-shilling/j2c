#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include <glib.h>

typedef enum
{
  J2C_BYTE    = 'B',
  J2C_CHAR    = 'C',
  J2C_DOUBLE  = 'D',
  J2C_FLOAT   = 'F',
  J2C_INT     = 'I',
  J2C_LONG    = 'J',
  J2C_SHORT   = 'S',
  J2C_BOOLEAN = 'Z',
  J2C_ARRAY   = '[',
  J2C_REF     = 'L',

  J2C_VOID    = 'V'
} J2cFundamentalType;

gboolean j2c_descriptor_is_reference_type (const gchar *type_descriptor);
gchar *j2c_descriptor_get_reference_java_name (const gchar *type_descriptor);
gboolean j2c_descriptor_is_array_type (const gchar *type_descriptor);
gchar *j2c_descriptor_get_array_content_type (const gchar *type_descriptor);

gboolean j2c_descriptor_is_fundamental_type (const gchar *type_descriptor);
gchar *j2c_descriptor_get_fundamental_type (J2cFundamentalType const type);

/* Type Descriptor */
gboolean j2c_descriptor_is_type_descriptor (const gchar *str);
gchar * j2c_descriptor_get_next_type_descriptor (const gchar *str);

/* Method Descriptor */
gchar  *j2c_descriptor_get_return_type (const gchar * descriptor);
gchar **j2c_descriptor_get_params (const gchar * descriptor);



#endif // __DESCRIPTOR_H__
