#include <j2c/attributes.h>
#include <j2c/compilation-unit-class.h>
#include <j2c/method-or-field-info.h>

static gpointer
j2c_read_attribute_any (gchar *name, GDataInputStream *in, const guint16 length, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (in != NULL, NULL);

  GError *tmp_error = NULL;

  if (tmp_error) g_propagate_error (error, tmp_error);
  return NULL;
}

static gpointer
j2c_read_attribute_class_field_or_method (gchar *name, GDataInputStream *in, const guint16 length, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (in != NULL, NULL);

  GError *tmp_error = NULL;

  gpointer ret = j2c_read_attribute_any (name, in, length, &tmp_error);

  if (!ret && !tmp_error)
    {

    }

  if (tmp_error) g_propagate_error (error, tmp_error);
  return ret;
}

static gpointer
j2c_read_attribute_class (gchar *name, GDataInputStream *in, const guint16 length, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (in != NULL, NULL);

  GError *tmp_error = NULL;

  gpointer ret = j2c_read_attribute_any (name, in, length, &tmp_error);

  if (!ret && !tmp_error)
    {
      ret = j2c_read_attribute_class_field_or_method (name, in, length, &tmp_error);
      if (!ret && !tmp_error)
        {

        }
    }

  if (tmp_error) g_propagate_error (error, tmp_error);
  return ret;
}

static gpointer
j2c_read_attribute_field (gchar *name, GDataInputStream *in, const guint16 length, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (in != NULL, NULL);

  GError *tmp_error = NULL;

  gpointer ret = j2c_read_attribute_any (name, in, length, &tmp_error);

  if (!ret && !tmp_error)
    {
      ret = j2c_read_attribute_class_field_or_method (name, in, length, &tmp_error);
      if (!ret && !tmp_error)
        {

        }
    }

  if (tmp_error) g_propagate_error (error, tmp_error);
  return ret;
}

static gpointer
j2c_read_attribute_method (gchar *name, GDataInputStream *in, const guint16 length, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (in != NULL, NULL);

  GError *tmp_error = NULL;

  gpointer ret = j2c_read_attribute_any (name, in, length, &tmp_error);

  if (!ret && !tmp_error)
    {
      ret = j2c_read_attribute_class_field_or_method (name, in, length, &tmp_error);
      if (!ret && !tmp_error)
        {

        }
    }

  if (tmp_error) g_propagate_error (error, tmp_error);
  return ret;
}

static gpointer
j2c_read_attribute_code (gchar *name, GDataInputStream *in, const guint16 length, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (in != NULL, NULL);

  GError *tmp_error = NULL;

  gpointer ret = j2c_read_attribute_any (name, in, length, &tmp_error);

  if (!ret && !tmp_error)
    {

    }

  if (tmp_error) g_propagate_error (error, tmp_error);
  return ret;
}

gpointer
j2c_read_attribute (GType type, GDataInputStream *in, J2cConstantPool *cp, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (in != NULL, NULL);
  g_return_val_if_fail (cp != NULL, NULL);

  GError *tmp_error = NULL;
  gchar *name = NULL;
  gpointer ret = NULL;

  guint16 name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;
  name = j2c_constant_pool_get_string (cp, name_index, &tmp_error);
  if (tmp_error) goto error;

  guint32 length = g_data_input_stream_read_uint32 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  if (type == J2C_TYPE_COMPILATION_UNIT_CLASS)
    {
      ret = j2c_read_attribute_class (name, in, length, &tmp_error);
    }
  else if (type == J2C_TYPE_FIELD_INFO)
    {
      ret = j2c_read_attribute_field (name, in, length, &tmp_error);
    }
  else if (type == J2C_TYPE_METHOD_INFO)
    {
      ret = j2c_read_attribute_method (name, in, length, &tmp_error);
    }
  else if (type == J2C_TYPE_ATTRIBUTE_CODE)
    {
      ret = j2c_read_attribute_code (name, in, length, &tmp_error);
    }

  if (!ret)
    {
      g_input_stream_skip (G_INPUT_STREAM (in), length, NULL, &tmp_error);
      if (tmp_error) goto error;

      if (!tmp_error)
        g_set_error (error,
                     J2C_ATTRIBUTE_ERROR,
                     J2C_UNKNOWN_ATTRIBUTE_ERROR,
                     "Unknown attribute name \'%s\'.",
                     name);
    }

error:
  if (tmp_error)
    g_propagate_error (error, tmp_error);
  if (name)
    g_free (name);

  return ret;
}
