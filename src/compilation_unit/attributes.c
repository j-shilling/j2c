#include <j2c/attributes.h>
#include <j2c/compilation-unit-class.h>
#include <j2c/method-or-field-info.h>
#include <j2c/modified-utf8.h>

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
          if (g_strcmp0 (name, J2C_SOURCE_FILE) == 0)
            {
              guint16 source_file_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (!tmp_error)
                ret = g_object_new (J2C_TYPE_ATTRIBUTE_SOURCE_FILE,
                                    J2C_ATTRIBUTE_PROP_SOURCE_FILE_INDEX, source_file_index,
                                    NULL);
            }
          else if (g_strcmp0 (name, J2C_INNER_CLASSES) == 0)
            {
              guint16 number_of_inner_classes =
                g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (!tmp_error)
                {
                  GPtrArray *inner_classes = g_ptr_array_sized_new (number_of_inner_classes);
                  g_ptr_array_set_free_func (inner_classes, g_object_unref);
                  for (gint i = 0; i < number_of_inner_classes; i++)
                    {
                      guint16 inner_class_info_index =
                        g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                      guint16 outer_class_info_index =
                        g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                      guint16 inner_name_index =
                        g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                      guint16 inner_class_access_flags =
                        g_data_input_stream_read_uint16 (in, NULL, &tmp_error);

                      if (!tmp_error)
                        {
                          J2cInnerClassInfo *info = g_object_new (J2C_TYPE_INNER_CLASS_INFO,
                                                                  J2C_ATTRIBUTE_PROP_INNER_CLASS_INFO_INDEX, inner_class_info_index,
                                                                  J2C_ATTRIBUTE_PROP_OUTER_CLASS_INFO_INDEX, outer_class_info_index,
                                                                  J2C_ATTRIBUTE_PROP_INNER_NAME_INDEX, inner_name_index,
                                                                  J2C_ATTRIBUTE_PROP_INNER_CLASS_ACCESS_FLAGS, inner_class_access_flags,
                                                                  NULL);
                          g_ptr_array_add (inner_classes, info);
                        }
                    }

                    if (tmp_error)
                      g_ptr_array_unref (inner_classes);
                    else
                      ret = g_object_new (J2C_TYPE_ATTRIBUTE_INNER_CLASSES,
                                          J2C_ATTRIBUTE_PROP_CLASSES, inner_classes,
                                          NULL);
                }
            }
           else if (g_strcmp0 (name, J2C_ENCLOSING_METHOD) == 0)
             {
               guint16 class_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
               guint16 method_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);

               if (!tmp_error)
                 ret = g_object_new (J2C_TYPE_ATTRIBUTE_ENCLOSING_METHOD,
                                     J2C_ATTRIBUTE_PROP_CLASS_INDEX, class_index,
                                     J2C_ATTRIBUTE_PROP_METHOD_INDEX, method_index,
                                     NULL);
             }
           else if (g_strcmp0 (name, J2C_SOURCE_DEBUG_EXTENSION) == 0)
             {
               guint8 *utf8_bytes = g_malloc (length);

               if (length == g_input_stream_read (G_INPUT_STREAM (in),
                                                   utf8_bytes,
                                                   length,
                                                   NULL,
                                                   &tmp_error))
                 {
                   gchar *string = j2c_modified_utf8_to_standard (utf8_bytes, length, &tmp_error);
                   if (!tmp_error)
                     {
                       ret = g_object_new (J2C_TYPE_ATTRIBUTE_SOURCE_DEBUG_EXTENSION,
                                           J2C_ATTRIBUTE_PROP_DEBUG_EXTENSION, string,
                                           NULL);
                     }
                   if (string) g_free (string);
                 }
             }
            else if (g_strcmp0 (name, J2C_BOOTSTRAP_METHODS) == 0)
              {
                guint16 num_bootstrap_methods = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                if (!tmp_error)
                  {
                    GPtrArray *bootstrap_methods = g_ptr_array_sized_new (num_bootstrap_methods);
                    g_ptr_array_set_free_func (bootstrap_methods, g_object_unref);
                    for (gint i = 0; i < num_bootstrap_methods; i++)
                      {
                        guint16 bootstrap_method_ref =
                          g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                        guint16 num_bootstrap_arguments =
                          g_data_input_stream_read_uint16 (in, NULL, &tmp_error);

                        GArray *bootstrap_arguments = g_array_sized_new (TRUE,
                                                                         FALSE,
                                                                         sizeof (guint16),
                                                                         num_bootstrap_arguments);
                        for (gint j = 0; j < num_bootstrap_arguments; j++)
                          {
                            guint16 argument =
                              g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                            g_array_append_val (bootstrap_arguments, argument);
                          }
                        J2cBootstrapMethodInfo *info = g_object_new (J2C_TYPE_BOOTSTRAP_METHOD_INFO,
                                                                     J2C_ATTRIBUTE_PROP_BOOTSTRAP_METHOD_REF, bootstrap_method_ref,
                                                                     J2C_ATTRIBUTE_PROP_BOOTSTRAP_ARGUMENTS, bootstrap_arguments,
                                                                     NULL);
                        g_ptr_array_add (bootstrap_methods, info);
                      }

                      ret = g_object_new (J2C_TYPE_ATTRIBUTE_BOOTSTRAP_METHODS,
                                          J2C_ATTRIBUTE_PROP_BOOTSTRAP_METHODS, bootstrap_methods,
                                          NULL);
                  }
              }
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
          if (g_strcmp0 (name, J2C_CONSTANT_VALUE) == 0)
            {
              guint16 constant_value_index =
                g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (!tmp_error)
                ret = g_object_new (J2C_TYPE_ATTRIBUTE_CONSTANT_VALUE,
                                    J2C_ATTRIBUTE_PROP_CONSTANT_VALUE_INDEX, constant_value_index,
                                    NULL);
            }
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
                     "Unknown attribute name \'%s\'",
                     name);
    }

error:
  if (tmp_error)
    g_propagate_error (error, tmp_error);
  if (name)
    g_free (name);

  return ret;
}
