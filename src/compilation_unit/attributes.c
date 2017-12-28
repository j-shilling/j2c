#include <j2c/attributes.h>
#include <j2c/logger.h>

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

  if (FALSE)
    {
     /* TODO: Parse name to attribute */
    }
  else
    {
      g_input_stream_skip (G_INPUT_STREAM (in), length, NULL, &tmp_error);
      if (tmp_error) goto error;

      j2c_logger_warning ("Unrecognized attribute name \'%s\'", name);
    }
error:
  if (tmp_error)
    g_propagate_error (error, tmp_error);
  if (name)
    g_free (name);

  return ret;
}
