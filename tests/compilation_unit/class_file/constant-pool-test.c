#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <math.h>

#include <j2c/constant-pool.h>
#include <j2c/constant-pool-item.h>

typedef struct
{

  gchar *type;

  guint16 name_index;
  guint16 class_index;
  guint16 name_and_type_index;
  guint16 string_index;
  gint32 int_value;
  gfloat float_value;
  gint64 long_value;
  gdouble double_value;
  guint16 descriptor_index;
  gchar *utf8_value;
  guint8 reference_kind;
  guint16 reference_index;
  guint16 bootstrap_method_attr_index;

} ConstantPoolItem;

typedef struct
{
  gchar *class_file_path;
  ConstantPoolItem **arr;
  gsize len;
} ConstantPool;

static ConstantPoolItem *
constant_pool_item_alloc (void)
{
  return g_malloc0 (sizeof (ConstantPoolItem));
}

static void
constant_pool_item_free (ConstantPoolItem *item)
{
  if (item)
    {
      if (item->type)
	g_free (item->type);
      if (item->utf8_value)
	g_free (item->utf8_value);
      g_free (item);
    }
}

static void
constant_pool_free (gpointer data)
{
  ConstantPool *pool = data;
  if (pool)
    {
      if (pool->class_file_path)
	g_free (pool->class_file_path);
      if (pool->arr)
	{
	  for (gint i = 0; i < pool->len; i++)
	    if (pool->arr[i])
	      constant_pool_item_free (pool->arr[i]);
	  g_free (pool->arr);
	}
      g_free (pool);
    }
}

static ConstantPool *
constant_pool_alloc (void)
{
  ConstantPool *pool = g_malloc0 (sizeof (ConstantPool));
  return pool;
}

static long
str_as_long (const gchar *text, GError **error)
{
  long ret = strtol (text, NULL, 10);

  if (ret == LONG_MIN || ret == LONG_MAX)
    {
      g_set_error (error, 1, 1, "Cannot parse \'%s\' as a number", text);
      return 0;
    }

  return ret;
}

static double
str_as_double (const gchar *text, GError **error)
{
  if (!g_strcmp0 (text, "Infinity"))
    return INFINITY;
  else if (!g_strcmp0 (text, "-Infinity"))
    return -INFINITY;
  else if (!g_strcmp0 (text, "NaN"))
    return NAN;

  double ret = strtod (text, NULL);

  return ret;
}

static void
parse_start_element (GMarkupParseContext *context,
		     const gchar *element_name,
		     const gchar **attribute_names,
		     const gchar **attribute_values,
		     gpointer user_data,
		     GError **error)
{
  ConstantPool *pool = user_data;

  /* parse <constant_pool> */
  if (g_strcmp0 (element_name, "constant_pool") == 0)
    {
      for (int i = 0; attribute_names[i]; i++)
	{
	  if (!g_strcmp0 (attribute_names[i], "file"))
	    {
	      if (pool->class_file_path)
		g_free (pool->class_file_path);
	      pool->class_file_path = g_strdup (attribute_values[i]);
	      break;
	    }
	}
      return;
    }

  /* parse a child of <constant_pool> */
  const GSList *stack = g_markup_parse_context_get_element_stack (context);
  if (stack && stack->next && !g_strcmp0 (stack->next->data, "constant_pool"))
    {
      gchar **attrs = g_strsplit (element_name, "_", -1);
      if (g_strv_length (attrs) != 2)
	{
	  g_strfreev (attrs);
	  g_set_error (error, 1, 1, "Cannot parse item tag name \'%s\'", element_name);
	  return;
	}

      GError *tmp_error = NULL;
      long index = str_as_long (attrs[1], &tmp_error);
      if (tmp_error)
	{
	  g_strfreev (attrs);
	  g_propagate_error (error, tmp_error);
	  return;	
	}

      ConstantPoolItem *item = NULL;
      if (index < pool->len)
	item = pool->arr[index];
      if (!item)
	{
	  item = constant_pool_item_alloc ();
	  if (index >= pool->len)
	    {
	      gsize len = index + 1;
	      ConstantPoolItem **arr = g_malloc0 (len * sizeof (ConstantPoolItem *));
	      for (gint i = 0; i < pool->len; i++)
		if (pool->arr[i])
		  arr[i] = pool->arr[i];

	      g_free (pool->arr);
	      pool->arr = arr;
	      pool->len = len;
	    }
	  pool->arr[index] = item;
	}

      if (item->type)
	g_free (item->type);

      item->type = g_strdup (attrs[0]);
      g_strfreev (attrs);

      return;
    }
}

