#include <j2c/attributes.h>
#include <j2c/compilation-unit-class.h>
#include <j2c/method-or-field-info.h>
#include <j2c/modified-utf8.h>
#include <j2c/logger.h>

static gpointer
j2c_read_attribute_any (gchar *name, GDataInputStream *in, const guint16 length, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (length == 0 || in != NULL, NULL);

  gpointer ret = NULL;
  GError *tmp_error = NULL;

  if (g_strcmp0 (name, J2C_RUNTIME_VISIBLE_TYPE_ANNOTATIONS) == 0 || g_strcmp0 (name, J2C_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS) == 0)
    {
      guint16 num_annotations = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
      if (tmp_error) goto end;

      GPtrArray *annotations = g_ptr_array_sized_new (num_annotations);
      g_ptr_array_set_free_func (annotations, g_object_unref);

      for (gint i = 0; i < num_annotations; i ++)
        {
          J2cTargetInfo *target_info = NULL;

          guint8 target_type = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
          if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

          if (target_type <= 0x01) /* type_parameter_target */
            {
              guint8 type_parameter_index = g_data_input_stream_read_byte (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

              target_info = g_object_new (J2C_TYPE_TARGET_INFO,
                                          J2C_ATTRIBUTE_PROP_TARGET_TYPE, target_type,
                                          J2C_ATTRIBUTE_PROP_TYPE_PARAMETER_INDEX, type_parameter_index,
                                          NULL);
            }
          else if (target_type == 0x10) /* supertype_target */
            {
              guint16 supertype_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

              target_info = g_object_new (J2C_TYPE_TARGET_INFO,
                                          J2C_ATTRIBUTE_PROP_TARGET_TYPE, target_type,
                                          J2C_ATTRIBUTE_PROP_SUPERTYPE_INDEX, supertype_index,
                                          NULL);
            }
          else if (target_type >= 0x11 && target_type <= 0x12) /* type_parameter_bound_target */
            {
              guint8 type_parameter_index = g_data_input_stream_read_byte (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }
              guint8 bound_index = g_data_input_stream_read_byte (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

              target_info = g_object_new (J2C_TYPE_TARGET_INFO,
                                          J2C_ATTRIBUTE_PROP_TARGET_TYPE, target_type,
                                          J2C_ATTRIBUTE_PROP_TYPE_PARAMETER_INDEX, type_parameter_index,
                                          J2C_ATTRIBUTE_PROP_BOUND_INDEX, bound_index,
                                          NULL);
            }
          else if (target_type >= 0x13 && target_type <= 0x15) /* empty_target */
            {
              /* Do Nothing */
            }
          else if (target_type == 0x16) /* formal_parameter_target */
            {
              guint8 formal_parameter_index = g_data_input_stream_read_byte (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

              target_info = g_object_new (J2C_TYPE_TARGET_INFO,
                                          J2C_ATTRIBUTE_PROP_TARGET_TYPE, target_type,
                                          J2C_ATTRIBUTE_PROP_FORMAL_PARAMETER_INDEX, formal_parameter_index,
                                          NULL);
            }
          else if (target_type == 0x17) /* throws_target */
            {
              guint16 throws_type_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

              target_info = g_object_new (J2C_TYPE_TARGET_INFO,
                                          J2C_ATTRIBUTE_PROP_TARGET_TYPE, target_type,
                                          J2C_ATTRIBUTE_PROP_THROWS_TYPE_INDEX, throws_type_index,
                                          NULL);
            }
          else if (target_type >= 0x40 && target_type <= 0x41) /* localvar_target */
            {
              guint16 table_length = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }
              GPtrArray *table = g_ptr_array_sized_new (table_length);
              g_ptr_array_set_free_func (table, g_object_unref);

              for (gint j = 0; j < table_length; j++)
                {
                  guint16 start_pc = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (annotations); g_ptr_array_unref (table); goto end; }
                  guint16 length = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (annotations); g_ptr_array_unref (table); goto end; }
                  guint16 index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (annotations); g_ptr_array_unref (table); goto end; }

                  J2cLocalvar *localvar = g_object_new (J2C_TYPE_LOCALVAR,
                                                        J2C_ATTRIBUTE_PROP_START_PC, start_pc,
                                                        J2C_ATTRIBUTE_PROP_LENGTH, length,
                                                        J2C_ATTRIBUTE_PROP_INDEX, index,
                                                        NULL);
                  g_ptr_array_add (table, localvar);
                }

              target_info = g_object_new (J2C_TYPE_TARGET_INFO,
                                          J2C_ATTRIBUTE_PROP_TARGET_TYPE, target_type,
                                          J2C_ATTRIBUTE_PROP_LOCALVAR_TARGET, table,
                                          NULL);
            }
          else if (target_type == 0x42) /* catch_target */
            {
              guint16 exception_table_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

              target_info = g_object_new (J2C_TYPE_TARGET_INFO,
                                          J2C_ATTRIBUTE_PROP_TARGET_TYPE, target_type,
                                          J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE_INDEX, exception_table_index,
                                          NULL);
            }
          else if (target_type >= 0x43 && target_type <= 0x46) /* offset_target */
            {
              guint16 offset = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

              target_info = g_object_new (J2C_TYPE_TARGET_INFO,
                                          J2C_ATTRIBUTE_PROP_TARGET_TYPE, target_type,
                                          J2C_ATTRIBUTE_PROP_OFFSET, offset,
                                          NULL);
            }
          else if (target_type >= 0x47 && target_type <= 0x4B) /* type_argument_target */
            {
              guint16 offset = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }
              guint8 type_argument_index = g_data_input_stream_read_byte (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

              target_info = g_object_new (J2C_TYPE_TARGET_INFO,
                                          J2C_ATTRIBUTE_PROP_TARGET_TYPE, target_type,
                                          J2C_ATTRIBUTE_PROP_OFFSET, offset,
                                          J2C_ATTRIBUTE_PROP_TYPE_ARGUMENT_INDEX, type_argument_index,
                                          NULL);
            }
          else /* invalid target */
            {
              g_ptr_array_unref (annotations);
              goto end;
            }

          guint8 path_length = g_data_input_stream_read_byte (in, NULL, &tmp_error);
          if (tmp_error) { g_ptr_array_unref (annotations); goto end; }
          GPtrArray *path = g_ptr_array_sized_new (path_length);
          g_ptr_array_set_free_func (path, g_object_unref);

          for (gint j = 0; j < path_length; j++)
            {
              guint8 type_path_kind = g_data_input_stream_read_byte (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); g_ptr_array_unref (path); goto end; }
              guint8 type_argument_index = g_data_input_stream_read_byte (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); g_ptr_array_unref (path); goto end; }

              J2cPath *item = g_object_new (J2C_TYPE_PATH,
                                              J2C_ATTRIBUTE_PROP_TYPE_PATH_KIND, type_path_kind,
                                              J2C_ATTRIBUTE_PROP_TYPE_ARGUMENT_INDEX, type_argument_index,
                                              NULL);
              g_ptr_array_add (path, item);
            }

          J2cTPath *target_path = g_object_new (J2C_TYPE_T_PATH,
                                                J2C_ATTRIBUTE_PROP_PATH, path,
                                                NULL);

          guint16 type_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
          if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

          guint16 num_element_value_pairs = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
          if (tmp_error) { g_ptr_array_unref (annotations); goto end; }
          GPtrArray *element_value_pairs = g_ptr_array_sized_new (num_element_value_pairs);
          g_ptr_array_set_free_func (element_value_pairs, g_object_unref);
          for (gint j = 0; j < num_element_value_pairs; j++)
            {
              J2cElementValuePair *pair = j2c_element_value_pair_new_from_stream(in, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); g_ptr_array_unref (element_value_pairs); goto end; }

              g_ptr_array_add (element_value_pairs, pair);
            }

          J2cTAnnotation *annotation = g_object_new (J2C_TYPE_T_ANNOTATION,
                                                     J2C_ATTRIBUTE_PROP_TARGET_INFO, target_info,
                                                     J2C_ATTRIBUTE_PROP_TARGET_PATH, target_path,
                                                     J2C_ATTRIBUTE_PROP_TYPE_INDEX, type_index,
                                                     J2C_ATTRIBUTE_PROP_ELEMENT_VALUE_PAIRS, element_value_pairs,
                                                     NULL);
          g_ptr_array_add (annotations, annotation);
        }

      ret = g_object_new (g_strcmp0 (name, J2C_RUNTIME_VISIBLE_TYPE_ANNOTATIONS) == 0 ?
                            J2C_TYPE_ATTRIBUTE_RUNTIME_VISIBLE_TYPE_ANNOTATIONS : J2C_TYPE_ATTRIBUTE_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS,
                          J2C_ATTRIBUTE_PROP_TYPE_ANNOTATIONS, annotations,
                          NULL);
    }

