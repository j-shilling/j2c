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

gboolean j2c_descriptor_is_reference_type (gchar *type_descriptor);
gchar *j2c_descriptor_get_reference_java_name (gchar *type_descriptor);
gboolean j2c_descriptor_is_array_type (gchar *type_descriptor);
gchar *j2c_descriptor_get_array_content_type (gchar *type_descriptor);

gboolean j2c_descriptor_is_fundamental_type (gchar *type_descriptor);
gchar *j2c_descriptor_get_fundamental_type (J2cFundamentalType const type);

/* Type Descriptor */
typedef gchar * J2cTypeDescriptor;
gboolean j2c_descriptor_is_type_descriptor (gchar *str);
J2cTypeDescriptor j2c_descriptor_get_next_type_descriptor (gchar *str);

/* Method Descriptor */
typedef gchar * J2cMethodDescriptor;
J2cTypeDescriptor j2c_descriptor_get_return_type (J2cMethodDescriptor descriptor);
J2cTypeDescriptor *j2c_descriptor_get_params (J2cMethodDescriptor descriptor);



#endif // __DESCRIPTOR_H__
