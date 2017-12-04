#include "classfile.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <glib.h>

/* Local Error Domain and Codes */
#define CLASS_FILE_ERROR faircall_class_error_quark()

static GQuark
class_file_error_quark()
{
  return g_quark_from_static_string ("class-file-error-quark");
}

#define check_not_null(param, error, ret)      \
  if (NULL == param)                           \
    {                                          \
      g_set_error (error,                      \
		   CLASS_FILE_ERROR            \
		   INVALID_ARGUMENT,           \
		   "%s:%d %s cannot be null",  \
		   __FILE__, __LINE__, param); \
      return ret;                              \
    }

#define goto_if_null(param, error, label)      \
  if (NULL == param)                           \
    {                                          \
      g_set_error (error,                      \
		   CLASS_FILE_ERROR            \
		   INVALID_ARGUMENT,           \
		   "%s:%d %s cannot be null",  \
		   __FILE__, __LINE__, param); \
      goto label;                              \
    }



/*
 * FUNCTIONS TO READ FROM FILE
 */

static guint8
read8 (restrict FILE *const file, restrict GError **const error)
{
  check_not_null (file, error, 0);

  guint8 ret;
  if (1 != fread (&ret, sizeof (guint8), 1, file))
    {
      g_set_error (error,
		   CLASS_FILE_ERROR,
		   g_file_error_from_errno (errno),
		   "%s",
		   g_strerror (errno));
    }
  return ret;
}

static guint16
read16 (restrict FILE *const file, restrict GError **const error)
{
  check_not_null (file, error, 0);

  guint16 ret;
  if (1 != fread (&ret, sizeof (guint16), 1, file))
    {
      g_set_error (error,
		   CLASS_FILE_ERROR,
		   g_file_error_from_errno (errno),
		   "%s",
		   g_strerror (errno));
    }
  return GUINT16_FROM_BE (ret);
}

static guint32
read32 (restrict FILE *const file, restrict GError **const error)
{
  check_not_null (file, error, 0);

  guint32 ret;
  if (1 != fread (&ret, sizeof (guint32), 1, file))
    {
      g_set_error (error,
		   CLASS_FILE_ERROR,
		   g_file_error_from_errno (errno),
		   "%s",
		   g_strerror (errno));
    }
  return GUINT32_FROM_BE (ret);
}

static gboolean
utf8_info_eq (restrict Utf8Info const * const info, uint8_t *str, GError **error)
{
  if (info == NULL || str == NULL)
    {
      g_warning ("Cannot compare null parameters.");
      return FALSE;
    }

  if (info->tag != CONSTANT_Utf8)
    {
      g_warning ("This is not a CONSTANT_Utf8 structure.");
      return FALSE;
    }

  gchar *x = g_utf8_normalize (info->bytes,
			       info->length,
			       G_NORMALIZE_DEFAULT);
  if (x == NULL)
    g_set_error (error,
		 CLASS_FILE_ERROR,
		 INVALID_ARGUMENT,
		 "Cannot call utf8_info on invalid string.");

  gchar *y = g_utf8_normalize (str, -1, G_NORMALIZE_DEFAULT);
  if (y == NULL)
    g_set_error (error,
		 CLASS_FILE_ERROR,
		 INVALID_ARGUMENT,
		 "Cannot call utf8_info on invalid string.");

  gboolean ret = strncmp (x, y, info->length);
  g_free (x);
  g_free (y);

  return ret;
}

static gsize
get_verification_type_info (VerificationTypeInfo *info,
			gsize nmemb,
			FILE *file,
			GError **error)
{
  check_not_null(info, error, 0);
  check_not_null(file, error, 0);

  GError *tmp_error = NULL;
  gsize ret = 0;

  for (int i = 0; i < nmemb; i++)
    {
      guint8 tag = read8 (file, &tmp_error);
      if (tmp_error)
	goto error;

      switch (tag)
	{
	case ITEM_Top:
	case ITEM_Integer:
	case ITEM_Float:
	case ITEM_Long:
	case ITEM_Double:
	case ITEM_Null:
	case ITEM_UninitializedThis:
	  {
	    info[i].top_variable_info.tag = tag;
	    break;
	  }

	case ITEM_Object:
	  {
	    info[i].object_variable_info.tag = tag;
	    info[i].object_variable_info.cpool_index = read16 (file, &tmp_error);
	    if (tmp_error)
	      goto error;
	    break;
	  }

	case ITEM_Unititialized:
	  {
	    info[i].uninitialized_variable_info.tag = tag;
	    info[i].uninitialized_variable_info.offset = read16 (file, &tmp_error);
	    if (tmp_error)
	      goto error;
	    break;
	  }

	default:
	  goto error;
	}

      ret ++;
    }

  return ret;

error:
  if (tmp_error)
    g_propagate_error (error, tmp_error);

  return ret;
}
static void
free_attributes (void **attributes, guint16 count)
{

}