end:
  if (tmp_error) g_propagate_error (error, tmp_error);
  return ret;
}

static gpointer
j2c_read_attribute_class_field_or_method (gchar *name, GDataInputStream *in, const guint16 length, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (length == 0 || in != NULL, NULL);

  GError *tmp_error = NULL;

  gpointer ret = j2c_read_attribute_any (name, in, length, &tmp_error);

  if (!ret && !tmp_error)
    {
      if (g_strcmp0 (name, J2C_SYNTHETIC) == 0)
        {
          ret = g_object_new (J2C_TYPE_ATTRIBUTE_SYNTHETIC, NULL);
        }
      else if (g_strcmp0 (name, J2C_DEPRECATED) == 0)
        {
          ret = g_object_new (J2C_TYPE_ATTRIBUTE_DEPRECATED, NULL);
        }
      else if (g_strcmp0 (name, J2C_SIGNATURE) == 0)
        {
          guint16 signature_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
          if (tmp_error) goto end;

          ret = g_object_new (J2C_TYPE_ATTRIBUTE_SIGNATURE,
                              J2C_ATTRIBUTE_PROP_SIGNATURE_INDEX, signature_index,
                              NULL);
        }
      else if (g_strcmp0 (name, J2C_RUNTIME_VISIBLE_ANNOTATIONS) == 0 || g_strcmp0 (name, J2C_RUNTIME_INVISIBLE_ANNOTATIONS) == 0)
        {
          guint16 num_annotations = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
          if (tmp_error) goto end;

          GPtrArray *annotations = g_ptr_array_sized_new (num_annotations);
          g_ptr_array_set_free_func (annotations, g_object_unref);
          for (gint i = 0; i < num_annotations; i ++)
            {
              J2cAnnotation *annotation = j2c_annotation_new_from_stream(in, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (annotations); goto end; }

              g_ptr_array_add (annotations, annotation);
            }
          ret = g_object_new (g_strcmp0 (name, J2C_RUNTIME_VISIBLE_ANNOTATIONS) == 0 ?
                                J2C_TYPE_ATTRIBUTE_RUNTIME_VISIBLE_ANNOTATIONS : J2C_TYPE_ATTRIBUTE_RUNTIME_INVISIBLE_ANNOTATIONS,
                              J2C_ATTRIBUTE_PROP_ANNOTATIONS, annotations,
                              NULL);
        }
    }

end:
  if (tmp_error) g_propagate_error (error, tmp_error);
  return ret;
}