static void
parse_text (GMarkupParseContext *context,
	    const gchar *text,
	    gsize text_len,
	    gpointer user_data,
	    GError **error)
{
  ConstantPool *pool = user_data;
  const GSList *stack = g_markup_parse_context_get_element_stack (context);
  if (!g_strcmp0 (stack->data, "constant_pool"))
    /* Ignore <constant_pool> nodes */
    return;
  if (stack->next && !g_strcmp0 (stack->next->data, "constant_pool"))
    /* Ignore children of <constant_pool> nodes */
    return;
  if (stack->next && stack->next->next && !g_strcmp0 (stack->next->next->data, "constant_pool"))
    {
      /* Handle grandchildren of <constant_pool> nodes */
      const gchar *parent_name = stack->next->data;
      const gchar *element_name = stack->data;

      gchar **attrs = g_strsplit (parent_name, "_", -1);
      if (g_strv_length (attrs) != 2)
	{
	  g_strfreev (attrs);
	  g_set_error (error, 1, 1, "Cannot parse item tag name \'%s\'", parent_name);
	  return;
	}

      GError *tmp_error = NULL;
      long index = str_as_long (attrs[1], &tmp_error);
      if (index == LONG_MIN || index == LONG_MAX)
	{
	  g_strfreev (attrs);
	  g_propagate_error (error, tmp_error);
	  return;	
	}
      g_strfreev (attrs);

      if (index >= pool->len)
	{
	  g_set_error (error, 1, 1, "%ld is not a valid index in arr len %ld", index, pool->len);
	  return;
	}

      ConstantPoolItem *item = pool->arr[index];
      if (!item)
	{
	  g_set_error (error, 1, 1, "No item has been  placed at index %ld", index);
	  return;
	}

      gchar *str = g_malloc0 (text_len);
      snprintf (str, text_len, "%s", text);

      if (!g_strcmp0 (element_name, "class_index"))
	item->class_index = str_as_long (str, error);
      else if (!g_strcmp0 (element_name, "name_and_type_index"))
	item->name_and_type_index = str_as_long (str, error);
      else if (!g_strcmp0 (element_name, "string_index"))
	item->string_index = str_as_long (str,error);
      else if (!g_strcmp0 (element_name, "value") && !g_strcmp0 (item->type, "Integer"))
	item->int_value = str_as_long (str, error);
      else if (!g_strcmp0 (element_name, "value") && !g_strcmp0 (item->type, "Long"))
	item->long_value = str_as_long (str, error);
      else if (!g_strcmp0 (element_name, "value") && !g_strcmp0 (item->type, "Float"))
	item->float_value = str_as_double (str, error);
      else if (!g_strcmp0 (element_name, "value") && !g_strcmp0 (item->type, "Double"))
	item->double_value = str_as_double (str, error);
      else if (!g_strcmp0 (element_name, "value") && !g_strcmp0 (item->type, "Utf8"))
	item->utf8_value = g_strdup (str);
      else if (!g_strcmp0 (element_name, "name_index"))
	item->name_index = str_as_long (str, error);
      else if (!g_strcmp0 (element_name, "descriptor_index"))
	item->descriptor_index = str_as_long (str, error);
      else if (!g_strcmp0 (element_name, "reference_kind"))
	item->reference_kind = str_as_long (str, error);
      else if (!g_strcmp0 (element_name, "reference_index"))
	item->reference_index = str_as_long (str, error);
      else if (!g_strcmp0 (element_name, "bootstrap_method_attr_index"))
	item->bootstrap_method_attr_index = str_as_long (str, error);

      g_free (str);
   }
}