static void **
get_attributes (restrict FILE *file,
		const guint16 count,
		restrict gpointer constant_pool,
		GError **error)
{
  check_not_null (file, error, NULL);
  check_not_null (file, constant_pool, NULL);
  if (count == 0)
    return NULL;

  void **ret = calloc (count, sizeof(void *));
  if (ret == NULL)
    {
      g_set_error (error,
		   CLASS_FILE_ERROR,
		   MEMORY_ALLOC_ERROR,
		   "%s", g_strerror (errno));
      return NULL;
    }

  GError *tmp_error = NULL;

  for (int i = 0; i < count; i++)
    {
      guint16 index = read16 (file, &tmp_error);
      if (tmp_error) goto error;

      Utf8Info *info = constant_pool[index];

      /*
       * CONSTANT VALUE
       */
      if (utf8_info_eq (info, ATTRIBUTE_ConstantValue))
	{
	  ConstantValueAttribute *attr = malloc (
	      sizeof(ConstantValueAttribute));
	  if (attr == NULL)
	    {
	      g_set_error (error,
			   CLASS_FILE_ERROR,
			   MEMORY_ALLOC_ERROR,
			   "%s", g_strerror (errno));
	      goto error;
	    }
	  ret[i] = attr;

	  attr->attribute_name_index = index;
	  attr->attribute_length = read32 (file, &tmp_error);
	  if (tmp_error) goto error;
	  attr->constantvalue_index = read16 (file, &tmp_error);
	  if (tmp_error) goto error;
	}

      /*
       * CODE
       */
      else if (utf8_info_eq (info, ATTRIBUTE_Code))
	{
	  CodeAttribute *attr = malloc (sizeof(CodeAttribute));
	  if (attr == NULL)
	    {
	      g_set_error (error,
			   CLASS_FILE_ERROR,
			   MEMORY_ALLOC_ERROR,
			   "%s", g_strerror (errno));
	      goto error;
	    }
	  ret[i] = attr;

	  attr->attribute_name_index = index;
	  attr->attribute_length = read32 (file, &tmp_error);
	  if (tmp_error) goto error;

	  attr->max_stack = read16 (file, &tmp_error);
	  if (tmp_error) goto error;
	  attr->max_locals = read16 (file, &tmp_error);
	  if (tmp_error) goto error;
	  attr->code_length = read32 (file, &tmp_error);
	  if (tmp_error) goto error;

	  attr->code = calloc (attr->code_length, sizeof(uint8_t));
	  if (attr->code == NULL)
	    {
	      g_set_error (error,
			   CLASS_FILE_ERROR,
			   MEMORY_ALLOC_ERROR,
			   "%s", g_strerror (errno));
	      goto error;
	    }

	  for (int j = 0; j < attr->code_length; j++)
	    {
	      attr->code[j] = read8 (file, &tmp_error);
	      if (tmp_error) goto error;
	    }

	  attr->exception_table_length = read16 (file, &tmp_error);
	  if (tmp_error) goto error;
	  attr->exception_table = calloc (attr->exception_table_length,
					  sizeof(ExceptionTable));
	  if (attr->exception_table == NULL)
	    {
	      g_set_error (error,
			   CLASS_FILE_ERROR,
			   MEMORY_ALLOC_ERROR,
			   "%s", g_strerror (errno));
	      goto error;
	    }

	  for (int j = 0; j < attr->exception_table_length; j++)
	    {
	      attr->exception_table[j].start_pc = read16 (file, &tmp_error);
	      if (tmp_error) goto error;
	      attr->exception_table[j].end_pc = read16 (file, &tmp_error);
	      if (tmp_error) goto error;
	      attr->exception_table[j].handler_pc = read16 (file, &tmp_error);
	      if (tmp_error) goto error;
	      attr->exception_table[j].catch_type = read16 (file, &tmp_error);
	      if (tmp_error) goto error;
	    }

	  attr->attributes_count = read16 (file, &tmp_error);
	  if (tmp_error) goto error;
	  attr->attributes = get_attributes (file,
					     attr->attributes_count,
					     constant_pool,
					     &tmp_error);
	  if (tmp_error) goto error;
	}

      /*
       * STACK MAP TABLE
       */
      if (utf8_info_eq (info, ATTRIBUTE_StackMapTable))
	{
	  StackMapTableAttribute *attr = malloc (
	      sizeof(StackMapTableAttribute));
	  if (attr == NULL)
	    {
	      g_set_error (error,
			   CLASS_FILE_ERROR,
			   MEMORY_ALLOC_ERROR,
			   "%s", g_strerror (errno));
	      goto error;
	    }
	  ret[i] = attr;

	  attr->attribute_name_index = index;
	  attr->attribute_length = read32 (file, &tmp_error);
	  if (tmp_error) goto error;

	  attr->number_of_entries = read16 (file, &tmp_error);
	  if (tmp_error) goto error;
	  attr->entries = calloc (attr->number_of_entries,
				  sizeof(StackMapFrame));
	  if (attr->entries == NULL)
	    {
	      g_set_error (error,
			   CLASS_FILE_ERROR,
			   MEMORY_ALLOC_ERROR,
			   "%s", g_strerror (errno));
	      goto error;
	    }

	  for (int j = 0; j < attr->number_of_entries; j++)
	    {
	      guint8 frame_type = read8 (file, &tmp_error);
	      if (tmp_error) goto error;

	      if (frame_type <= 63)
		{
		  attr->entries[j].same_frame.frame_type = frame_type;
		}
	      else if (frame_type <= 127)
		{
		  attr->entries[j].same_locals_1_stack_item_frame.frame_type =
		      frame_type;
		  get_verification_type_info (&attr->entries[j].same_locals_1_stack_item_frame.stack,
					      1, file, &tmp_error);
		  if (tmp_error) goto error;
		}

	      else if (frame_type == 247)
		{
		  attr->entries[j].same_locals_1_stack_item_frame_extended.frame_type =
		      frame_type;
		  attr->entries[j].same_locals_1_stack_item_frame_extended.offset_delta =
		      read16 (file, &tmp_error);
		  if (tmp_error) goto error;

		  get_verification_type_info (&attr->entries[j]
							     .same_locals_1_stack_item_frame_extended
							     .stack,
		  			1, file, &tmp_error);
		  if (tmp_error) goto error;
		}

	      else if (frame_type >= 248 && frame_type <= 250)
		{
		  attr->entries[j].chop_frame.frame_type = frame_type;
		  attr->entries[j].chop_frame.offset_delta = read16 (file, &tmp_error);
		  if (tmp_error) goto error;
		}

	      else if (frame_type == 251)
		{
		  attr->entries[j].same_frame_extended.frame_type = frame_type;
		  attr->entries[j].same_frame_extended.offset_delta = read16 (
		      file, &tmp_error);
		  if (tmp_error)
		    goto error;
		}

	      else if (frame_type >= 252 && frame_type <= 254)
		{
		  attr->entries[j].append_frame.frame_type = frame_type;
		  attr->entries[j].append_frame.offset_delta = read16 (
		      file, &tmp_error);
		  if (tmp_error)
		    goto error;

		  gsize size = frame_type - 251;
		  attr->entries[j].append_frame.locals = calloc (size, sizeof (VerificationTypeInfo));
		  if (attr->entries[j].append_frame.locals == NULL)
		    {
		      g_set_error (error,
				   CLASS_FILE_ERROR,
				   MEMORY_ALLOC_ERROR,
				   "%s", g_strerror (errno));
		      goto error;
		    }
		  get_verification_type_info (attr->entries[j].append_frame.locals,
					      size, file, &tmp_error);
		  if (tmp_error) goto error;
		}

	      else if (frame_type == 255)
		{
		  attr->entries[j].full_frame.frame_type = frame_type;
		  attr->entries[j].full_frame.offset_delta = read16 (file, &tmp_error);
		  if (tmp_error) goto error;

		  attr->entries[j].full_frame.number_of_locals = read16 (file, &tmp_error);
		  if (tmp_error) goto error;
		  attr->entries[j].full_frame.locals = calloc (attr->entries[j].full_frame.number_of_locals,
							       sizeof (VerificationTypeInfo));
		  if (attr->entries[j].full_frame.locals == NULL)
		    {
		      g_set_error (error,
				   CLASS_FILE_ERROR,
				   MEMORY_ALLOC_ERROR,
				   "%s", g_strerror (errno));
		      goto error;
		    }
		  get_verification_type_info (attr->entries[j].full_frame.locals,
					      attr->entries[j].full_frame.number_of_locals,
					      file, &tmp_error);
		  if (tmp_error)
		    goto error;

		  attr->entries[j].full_frame.number_of_stack_items = read16 (
		      file, &tmp_error);
		  if (tmp_error)
		    goto error;
		  attr->entries[j].full_frame.stack = calloc (
		      attr->entries[j].full_frame.number_of_stack_items,
		      sizeof(VerificationTypeInfo));
		  if (attr->entries[j].full_frame.stack == NULL)
		    {
		      g_set_error (error,
		      CLASS_FILE_ERROR,
				   MEMORY_ALLOC_ERROR, "%s",
				   g_strerror (errno));
		      goto error;
		    }
		  get_verification_type_info (
		      attr->entries[j].full_frame.stack,
		      attr->entries[j].full_frame.number_of_stack_items, file,
		      &tmp_error);
		  if (tmp_error)
		    goto error;
		}
	    }
	}

      else if (utf8_info_eq (info, ATTRIBUTE_Exceptions))
	{
	  ExceptionsAttribute *attr = malloc (
	      sizeof(ExceptionsAttribute));
	  if (attr == NULL)
	    {
	      g_set_error (error,
			   CLASS_FILE_ERROR,
			   MEMORY_ALLOC_ERROR,
			   "%s", g_strerror (errno));
	      goto error;
	    }
	  ret[i] = attr;

	  attr->attribute_name_index = index;
	  attr->attribute_length = read32 (file, &tmp_error);
	  if (tmp_error)
	    goto error;

	  attr->number_of_exceptions = read16 (file, &tmp_error);
	  if (tmp_error) goto error;
	  attr->exception_index_table = calloc (attr->number_of_exceptions, sizeof (guint16));
	  if (attr->exception_index_table == NULL)
	    {
	      g_set_error (error,
			   CLASS_FILE_ERROR,
			   MEMORY_ALLOC_ERROR,
			   "%s", g_strerror (errno));
	      goto error;
	    }

	  for (int j = 0; j < attr->number_of_exceptions; j++)
	    {
	      attr->exception_index_table[j] = read16 (file, &tmp_error);
	      if (tmp_error) goto error;
	    }
	}
    }

  return ret;

error:
  free_attributes (ret, count);

  if (tmp_error)
    g_propagate_error (error, tmp_error);

  return NULL;
}