static gpointer
j2c_read_attribute_class (gchar *name, GDataInputStream *in, const guint16 length, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (length == 0 || in != NULL, NULL);

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
  g_return_val_if_fail (length == 0 || in != NULL, NULL);

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
j2c_read_attribute_method (gchar *name, GDataInputStream *in, const guint16 length, J2cConstantPool *cp, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (length == 0 || in != NULL, NULL);

  GError *tmp_error = NULL;

  gpointer ret = j2c_read_attribute_any (name, in, length, &tmp_error);

  if (!ret && !tmp_error)
    {
      ret = j2c_read_attribute_class_field_or_method (name, in, length, &tmp_error);
      if (!ret && !tmp_error)
        {
          if (g_strcmp0 (name, J2C_CODE) == 0)
            {
              guint16 max_stack = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) goto end;
              guint16 max_locals = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) goto end;
              guint32 code_length = g_data_input_stream_read_uint32 (in, NULL, &tmp_error);
              if (tmp_error) goto end;
              guint8 *bytes = g_malloc (code_length);

              if (code_length != g_input_stream_read (G_INPUT_STREAM (in), bytes, code_length, NULL, &tmp_error))
                {
                  g_free (bytes);
                  goto end;
                }

              GBytes *code = g_bytes_new_take (bytes, code_length);

              guint16 exception_table_length = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) goto end;

              GPtrArray *exception_table = g_ptr_array_sized_new (exception_table_length);
              g_ptr_array_set_free_func (exception_table, g_object_unref);
              for (gint i = 0; i < exception_table_length; i ++)
                {
                  guint16 start_pc = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (exception_table); goto end; }
                  guint16 end_pc = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (exception_table); goto end; }
                  guint16 handler_pc = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (exception_table); goto end; }
                  guint16 catch_type = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (exception_table); goto end; }

                  J2cExceptionInfo *info = g_object_new (J2C_TYPE_EXCEPTION_INFO,
                                                         J2C_ATTRIBUTE_PROP_START_PC, start_pc,
                                                         J2C_ATTRIBUTE_PROP_END_PC, end_pc,
                                                         J2C_ATTRIBUTE_PROP_HANDLER_PC, handler_pc,
                                                         J2C_ATTRIBUTE_PROP_CATCH_TYPE, catch_type,
                                                         NULL);
                   g_ptr_array_add (exception_table, info);
                }

              guint16 attributes_count = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (exception_table); goto end; }
              GPtrArray *attributes = g_ptr_array_sized_new (attributes_count);
              g_ptr_array_set_free_func (attributes, g_object_unref);
              for (gint i = 0; i < attributes_count; i++)
                {
                  gpointer attribute = j2c_read_attribute(J2C_TYPE_ATTRIBUTE_CODE, in, cp, &tmp_error);
                  if (tmp_error)
                    {
                      j2c_logger_warning ("Could not read attribute: %s.", tmp_error->message);
                      g_error_free (tmp_error);
                      tmp_error = NULL;
                    }
                  else
                    {
                      g_ptr_array_add (attributes, attribute);
                    }
                }

              ret = g_object_new (J2C_TYPE_ATTRIBUTE_CODE,
                                  J2C_ATTRIBUTE_PROP_MAX_STACK, max_stack,
                                  J2C_ATTRIBUTE_PROP_MAX_LOCALS, max_locals,
                                  J2C_ATTRIBUTE_PROP_CODE, code,
                                  J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE, exception_table,
                                  J2C_ATTRIBUTE_PROP_ATTRIBUTES, attributes,
                                  NULL);
          }
          else if (g_strcmp0 (name, J2C_EXCEPTIONS) == 0)
            {
              guint16 number_of_exceptions = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) goto end;

              GArray *exception_index_table = g_array_sized_new (TRUE, FALSE, sizeof (guint16), number_of_exceptions);
              for (gint i = 0; i < number_of_exceptions; i ++)
                {
                  guint16 index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error)
                    {
                      g_array_unref (exception_index_table);
                      goto end;
                    }

                  g_array_append_val (exception_index_table, index);
                }

              ret = g_object_new (J2C_TYPE_ATTRIBUTE_EXCEPTIONS,
                                  J2C_ATTRIBUTE_PROP_EXCEPTION_INDEX_TABLE, exception_index_table,
                                  NULL);
            }
          else if (g_strcmp0 (name, J2C_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS) == 0 || g_strcmp0 (name, J2C_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS) == 0)
            {
              guint16 num_parameters = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) goto end;

              GPtrArray *parameter_annotations = g_ptr_array_sized_new (num_parameters);
              g_ptr_array_set_free_func (parameter_annotations, g_object_unref);
              for (gint i = 0; i < num_parameters; i ++)
                {
                  guint16 num_annotations = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error)
                    {
                      g_ptr_array_unref (parameter_annotations);
                      goto end;
                    }
                  GPtrArray *annotations = g_ptr_array_sized_new (num_annotations);
                  g_ptr_array_set_free_func (annotations, g_object_unref);
                  for (gint j = 0; j < num_annotations; j++)
                    {
                      J2cAnnotation *annotation = j2c_annotation_new_from_stream (in, &tmp_error);
                      if (tmp_error)
                        {
                          g_ptr_array_unref (parameter_annotations);
                          g_ptr_array_unref (annotations);
                          goto end;
                        }
                      g_ptr_array_add (annotations, annotation);
                    }
                  J2cParameterAnnotations *annos = g_object_new (J2C_TYPE_PARAMETER_ANNOTATIONS,
                                                                 J2C_ATTRIBUTE_PROP_ANNOTATIONS, annotations,
                                                                 NULL);
                  g_ptr_array_add (parameter_annotations, annos);
                }
                ret = g_object_new (g_strcmp0 (name, J2C_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS) ?
                                      J2C_TYPE_ATTRIBUTE_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS : J2C_TYPE_ATTRIBUTE_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS,
                                    J2C_ATTRIBUTE_PROP_PARAMETER_ANNOTATIONS, parameter_annotations,
                                    NULL);
            }
          else if (g_strcmp0 (name, J2C_ANNOTATION_DEFAULT) == 0)
            {
              J2cElementValue *default_value = j2c_element_value_new_from_stream(in, &tmp_error);
              if (tmp_error) goto end;

              ret = g_object_new (J2C_TYPE_ATTRIBUTE_ANNOTATION_DEFAULT,
                                  J2C_ATTRIBUTE_PROP_DEFAULT_VALUE, default_value,
                                  NULL);
              g_object_unref (default_value);
            }
          else if (g_strcmp0 (name, J2C_METHOD_PARAMETERS) == 0)
            {
              guint16 parameters_count = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) goto end;

              GPtrArray *parameters = g_ptr_array_sized_new (parameters_count);
              g_ptr_array_set_free_func (parameters, g_object_unref);
              for (gint i = 0; i < parameters_count; i ++)
                {
                  guint16 name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (parameters); goto end; }
                  guint16 access_flags = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (parameters); goto end; }

                  J2cParameter *parameter = g_object_new (J2C_TYPE_PARAMETER,
                                                          J2C_ATTRIBUTE_PROP_NAME_INDEX, name_index,
                                                          J2C_ATTRIBUTE_PROP_ACCESS_FLAGS, access_flags,
                                                          NULL);
                  g_ptr_array_add (parameters, parameter);
                }
              ret = g_object_new (J2C_TYPE_ATTRIBUTE_METHOD_PARAMETERS,
                                  J2C_ATTRIBUTE_PROP_PARAMETERS, parameters,
                                  NULL);
            }
        }
    }