static void
collect_children (GPtrArray *array, GFile *file)
{
  g_return_if_fail (array != NULL);
  g_return_if_fail (file != NULL);
  g_return_if_fail (g_file_query_exists (file, NULL));
  g_return_if_fail (g_file_query_file_type (file, G_FILE_QUERY_INFO_NONE, NULL) == G_FILE_TYPE_DIRECTORY);

  GError *error = NULL;
  GFileEnumerator *direnum = g_file_enumerate_children (file,
							G_FILE_ATTRIBUTE_STANDARD_TYPE,
							G_FILE_QUERY_INFO_NONE,
							NULL,
							&error);
  if (error)
    goto error;

  while (TRUE)
    {
      GFile *child = NULL;
      GFileInfo *info = NULL;

      if (!g_file_enumerator_iterate (direnum, &info, &child, NULL, &error))
	goto error;
      if (!info)
	break;

      if (g_file_info_get_attribute_uint32 (info, G_FILE_ATTRIBUTE_STANDARD_TYPE)
	    == G_FILE_TYPE_DIRECTORY)
	{
	  collect_children (array, child);
	}
      else
	{
	  g_ptr_array_add (array, g_object_ref (child));
	}

    }

  g_object_unref (direnum);

error:
  if (error)
    {
      fprintf (stderr, "%s", error->message);
      exit (EXIT_FAILURE);
    }

}

