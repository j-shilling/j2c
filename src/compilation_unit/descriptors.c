#include <j2c/descriptors.h>

#include <string.h>

gboolean
j2c_descriptor_is_reference_type (gchar *type_descriptor)
{
  g_return_val_if_fail (type_descriptor != NULL, FALSE);
  g_return_val_if_fail (*type_descriptor != '\0', FALSE);

  size_t len = strlen (type_descriptor);
  for (int i = 0 ; i < len; i++)
    if (type_descriptor[i] == ';')
      return type_descriptor[0] == 'L';

  return FALSE;
}

gchar *
j2c_descriptor_get_reference_java_name (gchar *type_descriptor)
{
  g_return_val_if_fail (type_descriptor != NULL, NULL);
  g_return_val_if_fail (*type_descriptor != '\0', NULL);

  size_t len =  strlen (type_descriptor);
  g_return_val_if_fail (type_descriptor[len-1] == ';', NULL);

  gchar *ret = g_malloc0 (len - 1);
  memcpy (ret, type_descriptor + 1, len - 2);

  return ret;
}

gboolean
j2c_descriptor_is_array_type (gchar *type_descriptor)
{
  g_return_val_if_fail (type_descriptor != NULL, FALSE);
  g_return_val_if_fail (*type_descriptor != '\0', FALSE);

  return type_descriptor[0] == '[';
}

gchar *
j2c_descriptor_get_array_content_type (gchar *type_descriptor)
{
  g_return_val_if_fail (type_descriptor != NULL, NULL);
  g_return_val_if_fail (*type_descriptor != '\0', NULL);

  size_t len = strlen (type_descriptor);
  gchar *ret = g_malloc0 (len);
  memcpy (ret, type_descriptor + 1, len - 1);

  return ret;
}

gboolean
j2c_descriptor_is_fundamental_type (gchar *type_descriptor)
{
  g_return_val_if_fail (type_descriptor != NULL, FALSE);
  g_return_val_if_fail (*type_descriptor != '\0', FALSE);

  return     type_descriptor[0] == (gchar) J2C_BYTE
          || type_descriptor[0] == (gchar) J2C_CHAR
          || type_descriptor[0] == (gchar) J2C_DOUBLE
          || type_descriptor[0] == (gchar) J2C_FLOAT
          || type_descriptor[0] == (gchar) J2C_INT
          || type_descriptor[0] == (gchar) J2C_LONG
          || type_descriptor[0] == (gchar) J2C_SHORT
          || type_descriptor[0] == (gchar) J2C_BOOLEAN
          || type_descriptor[0] == (gchar) J2C_VOID;
}

gchar *
j2c_descriptor_get_fundamental_type (J2cFundamentalType const type)
{

  gchar *ret = g_malloc (2);
  ret[0] = (gchar) type;
  ret[1] = '\0';

  return ret;
}

gboolean
j2c_descriptor_is_type_descriptor (gchar *str)
{
  g_return_val_if_fail (str != NULL, FALSE);
  g_return_val_if_fail (*str != '\0', FALSE);

  return     str[0] == (gchar) J2C_BYTE
          || str[0] == (gchar) J2C_CHAR
          || str[0] == (gchar) J2C_DOUBLE
          || str[0] == (gchar) J2C_FLOAT
          || str[0] == (gchar) J2C_INT
          || str[0] == (gchar) J2C_LONG
          || str[0] == (gchar) J2C_SHORT
          || str[0] == (gchar) J2C_BOOLEAN
          || str[0] == (gchar) J2C_ARRAY
          || str[0] == (gchar) J2C_REF
          || str[0] == (gchar) J2C_VOID;
}

J2cTypeDescriptor
j2c_descriptor_get_next_type_descriptor (gchar *str)
{
  g_return_val_if_fail (str != NULL, NULL);
  g_return_val_if_fail (*str != '\0', NULL);

  if (j2c_descriptor_is_fundamental_type (str))
    {
      gchar *ret = g_malloc (2);
      ret [0] = str[0];
      ret [1] = '\0';

      return (J2cTypeDescriptor) ret;
    }

  if (j2c_descriptor_is_array_type (str))
    {
      J2cTypeDescriptor content_type = j2c_descriptor_get_next_type_descriptor (str + 1);
      if (!content_type)
        return NULL;

      size_t len = strlen (content_type);
      gchar *ret = g_malloc0 (len + 1);

      ret[0] = (gchar) J2C_ARRAY;
      memcpy (ret + 1, content_type, len);
      g_free (content_type);

      return (J2cTypeDescriptor) ret;
    }

  if (j2c_descriptor_is_reference_type (str))
    {
      gint i = 0;
      while (str[i] != ';' && str[i] != '\0')
        i++;

      if (str[i] == '\0')
        return NULL;

      gchar *ret = g_malloc0 (i + 2);
      memcpy (ret, str, i + 1);

      return (J2cTypeDescriptor) ret;
    }
}

J2cTypeDescriptor
j2c_descriptor_get_return_type (J2cMethodDescriptor descriptor)
{
  g_return_val_if_fail (descriptor && descriptor[0] == '(', NULL);

  gint i = 0;
  while (descriptor[i] != ')' && descriptor[i] != '\0')
    i++;

  g_return_val_if_fail (descriptor[i] != '\0' && descriptor[i+1] != '\0', NULL);
  i++;

  return j2c_descriptor_get_next_type_descriptor (descriptor + i);
}

J2cTypeDescriptor *
j2c_descriptor_get_params (J2cMethodDescriptor descriptor)
{
  g_return_val_if_fail (descriptor && descriptor[0] == '(', NULL);

  size_t len = 0;
  gchar **ret = g_malloc0 (sizeof (gchar *) * (len + 1));
  gint i = 1;
  while (descriptor[i] != ')' && i < strlen (descriptor))
    {
      gchar *type = j2c_descriptor_get_next_type_descriptor (descriptor + i);
      if (!type)
        return ret;

      len ++;
      gchar **tmp = g_malloc0 (sizeof (gchar *) * (len + 1));

      for (gint j = 0; j < (len - 1); j ++)
        tmp[j] = ret[j];

      tmp[len - 1] = type;
      g_free (ret);
      ret = tmp;

      i += strlen (type);
    }

  return ret;
}