end:
  if (tmp_error) g_propagate_error (error, tmp_error);
  return ret;
}

static gpointer
j2c_read_attribute_code (gchar *name, GDataInputStream *in, const guint16 length, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (name != NULL && *name != '\0', NULL);
  g_return_val_if_fail (length == 0 || in != NULL, NULL);

  GError *tmp_error = NULL;

  gpointer ret = j2c_read_attribute_any (name, in, length, &tmp_error);

  if (!ret && !tmp_error)
    {
      if (g_strcmp0 (name, J2C_LINE_NUMBER_TABLE) == 0)
        {
          guint16 line_number_table_length = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
          if (tmp_error) goto end;
          GPtrArray *line_number_table = g_ptr_array_sized_new (line_number_table_length);
          g_ptr_array_set_free_func (line_number_table, g_object_unref);
          for (gint i = 0; i < line_number_table_length; i++)
            {
              guint16 start_pc = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (line_number_table); goto end; }
              guint16 line_number = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (line_number_table); goto end; }

              J2cLineNumber *entry = g_object_new (J2C_TYPE_LINE_NUMBER,
                                                   J2C_ATTRIBUTE_PROP_START_PC, start_pc,
                                                   J2C_ATTRIBUTE_PROP_LINE_NUMBER, line_number,
                                                   NULL);
              g_ptr_array_add (line_number_table, entry);
            }
           ret = g_object_new (J2C_TYPE_ATTRIBUTE_LINE_NUMBER_TABLE,
                               J2C_ATTRIBUTE_PROP_LINE_NUMBER_TABLE, line_number_table,
                               NULL);
        }
      else if (g_strcmp0 (name, J2C_LOCAL_VARIABLE_TABLE) == 0)
        {
          guint16 local_variable_table_length = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
          if (tmp_error) goto end;
          GPtrArray *local_variable_table = g_ptr_array_sized_new (local_variable_table_length);
          g_ptr_array_set_free_func (local_variable_table, g_object_unref);
          for (gint i = 0; i < local_variable_table_length; i++)
            {
              guint16 start_pc = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (local_variable_table); goto end; }
              guint16 length = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (local_variable_table); goto end; }
              guint16 name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (local_variable_table); goto end; }
              guint16 descriptor_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (local_variable_table); goto end; }
              guint16 index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (local_variable_table); goto end; }

              J2cLocalVariable *entry = g_object_new (J2C_TYPE_LOCAL_VARIABLE,
                                                      J2C_ATTRIBUTE_PROP_START_PC, start_pc,
                                                      J2C_ATTRIBUTE_PROP_LENGTH, length,
                                                      J2C_ATTRIBUTE_PROP_NAME_INDEX, name_index,
                                                      J2C_ATTRIBUTE_PROP_DESCRIPTOR_INDEX, descriptor_index,
                                                      J2C_ATTRIBUTE_PROP_INDEX, index,
                                                      NULL);
              g_ptr_array_add (local_variable_table, entry);
            }
          ret = g_object_new (J2C_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TABLE,
                              J2C_ATTRIBUTE_PROP_LOCAL_VARIABLE_TABLE, local_variable_table,
                              NULL);
        }
      else if (g_strcmp0 (name, J2C_LOCAL_VARIABLE_TYPE_TABLE) == 0)
        {
          guint16 local_variable_type_table_length = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
          if (tmp_error) goto end;
          GPtrArray *local_variable_type_table = g_ptr_array_sized_new (local_variable_type_table_length);
          g_ptr_array_set_free_func (local_variable_type_table, g_object_unref);
          for (gint i = 0; i < local_variable_type_table_length; i++)
            {
              guint16 start_pc = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (local_variable_type_table); goto end; }
              guint16 length = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (local_variable_type_table); goto end; }
              guint16 name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (local_variable_type_table); goto end; }
              guint16 signature_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (local_variable_type_table); goto end; }
              guint16 index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (local_variable_type_table); goto end; }

              J2cLocalVariable *entry = g_object_new (J2C_TYPE_LOCAL_VARIABLE_TYPE,
                                                      J2C_ATTRIBUTE_PROP_START_PC, start_pc,
                                                      J2C_ATTRIBUTE_PROP_LENGTH, length,
                                                      J2C_ATTRIBUTE_PROP_NAME_INDEX, name_index,
                                                      J2C_ATTRIBUTE_PROP_SIGNATURE_INDEX, signature_index,
                                                      J2C_ATTRIBUTE_PROP_INDEX, index,
                                                      NULL);
              g_ptr_array_add (local_variable_type_table, entry);
            }
          ret = g_object_new (J2C_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TABLE,
                              J2C_ATTRIBUTE_PROP_LOCAL_VARIABLE_TABLE, local_variable_type_table,
                              NULL);
        }
      else if (g_strcmp0 (name, J2C_STACK_MAP_TABLE) == 0)
        {
          guint16 number_of_entries = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
          if (tmp_error) goto end;
          GPtrArray *entries = g_ptr_array_sized_new (number_of_entries);
          g_ptr_array_set_free_func (entries, g_object_unref);
          for (gint i = 0; i < number_of_entries; i ++)
            {
              guint8 tag = g_data_input_stream_read_byte (in, NULL, &tmp_error);
              if (tmp_error) { g_ptr_array_unref (entries); goto end; }

              if (tag >= 0 && tag <= 63) /* SAME_FRAME */
                {
                  J2cStackMapFrame *frame = g_object_new (J2C_TYPE_STACK_MAP_FRAME,
                                                          J2C_ATTRIBUTE_PROP_TAG, tag,
                                                          NULL);
                  g_ptr_array_add (entries, frame);
                }
              else if (tag >= 64 && tag <= 127) /* SAME_LOCALS_1_STACK_ITEM_FRAME */
                {
                  GPtrArray *stack = g_ptr_array_sized_new (1);
                  g_ptr_array_set_free_func (stack, g_object_unref);
                  guint8 verification_tag = g_data_input_stream_read_byte (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (stack); goto end; }

                  if (verification_tag == 7 || verification_tag == 8) /* ITEM_Object || ITEM_Unititialized */
                    {
                      guint16 index_or_offset = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                      if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (stack); goto end; }

                      J2cVerificationTypeInfo *info = g_object_new (J2C_TYPE_VERIFICATION_TYPE_INFO,
                                                                    J2C_ATTRIBUTE_PROP_TAG, verification_tag,
                                                                    tag == 7 ?
                                                                      J2C_ATTRIBUTE_PROP_CPOOL_INDEX : J2C_ATTRIBUTE_PROP_OFFSET,
                                                                    index_or_offset,
                                                                    NULL);
                      g_ptr_array_add (stack, info);
                    }
                  else
                    {
                      J2cVerificationTypeInfo *info = g_object_new (J2C_TYPE_VERIFICATION_TYPE_INFO,
                                                                    J2C_ATTRIBUTE_PROP_TAG, verification_tag,
                                                                    NULL);
                      g_ptr_array_add (stack, info);
                    }

                  J2cStackMapFrame *frame = g_object_new (J2C_TYPE_STACK_MAP_FRAME,
                                                          J2C_ATTRIBUTE_PROP_TAG, tag,
                                                          J2C_ATTRIBUTE_PROP_STACK, stack,
                                                          NULL);
                  g_ptr_array_add (entries, frame);
                }
              else if (tag == 247) /* SAME_LOCALS_1_STACK_ITEM_FRAME_EXTENDED */
                {
                  guint16 offset_delta = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (entries); goto end; }
                  GPtrArray *stack = g_ptr_array_sized_new (1);
                  g_ptr_array_set_free_func (stack, g_object_unref);
                  guint8 verification_tag = g_data_input_stream_read_byte (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (stack); goto end; }

                  if (verification_tag == 7 || verification_tag == 8) /* ITEM_Object || ITEM_Unititialized */
                    {
                      guint16 index_or_offset = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                      if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (stack); goto end; }

                      J2cVerificationTypeInfo *info = g_object_new (J2C_TYPE_VERIFICATION_TYPE_INFO,
                                                                    J2C_ATTRIBUTE_PROP_TAG, verification_tag,
                                                                    tag == 7 ?
                                                                      J2C_ATTRIBUTE_PROP_CPOOL_INDEX : J2C_ATTRIBUTE_PROP_OFFSET,
                                                                    index_or_offset,
                                                                    NULL);
                      g_ptr_array_add (stack, info);
                    }
                  else
                    {
                      J2cVerificationTypeInfo *info = g_object_new (J2C_TYPE_VERIFICATION_TYPE_INFO,
                                                                    J2C_ATTRIBUTE_PROP_TAG, verification_tag,
                                                                    NULL);
                      g_ptr_array_add (stack, info);
                    }

                  J2cStackMapFrame *frame = g_object_new (J2C_TYPE_STACK_MAP_FRAME,
                                                          J2C_ATTRIBUTE_PROP_TAG, tag,
                                                          J2C_ATTRIBUTE_PROP_OFFSET_DELTA, offset_delta,
                                                          J2C_ATTRIBUTE_PROP_STACK, stack,
                                                          NULL);
                  g_ptr_array_add (entries, frame);
                }
              else if ((tag >= 248 && tag <= 250) || tag == 251)/* CHOP_FAME || SAME_FRAME_EXTENED */
                {
                  guint16 offset_delta = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (entries); goto end; }
                  J2cStackMapFrame *frame = g_object_new (J2C_TYPE_STACK_MAP_FRAME,
                                                          J2C_ATTRIBUTE_PROP_TAG, tag,
                                                          J2C_ATTRIBUTE_PROP_OFFSET_DELTA, offset_delta,
                                                          NULL);
                  g_ptr_array_add (entries, frame);
                }
              else if (tag >= 252 && tag <= 254) /* APPEND_FRAME */
                {
                  guint16 offset_delta = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (entries); goto end; }

                  guint16 number_of_locals = tag - 251;

                  GPtrArray *locals = g_ptr_array_sized_new (number_of_locals);
                  g_ptr_array_set_free_func (locals, g_object_unref);

                  for (gint i = 0; i < number_of_locals; i++)
                    {
                      guint8 verification_tag = g_data_input_stream_read_byte (in, NULL, &tmp_error);
                      if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (locals); goto end; }

                      if (verification_tag == 7 || verification_tag == 8) /* ITEM_Object || ITEM_Unititialized */
                        {
                          guint16 index_or_offset = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                          if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (locals); goto end; }

                          J2cVerificationTypeInfo *info = g_object_new (J2C_TYPE_VERIFICATION_TYPE_INFO,
                                                                        J2C_ATTRIBUTE_PROP_TAG, verification_tag,
                                                                        tag == 7 ?
                                                                          J2C_ATTRIBUTE_PROP_CPOOL_INDEX : J2C_ATTRIBUTE_PROP_OFFSET,
                                                                        index_or_offset,
                                                                        NULL);
                          g_ptr_array_add (locals, info);
                        }
                      else
                        {
                          J2cVerificationTypeInfo *info = g_object_new (J2C_TYPE_VERIFICATION_TYPE_INFO,
                                                                        J2C_ATTRIBUTE_PROP_TAG, verification_tag,
                                                                        NULL);
                          g_ptr_array_add (locals, info);
                        }
                    }

                  J2cStackMapFrame *frame = g_object_new (J2C_TYPE_STACK_MAP_FRAME,
                                                          J2C_ATTRIBUTE_PROP_TAG, tag,
                                                          J2C_ATTRIBUTE_PROP_OFFSET_DELTA, offset_delta,
                                                          J2C_ATTRIBUTE_PROP_LOCALS, locals,
                                                          NULL);
                  g_ptr_array_add (entries, frame);
                }
              else if (tag == 255) /* FULL_FRAME */
                {
                  guint16 offset_delta = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (entries); goto end; }

                  guint16 number_of_locals = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (entries); goto end; }

                  GPtrArray *locals = g_ptr_array_sized_new (number_of_locals);
                  g_ptr_array_set_free_func (locals, g_object_unref);

                  for (gint i = 0; i < number_of_locals; i++)
                    {
                      guint8 verification_tag = g_data_input_stream_read_byte (in, NULL, &tmp_error);
                      if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (locals); goto end; }

                      if (verification_tag == 7 || verification_tag == 8) /* ITEM_Object || ITEM_Unititialized */
                        {
                          guint16 index_or_offset = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                          if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (locals); goto end; }

                          J2cVerificationTypeInfo *info = g_object_new (J2C_TYPE_VERIFICATION_TYPE_INFO,
                                                                        J2C_ATTRIBUTE_PROP_TAG, verification_tag,
                                                                        tag == 7 ?
                                                                          J2C_ATTRIBUTE_PROP_CPOOL_INDEX : J2C_ATTRIBUTE_PROP_OFFSET,
                                                                        index_or_offset,
                                                                        NULL);
                          g_ptr_array_add (locals, info);
                        }
                      else
                        {
                          J2cVerificationTypeInfo *info = g_object_new (J2C_TYPE_VERIFICATION_TYPE_INFO,
                                                                        J2C_ATTRIBUTE_PROP_TAG, verification_tag,
                                                                        NULL);
                          g_ptr_array_add (locals, info);
                        }
                    }

                  guint16 number_of_stack_items = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                  if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (locals); goto end; }

                  GPtrArray *stack = g_ptr_array_sized_new (number_of_stack_items);
                  g_ptr_array_set_free_func (stack, g_object_unref);

                  for (gint i = 0; i < number_of_stack_items; i++)
                    {
                      guint8 verification_tag = g_data_input_stream_read_byte (in, NULL, &tmp_error);
                      if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (stack); g_ptr_array_unref (locals); goto end; }

                      if (verification_tag == 7 || verification_tag == 8) /* ITEM_Object || ITEM_Unititialized */
                        {
                          guint16 index_or_offset = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
                          if (tmp_error) { g_ptr_array_unref (entries); g_ptr_array_unref (stack); g_ptr_array_unref (locals); goto end; }

                          J2cVerificationTypeInfo *info = g_object_new (J2C_TYPE_VERIFICATION_TYPE_INFO,
                                                                        J2C_ATTRIBUTE_PROP_TAG, verification_tag,
                                                                        tag == 7 ?
                                                                          J2C_ATTRIBUTE_PROP_CPOOL_INDEX : J2C_ATTRIBUTE_PROP_OFFSET,
                                                                        index_or_offset,
                                                                        NULL);
                          g_ptr_array_add (stack, info);
                        }
                      else
                        {
                          J2cVerificationTypeInfo *info = g_object_new (J2C_TYPE_VERIFICATION_TYPE_INFO,
                                                                        J2C_ATTRIBUTE_PROP_TAG, verification_tag,
                                                                        NULL);
                          g_ptr_array_add (stack, info);
                        }
                    }

                  J2cStackMapFrame *frame = g_object_new (J2C_TYPE_STACK_MAP_FRAME,
                                                          J2C_ATTRIBUTE_PROP_TAG, tag,
                                                          J2C_ATTRIBUTE_PROP_OFFSET_DELTA, offset_delta,
                                                          J2C_ATTRIBUTE_PROP_LOCALS, locals,
                                                          J2C_ATTRIBUTE_PROP_STACK, stack,
                                                          NULL);
                  g_ptr_array_add (entries, frame);
                }
            }
          ret = g_object_new (J2C_TYPE_ATTRIBUTE_STACK_MAP_TABLE,
                              J2C_ATTRIBUTE_PROP_ENTRIES, entries,
                              NULL);
        }
    }