void
free_class_file (ClassFile *obj)
{
  if (obj != NULL)
    {
      if (obj->constant_pool != NULL)
	{
	  for (int i = 0; i < obj->constant_pool_count; i++)
	    {
	      if (obj->constant_pool[i] != NULL)
		{
		  if (((guint8) obj->constant_pool[i]) == CONSTANT_Utf8)
		    {
		      Utf8Info *info = obj->constant_pool[i];
		      if (info->bytes != NULL)
			free (info->bytes);
		    }
		  free (obj->constant_pool[i]);
		}
	    }
	  free (obj->constant_pool);
	}

      if (obj->interfaces != NULL)
	free (obj->interfaces);

      if (obj->fields != NULL)
	{
	  for (int i = 0; i < obj->fields_count; i++)
	    if (obj->fields[i] != NULL)
	      free_attributes (obj->fields[i]->attributes);
	  free (obj->fields);
	}

      if (obj->methods != NULL)
	{
	  for (int i = 0; i < obj->methods_count; i++)
	    if (obj->methods[i] != NULL)
	      free_attributes (obj->methods[i]->attributes);
	  free (obj->methods);
	}

      free_attributes (obj->attributes);
      free (obj);
    }
}

ClassFile *
parse_class_file (restrict FILE *file, GError **error)
{
  /* Check file */
  check_not_null (file, error, NULL);

  ClassFile *ret = NULL;
  GError *tmp_error = NULL;

  guint32 magic = read32 (file, &tmp_error);
  if (tmp_error) goto error;

  if (magic != CLASS_FILE_MAGIC_NUMBER)
    {
      g_set_error (error,
		   CLASS_FILE_ERROR,
		   INVALID_ARGUMENT,
		   "This file is not a class file.");
      return NULL;
    }

  /* Build ClassFile object */
  ret = calloc (1, sizeof(ClassFile));
  if (ret == NULL)
    {
      g_set_error (CLASS_FILE_ERROR,
		   MEMORY_ALLOC_ERROR,
		   "%s", g_strerror (errno));
      return NULL;
    }

  ret->magic = magic;

  /* Get version information */
  ret->minor_version = read16(file, &tmp_error);
  if (tmp_error) goto error;
  ret->major_version = read16(file, &tmp_error);
  if (tmp_error) goto error;

  /* Build constant pool */
  ret->constant_pool_count = read16(file, &tmp_error);
  if (tmp_error) goto error;

  ret->constant_pool = calloc (ret->constant_pool_count, sizeof(gpointer));
  if (ret->constant_pool == NULL)
    {
      g_set_error (CLASS_FILE_ERROR,
		   MEMORY_ALLOC_ERROR,
		   "%s", g_strerror (errno));
      goto error;
    }

  for (int i = 0; i < ret->constant_pool_count; i++)
    {
      gpointer item = NULL;
      guint8 tag = read8(file, &tmp_error);
      if (tmp_error) goto error;

      switch (tag)
	{
	case CONSTANT_Class:
	  {
	    ClassInfo *info = malloc (sizeof(ClassInfo));
	    if (info == NULL)
	      {
		g_set_error (CLASS_FILE_ERROR, MEMORY_ALLOC_ERROR, "%s",
			     g_strerror (errno));
		goto error;
	      }

	    info->tag = CONSTANT_Class;
	    info->name_index = read16(file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }

	    item = info;
	    break;
	  }

	case CONSTANT_Fieldref:
	case CONSTANT_Methodref:
	case CONSTANT_InterfaceMethodref:
	  {
	    FieldrefInfo *info = malloc (sizeof(FieldrefInfo));
	    if (info == NULL)
	      {
		g_set_error (CLASS_FILE_ERROR, MEMORY_ALLOC_ERROR, "%s",
			     g_strerror (errno));
		goto error;
	      }

	    info->tag = tag;
	    info->class_index = read16(file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }
	    info->name_and_type_index = read16 (file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }

	    item = info;
	    break;
	  }

	case CONSTANT_String:
	  {
	    StringInfo *info = malloc (sizeof(StringInfo));
	    if (info == NULL)
	      {
		g_set_error (CLASS_FILE_ERROR, MEMORY_ALLOC_ERROR, "%s",
			     g_strerror (errno));
		goto error;
	      }

	    info->tag = CONSTANT_String;
	    info->string_index = read16 (file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }

	    item = info;
	    break;
	  }

	case CONSTANT_Integer:
	case CONSTANT_Float:
	  {
	    IntegerInfo *info = malloc (sizeof(IntegerInfo));
	    if (info == NULL)
	      {
		g_set_error (CLASS_FILE_ERROR, MEMORY_ALLOC_ERROR, "%s",
			     g_strerror (errno));
		goto error;
	      }

	    info->tag = tag;
	    info->bytes = read32(file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }

	    item = info;
	    break;
	  }

	case CONSTANT_Long:
	case CONSTANT_Double:
	  {
	    LongInfo *info = malloc (sizeof(IntegerInfo));
	    if (info == NULL)
	      {
		g_set_error (CLASS_FILE_ERROR, MEMORY_ALLOC_ERROR, "%s",
			     g_strerror (errno));
		goto error;
	      }

	    info->tag = tag;
	    info->high_bytes = read32(file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }
	    info->low_bytes = read32(file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }

	    item = info;
	    break;
	  }

	case CONSTANT_NameAndType:
	  {
	    NameAndTypeInfo *info = malloc (sizeof(NameAndTypeInfo));
	    if (info == NULL)
	      {
		g_set_error (CLASS_FILE_ERROR, MEMORY_ALLOC_ERROR, "%s",
			     g_strerror (errno));
		goto error;
	      }

	    info->tag = CONSTANT_NameAndType;
	    info->name_index = read16 (file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }
	    info->descriptor_index = read16 (file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }

	    item = info;
	    break;
	  }

	case CONSTANT_Utf8:
	  {
	    Utf8Info *info = malloc (sizeof(Utf8Info));
	    if (info == NULL)
	      {
		g_set_error (CLASS_FILE_ERROR, MEMORY_ALLOC_ERROR, "%s",
			     g_strerror (errno));
		goto error;
	      }

	    info->tag = CONSTANT_Utf8;
	    info->length = read16 (file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }

	    info->bytes = calloc (info->length, sizeof(uint8_t));
	    if (info->bytes == NULL)
	      {
		free (info);
		break;
	      }

	    for (int j = 0; j < info->length; j++)
	      {
		info->bytes[j] = read8 (file, &tmp_error);
		if (tmp_error)
		  {
		    free (info->bytes);
		    free (info);
		    goto error;
		  }
	      }

	    item = info;
	    break;
	  }

	case CONSTANT_MethodHandle:
	  {
	    MethodHandleInfo *info = malloc (sizeof(MethodHandleInfo));
	    if (info == NULL)
	      {
		g_set_error (CLASS_FILE_ERROR, MEMORY_ALLOC_ERROR, "%s",
			     g_strerror (errno));
		goto error;
	      }

	    info->tag = CONSTANT_MethodHandle;
	    info->reference_kind = read8 (file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }
	    info->reference_index = read16 (file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }

	    item = info;
	    break;
	  }

	case CONSTANT_MethodType:
	  {
	    MethodTypeInfo *info = malloc (sizeof(MethodTypeInfo));
	    if (info == NULL)
	      {
		g_set_error (CLASS_FILE_ERROR, MEMORY_ALLOC_ERROR, "%s",
			     g_strerror (errno));
		goto error;
	      }

	    info->tag = CONSTANT_MethodType;
	    info->descriptor_index = read16 (file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }

	    item = info;
	    break;
	  }

	case CONSTANT_InvokeDynamic:
	  {
	    InvokeDynamicInfo *info = malloc (sizeof(InvokeDynamicInfo));
	    if (info == NULL)
	      {
		g_set_error (CLASS_FILE_ERROR, MEMORY_ALLOC_ERROR, "%s",
			     g_strerror (errno));
		goto error;
	      }

	    info->tag = CONSTANT_InvokeDynamic;
	    info->bootstrap_method_attr_index = read16 (file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }
	    info->name_and_type_index = read16 (file, &tmp_error);
	    if (tmp_error)
	      {
		free (info);
		goto error;
	      }

	    item = info;
	    break;
	  }

	default:
	  {
	    g_set_error (error,
			 CLASS_FILE_ERROR,
			 FILE_PARSE_ERROR,
			 "Found invalid constant tag %d",
			 tag);
	    goto error;
	  }
	}

      if (item == NULL)
	{
	  g_set_error (error,
		       CLASS_FILE_ERROR,
		       UNKNOWN_ERROR,
		       "Found invalid constant tag %d",
		       tag);
	  goto error;
	}
      else
	{
	  ret->constant_pool[i] = item;
	}
    }

  /* Other class data */
  ret->access_flags = read16 (file, &tmp_error);
  if (tmp_error) goto error;
  ret->this_class = read16 (file, &tmp_error);
  if (tmp_error) goto error;
  ret->super_class = read16 (file, &tmp_error);
  if (tmp_error) goto error;

  /* Interfaces */
  ret->interfaces_count = read16 (file, &tmp_error);
  if (tmp_error) goto error;
  ret->interfaces = calloc (ret->interfaces_count, sizeof(guint16));
  if (ret->interfaces == NULL)
    {
      g_set_error (error,
		   CLASS_FILE_ERROR,
		   MEMORY_ALLOC_ERROR,
		   "%s", g_strerror (errno));
      goto error;
    }

  for (int i = 0; i < ret->interfaces_count; i++)
    {
      ret->interfaces[i] = read16 (file, &tmp_error);
      if (tmp_error) goto error;
    }

  /* Fields */
  ret->fields_count = read16 (file, &tmp_error);
  if (tmp_error) goto error;

  ret->fields = calloc (ret->fields_count, sizeof(FieldInfo));
  if (ret->fields == NULL)
    {
      g_set_error (error,
		   CLASS_FILE_ERROR,
		   MEMORY_ALLOC_ERROR,
		   "%s", g_strerror (errno));
      goto error;
    }

  for (int i = 0; i < ret->fields_count; i++)
    {
      FieldInfo *info = malloc (sizeof(FieldInfo));
      if (info == NULL)
	{
	  g_set_error (error,
		       CLASS_FILE_ERROR,
		       MEMORY_ALLOC_ERROR,
		       "%s", g_strerror (errno));
	  goto error;
	}

      info->access_flags = read16 (file, &tmp_error);
      if (tmp_error)
	{
	  free (info);
	  goto error;
	}
      info->name_index = read16 (file, &tmp_error);
      if (tmp_error)
	{
	  free (info);
	  goto error;
	}
      info->descriptor_index = read16 (file, &tmp_error);
      if (tmp_error)
	{
	  free (info);
	  goto error;
	};
      info->attributes_count = read16 (file, &tmp_error);
      if (tmp_error)
	{
	  free (info);
	  goto error;
	}

      info->attributes = get_attributes (file,
					 info->attributes_count,
					 ret->constant_pool,
					 &tmp_error);
      if (tmp_error)
      	{
      	  free (info);
      	  goto error;
      	}
      ret->fields[i] = info;
    }

  /* Methods */
  ret->methods_count = read16 (file, &tmp_error);
  if (tmp_error) goto error;

  ret->methods = calloc (ret->methods_count, sizeof(MethodInfo));
  if (ret->methods == NULL)
    {
      g_set_error (error,
		   CLASS_FILE_ERROR,
		   MEMORY_ALLOC_ERROR,
		   "%s", g_strerror (errno));
      goto error;
    }

  for (int i = 0; i < ret->methods_count; i++)
    {
      MethodInfo *info = malloc (sizeof(MethodInfo));
      if (info == NULL)
	{
	  g_set_error (error,
		       CLASS_FILE_ERROR,
		       MEMORY_ALLOC_ERROR,
		       "%s", g_strerror (errno));
	  goto error;
	}

      info->access_flags = read16 (file, &tmp_error);
      if (tmp_error)
	{
	  free (info);
	  goto error;
	}
      info->name_index = read16 (file, &tmp_error);
      if (tmp_error)
	{
	  free (info);
	  goto error;
	}
      info->descriptor_index = read16 (file, &tmp_error);
      if (tmp_error)
	{
	  free (info);
	  goto error;
	};
      info->attributes_count = read16 (file, &tmp_error);
      if (tmp_error)
	{
	  free (info);
	  goto error;
	}

      info->attributes = get_attributes (file,
					 info->attributes_count,
					 ret->constant_pool,
					 &tmp_error);
      if (tmp_error)
	{
	  free (info);
	  goto error;
	}
      ret->methods[i] = info;
    }

  /* Class attributes */
  ret->attributes_count = read16 (file, &tmp_error);
  if (tmp_error) goto error;
  ret->attributes = get_attributes (file,
				    ret->attributes_count,
				    ret->constant_pool,
				    &tmp_error);
  if (tmp_error) goto error;

  return ret;

error:
  free_class_file (ret);

  if (tmp_error)
    g_propagate_error (error, tmp_error);

  return NULL;
}