static void
test_constant_pool (gconstpointer user_data)
{
  GError *error = NULL;

  GFileInputStream *in = NULL;
  GOutputStream *out = NULL;
  GBytes *bytes = NULL;

  GMarkupParser *parser = NULL;
  GMarkupParseContext *context = NULL;
  gchar *text = NULL;
  ConstantPool *pool = NULL;

  GFile *file = NULL;
  GDataInputStream *din = NULL;
  J2cConstantPool *test = NULL;

  /*
   * GET FILE AS BYTES
   */
  in = g_file_read ((GFile *)user_data, NULL, &error);
  out = g_memory_output_stream_new (NULL, 0, g_realloc, g_free);

  gchar buffer[100];
  gsize read = g_input_stream_read (G_INPUT_STREAM (in), buffer, 100, NULL, &error);
  if (error) goto error;
  while (read)
    {
      g_output_stream_write (out, buffer, read, NULL, &error);
      if (error) goto error;
      read = g_input_stream_read (G_INPUT_STREAM (in), buffer, 100, NULL, &error);
    }

  g_clear_object (&in);
  g_output_stream_close (out, NULL, &error);
  bytes = g_memory_output_stream_steal_as_bytes (G_MEMORY_OUTPUT_STREAM(out));

  g_clear_object (&out);

  /*
   *  PARSE XML DATA
   */

  parser = g_malloc0 (sizeof (GMarkupParser));
  parser->start_element = parse_start_element;
  parser->text = parse_text;
  pool = constant_pool_alloc ();
  context = g_markup_parse_context_new (parser, 0, pool, constant_pool_free);

  gsize text_len = 0;
  text = g_bytes_unref_to_data (bytes, &text_len);
  bytes = NULL;
  g_markup_parse_context_parse (context, text, text_len, &error);
  g_free (text);
  text = NULL;
  if (error) goto error;

  if (pool->class_file_path)
    {
      file = g_file_new_for_path (pool->class_file_path);
      in = g_file_read (file, NULL, &error);
      if (error) goto error;

      din = g_data_input_stream_new (G_INPUT_STREAM (in));
      g_data_input_stream_set_byte_order (din, G_DATA_STREAM_BYTE_ORDER_BIG_ENDIAN);

      g_input_stream_skip (G_INPUT_STREAM (din), 8, NULL, &error);
      if (error) goto error;
      test = j2c_constant_pool_new (din, &error);
      if (error) goto error;

      for (gint i = 0; i < pool->len; i++)
	{
	  ConstantPoolItem *expected = pool->arr[i];
	  if (!expected)
	    continue;

	  J2cConstantPoolItem *result = j2c_constant_pool_get (test, i, &error);
	  if (error)
	    {
	      g_object_unref (result);
	      goto error;
	    }
	  if (!g_strcmp0 (expected->type, "Class"))
	    {
	      /* name_index */
	      g_assert_true (J2C_IS_CLASS_INFO (result));
	      guint name_index = j2c_class_info_name_index (J2C_CLASS_INFO (result));
	      g_assert_cmpuint (name_index, ==, expected->name_index);
	    }
	  else if (!g_strcmp0 (expected->type, "Fieldref"))
	    {
	      /* class_index, name_and_type_index */
	      g_assert_true (J2C_IS_FIELDREF_INFO (result));
	      guint class_index = j2c_fieldref_info_class_index (J2C_FIELDREF_INFO (result));
	      guint name_and_type_index = j2c_fieldref_info_name_and_type_index (J2C_FIELDREF_INFO (result));
	      g_assert_cmpuint (class_index, ==, expected->class_index);
	      g_assert_cmpuint (name_and_type_index, ==, expected->name_and_type_index);
	    }
	  else if (!g_strcmp0 (expected->type, "Methodref"))
	    {
	      /* class_index, name_and_type_index */
	      g_assert_true (J2C_IS_METHODREF_INFO (result));
	      guint class_index = j2c_methodref_info_class_index (J2C_METHODREF_INFO (result));
	      guint name_and_type_index = j2c_methodref_info_name_and_type_index (J2C_METHODREF_INFO (result));
	      g_assert_cmpuint (class_index, ==, expected->class_index);
	      g_assert_cmpuint (name_and_type_index, ==, expected->name_and_type_index);
	    }
	  else if (!g_strcmp0 (expected->type, "InterfaceMethodref"))
	    {
	      /* class_index, name_and_type_index */
	      g_assert_true (J2C_IS_INTERFACE_METHODREF_INFO (result));
	      guint class_index = j2c_interface_methodref_info_class_index (J2C_INTERFACE_METHODREF_INFO (result));
	      guint name_and_type_index = j2c_interface_methodref_info_name_and_type_index (J2C_INTERFACE_METHODREF_INFO (result));
	      g_assert_cmpuint (class_index, ==, expected->class_index);
	      g_assert_cmpuint (name_and_type_index, ==, expected->name_and_type_index);
	    }
	  else if (!g_strcmp0 (expected->type, "String"))
	    {
	      /* string_index */
	      g_assert_true (J2C_IS_STRING_INFO (result));
	      guint string_index = j2c_string_info_string_index (J2C_STRING_INFO (result));
	      g_assert_cmpuint (string_index, ==, expected->string_index);
	    }
	  else if (!g_strcmp0 (expected->type, "Integer"))
	    {
	      /* int_value */
	      g_assert_true (J2C_IS_INTEGER_INFO (result));
	      gint32 value = j2c_integer_info_value (J2C_INTEGER_INFO (result));
	      g_assert_cmpint (value, ==, expected->int_value);
	    }
	  else if (!g_strcmp0 (expected->type, "Float"))
	    {
	      /* float_value */
	      g_assert_true (J2C_IS_FLOAT_INFO (result));
	      gfloat value = j2c_float_info_value (J2C_FLOAT_INFO (result));
	      g_assert_cmpfloat (value, ==, expected->float_value);
	    }
	  else if (!g_strcmp0 (expected->type, "Long"))
	    {
	      /* long_value */
	      g_assert_true (J2C_IS_LONG_INFO (result));
	      gint64 value = j2c_long_info_value (J2C_LONG_INFO (result));
	      g_assert_cmpint (value, ==, expected->long_value);
	    }
	  else if (!g_strcmp0 (expected->type, "Double"))
	    {
	      /* double_value */
	      g_assert_true (J2C_IS_DOUBLE_INFO (result));
	      gdouble value = j2c_double_info_value (J2C_DOUBLE_INFO (result));
	      g_assert_cmpfloat (value, ==, expected->double_value);
	    }
	  else if (!g_strcmp0 (expected->type, "NameAndType"))
	    {
	      /* name_index, descriptor_index */
	      g_assert_true (J2C_IS_NAME_AND_TYPE_INFO (result));
	      guint name_index = j2c_name_and_type_info_name_index (J2C_NAME_AND_TYPE_INFO (result));
	      guint descriptor_index = j2c_name_and_type_info_descriptor_index (J2C_NAME_AND_TYPE_INFO (result));
	      g_assert_cmpuint (name_index, ==, expected->name_index);
	      g_assert_cmpuint (descriptor_index, ==, expected->descriptor_index);
	    }
	  else if (!g_strcmp0 (expected->type, "Utf8"))
	    {
	      /* utf8_value */
	      g_assert_true (J2C_IS_UTF8_INFO (result));
	      const gchar *value = j2c_utf8_info_string (J2C_UTF8_INFO (result));
	      g_assert_cmpstr (value, ==, expected->utf8_value);
	    }
	  else if (!g_strcmp0 (expected->type, "MethodHandle"))
	    {
	      /* reference_kind, reference_index */
	      g_assert_true (J2C_IS_METHOD_HANDLE_INFO (result));
	      guint reference_kind = j2c_method_handle_info_reference_kind (J2C_METHOD_HANDLE_INFO (result));
	      guint reference_index = j2c_method_handle_info_reference_index (J2C_METHOD_HANDLE_INFO (result));
	      g_assert_cmpuint (reference_kind, ==, expected->reference_kind);
	      g_assert_cmpuint (reference_index, ==, expected->reference_index);
	    }
	  else if (!g_strcmp0 (expected->type, "MethodType"))
	    {
	      /* descriptor_index */
	      g_assert_true (J2C_IS_METHOD_TYPE_INFO (result));
	      guint descriptor_index = j2c_method_type_info_descriptor_index (J2C_METHOD_TYPE_INFO (result));
	      g_assert_cmpuint (descriptor_index, ==, expected->descriptor_index);
	    }
	  else if (!g_strcmp0 (expected->type, "InvokeDynamic"))
	    {
	      /* boostrap_method_attr_index, name_and_type_index */
	      g_assert_true (J2C_IS_INVOKE_DYNAMIC_INFO (result));
	      guint bootstrap_method_attr_index = j2c_invoke_dynamic_info_bootstrap_method_attr_index (J2C_INVOKE_DYNAMIC_INFO (result));
	      guint name_and_type_index = j2c_invoke_dynamic_info_name_and_type_index (J2C_INVOKE_DYNAMIC_INFO (result));
	      g_assert_cmpuint (bootstrap_method_attr_index, ==, expected->bootstrap_method_attr_index);
	      g_assert_cmpuint (name_and_type_index, ==, expected->name_and_type_index);
	    }
	  else
	    {
	      g_set_error (&error, 1, 1, "Bad type in xml data: %s", expected->type);
	      g_object_unref (result);
	      goto error;
	    }
	  g_clear_object (&result);
	}

      g_clear_object (&test);
    }
  g_markup_parse_context_free (context);
  return;

error:
  if (in)
    g_object_unref (in);
  if (out)
    g_object_unref (out);
  if (bytes)
    g_bytes_unref (bytes);
  if (parser)
    g_free (parser);
  if (context)
    g_markup_parse_context_free (context);
  if (text)
    g_free (text);
  if (file)
    g_object_unref (file);
  if (din)
    g_object_unref (din);
  if (test)
    g_object_unref (test);
  if (error)
    {
      fprintf (stderr, "%s\n", error->message);
      exit (EXIT_FAILURE);
    }
}

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  GPtrArray *files = g_ptr_array_new_with_free_func (g_object_unref);
  GFile *root_dir = g_file_new_for_path ("./constant_pool_data");
  collect_children (files, root_dir);
  g_object_unref (root_dir);

  g_test_init (&argc, &argv, NULL);

  for (gint i = 0; i < files->len; i ++)
    {
      GFile *file = g_ptr_array_index (files, i);

      GFileInfo *info = g_file_query_info (file, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME, G_FILE_QUERY_INFO_NONE, NULL, NULL);
      if (!info)
	exit (EXIT_FAILURE);

      const gchar *path = g_file_info_get_display_name (info);

      gchar *test = g_strdup_printf ("/compilation_unit/class_file/constant_pool/%s",
				     path);

      g_object_unref (info);
      g_test_add_data_func_full (test,
				 g_object_ref (file),
				 test_constant_pool,
				 g_object_unref);

      g_free (test);
    }

  g_ptr_array_free (files, TRUE);

  return g_test_run();
}