end:
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
  guint8 *data_buffer = NULL;
  GInputStream *memory_input_stream = NULL;
  GDataInputStream *data_in = NULL;

  guint16 name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto error;
  name = j2c_constant_pool_get_string (cp, name_index, &tmp_error);
  if (tmp_error) goto error;

  guint32 length = g_data_input_stream_read_uint32 (in, NULL, &tmp_error);
  if (tmp_error) goto error;

  if (length > 0)
    {
      data_buffer = g_malloc (length);
      if (length != g_input_stream_read (G_INPUT_STREAM (in), data_buffer, length, NULL, &tmp_error))
        goto error;

      memory_input_stream = g_memory_input_stream_new_from_data (data_buffer, length, NULL);
      data_in = g_data_input_stream_new (memory_input_stream);
      g_data_input_stream_set_byte_order (data_in, g_data_input_stream_get_byte_order (in));
    }

  if (type == J2C_TYPE_COMPILATION_UNIT_CLASS)
    {
      ret = j2c_read_attribute_class (name, data_in, length, &tmp_error);
    }
  else if (type == J2C_TYPE_FIELD_INFO)
    {
      ret = j2c_read_attribute_field (name, data_in, length, &tmp_error);
    }
  else if (type == J2C_TYPE_METHOD_INFO)
    {
      ret = j2c_read_attribute_method (name, data_in, length, cp, &tmp_error);
    }
  else if (type == J2C_TYPE_ATTRIBUTE_CODE)
    {
      ret = j2c_read_attribute_code (name, data_in, length, &tmp_error);
    }

  if (!ret && !tmp_error)
    g_set_error (error,
                 J2C_ATTRIBUTE_ERROR,
                 J2C_UNKNOWN_ATTRIBUTE_ERROR,
                 "Unknown attribute name \'%s\'",
                 name);

error:
  if (tmp_error)
    g_propagate_error (error, tmp_error);
  if (name)
    g_free (name);
  if (data_buffer)
    g_free (data_buffer);
  if (memory_input_stream)
    g_object_unref (memory_input_stream);
  if (data_in)
    g_object_unref (data_in);

  return ret;
}
