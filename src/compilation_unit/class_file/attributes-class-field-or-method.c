#include <j2c/attributes.h>
#include <j2c/object-array.h>

struct _J2cAttributeSynthetic
{
  GObject parent;
};

G_DEFINE_TYPE (J2cAttributeSynthetic, j2c_attribute_synthetic, G_TYPE_OBJECT)

struct _J2cAttributeDeprecated
{
  GObject parent;
};

G_DEFINE_TYPE(J2cAttributeDeprecated, j2c_attribute_deprecated, G_TYPE_OBJECT)

struct _J2cAttributeSignature
{
  GObject parent;
  guint16 signature_index;
};

G_DEFINE_TYPE(J2cAttributeSignature, j2c_attribute_signature, G_TYPE_OBJECT)

struct _J2cAttributeRuntimeVisibleAnnotations
{
  GObject parent;
  GPtrArray *annotations; /* J2cAnnotation[] */
};

G_DEFINE_TYPE (J2cAttributeRuntimeVisibleAnnotations, j2c_attribute_runtime_visible_annotations, G_TYPE_OBJECT)

struct _J2cAttributeRuntimeInvisibleAnnotations
{
  GObject parent;
  GPtrArray *annotations; /* J2cAnnotation[] */
};

G_DEFINE_TYPE (J2cAttributeRuntimeInvisibleAnnotations, j2c_attribute_runtime_invisible_annotations, G_TYPE_OBJECT)

struct _J2cAnnotation
{
  GObject parent;

  guint16 type_index;
  GPtrArray *element_value_pairs; /* J2cElementValuePair[] */
};

G_DEFINE_TYPE(J2cAnnotation, j2c_annotation, G_TYPE_OBJECT)

struct _J2cElementValuePair
{
  GObject parent;

  guint16 element_name_index;
  J2cElementValue *value;
};

G_DEFINE_TYPE(J2cElementValuePair, j2c_element_value_pair, G_TYPE_OBJECT)

struct _J2cElementValue
{
  GObject parent;

  guint8 tag;
  /***
  J2cElementValue.tag is a single ASCII character denoting the type of
  this union.

    B -> constant_value_index  CONSTANT_Integer
    C -> constant_value_index  CONSTANT_Integer
    D -> constant_value_index  CONSTANT_Double
    F -> constant_value_index  CONSTANT_Float
    I -> constant_value_index  CONSTANT_Integer
    J -> constant_value_index  CONSTANT_Long
    S -> constant_value_index  CONSTANT_Integer
    Z -> constant_value_index  CONSTANT_Integer
    s -> constant_value_index  CONSTANT_Utf8

    e -> enum_const_value { type_name_index; const_name_index; }
    c -> class_info_index
    @ -> annotation_value
    [ -> array_value
  ***/
  union
    {
      guint16 const_value_index;

      struct
        {
          guint16 type_name_index;
          guint16 const_name_index;
        };

      guint16 class_info_index;
      J2cAnnotation *annotation_value;
      J2cObjectArray *array_value; /* J2cElementValue[] */
    };
};

G_DEFINE_TYPE(J2cElementValue, j2c_element_value, G_TYPE_OBJECT)

enum
{
  PROP_ELEMENT_VALUE_PAIRS = 1,
  PROP_TYPE_INDEX,
  PROP_ELEMENT_NAME_INDEX,
  PROP_VALUE,
  PROP_TAG,
  PROP_CONSTANT_VALUE_INDEX,
  PROP_TYPE_NAME_INDEX,
  PROP_CONST_NAME_INDEX,
  PROP_CLASS_INFO_INDEX,
  PROP_ANNOTATION_VALUE,
  PROP_ARRAY_VALUE,
  PROP_SIGNATURE_INDEX,
  PROP_ANNOTATIONS
};

/****
  PRIVATE METHOD PROTOTYPES
 ****/

static void j2c_attribute_runtime_visible_annotations_dispose (GObject *object);
static void j2c_attribute_runtime_invisible_annotations_dispose (GObject *object);
static void j2c_annotation_dispose (GObject *object);
static void j2c_element_value_pair_dispose (GObject *object);
static void j2c_element_value_dispose (GObject *object);

static void j2c_attribute_signature_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_runtime_visible_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_attribute_runtime_invisible_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_annotation_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_element_value_pair_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void j2c_element_value_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void j2c_attribute_signature_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_runtime_visible_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_attribute_runtime_invisible_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_annotation_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_element_value_pair_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void j2c_element_value_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

/****
  CONSTRUCTION METHODS
 ****/

J2cAnnotation *
j2c_annotation_new_from_stream (GDataInputStream *in, GError **error)
{
  g_return_val_if_fail (in != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  GError *tmp_error = NULL;
  J2cAnnotation *ret = NULL;

  guint16 type_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto end;
  guint16 num_element_value_pairs = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto end;

  GPtrArray *element_value_pairs = g_ptr_array_sized_new (num_element_value_pairs);
  g_ptr_array_set_free_func (element_value_pairs, g_object_unref);
  for (gint i = 0; i < num_element_value_pairs; i ++)
    {
      J2cElementValuePair *pair = j2c_element_value_pair_new_from_stream (in, &tmp_error);
      if (tmp_error)
        {
          g_ptr_array_unref (element_value_pairs);
          goto end;
        }
      g_ptr_array_add (element_value_pairs, pair);
    }
  ret = g_object_new (J2C_TYPE_ANNOTATION,
                      J2C_ATTRIBUTE_PROP_TYPE_INDEX, type_index,
                      J2C_ATTRIBUTE_PROP_ELEMENT_VALUE_PAIRS, element_value_pairs,
                      NULL);
end:
  if (tmp_error) g_propagate_error (error, tmp_error);
  return ret;
}

J2cElementValuePair *
j2c_element_value_pair_new_from_stream (GDataInputStream *in, GError **error)
{
  g_return_val_if_fail (in != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  GError *tmp_error = NULL;
  J2cElementValuePair *ret = NULL;

  guint16 element_name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
  if (tmp_error) goto end;
  J2cElementValue *value = j2c_element_value_new_from_stream(in, &tmp_error);
  if (tmp_error) goto end;

  ret = g_object_new (J2C_TYPE_ELEMENT_VALUE_PAIR,
                      J2C_ATTRIBUTE_PROP_ELEMENT_NAME_INDEX, element_name_index,
                      J2C_ATTRIBUTE_PROP_VALUE, value,
                      NULL);
  g_object_unref (value);
end:
  if (tmp_error) g_propagate_error (error, tmp_error);
  return ret;
}

J2cElementValue *
j2c_element_value_new_from_stream (GDataInputStream *in, GError **error)
{
  g_return_val_if_fail (in != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  GError *tmp_error = NULL;
  J2cElementValue *ret = NULL;
  J2cAnnotation *annotation_value = NULL;
  J2cObjectArray *array_value = NULL;

  guint8 tag = g_data_input_stream_read_byte (in, NULL, &tmp_error);
  if (tmp_error) goto end;

  guint16 const_value_index;
  guint16 type_name_index;
  guint16 const_name_index;
  guint16 class_info_index;
  guint16 num_values;

  switch (tag)
    {
      case '[':
        num_values = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
        if (tmp_error) goto end;

        array_value = j2c_object_array_sized_new (num_values);
        for (gint i = 0; i < num_values; i++)
          {
            J2cElementValue *value = j2c_element_value_new_from_stream(in, &tmp_error);
            if (tmp_error) goto end;
            j2c_object_array_add (array_value, value);
            g_object_unref (value);
          }
        ret = g_object_new (J2C_TYPE_ELEMENT_VALUE,
                            J2C_ATTRIBUTE_PROP_TAG, tag,
                            J2C_ATTRIBUTE_PROP_ARRAY_VALUE, array_value,
                            NULL);
        break;
      case '@':
        annotation_value = j2c_annotation_new_from_stream(in, &tmp_error);
        if (tmp_error) goto end;
        ret = g_object_new (J2C_TYPE_ELEMENT_VALUE,
                            J2C_ATTRIBUTE_PROP_TAG, tag,
                            J2C_ATTRIBUTE_PROP_ANNOTATION_VALUE, annotation_value,
                            NULL);
        break;
      case 'c':
        class_info_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
        if (tmp_error) goto end;
        ret = g_object_new (J2C_TYPE_ELEMENT_VALUE,
                            J2C_ATTRIBUTE_PROP_TAG, tag,
                            J2C_ATTRIBUTE_PROP_CLASS_INFO_INDEX, class_info_index,
                            NULL);
        break;
      case 'e':
        type_name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
        if (tmp_error) goto end;
        const_name_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
        if (tmp_error) goto end;
        ret = g_object_new (J2C_TYPE_ELEMENT_VALUE,
                            J2C_ATTRIBUTE_PROP_TAG, tag,
                            J2C_ATTRIBUTE_PROP_TYPE_NAME_INDEX, type_name_index,
                            J2C_ATTRIBUTE_PROP_CONST_NAME_INDEX, const_name_index,
                            NULL);
        break;
      default:
        const_value_index = g_data_input_stream_read_uint16 (in, NULL, &tmp_error);
        if (tmp_error) goto end;
        ret = g_object_new (J2C_TYPE_ELEMENT_VALUE,
                            J2C_ATTRIBUTE_PROP_TAG, tag,
                            J2C_ATTRIBUTE_PROP_CONSTANT_VALUE_INDEX, const_value_index,
                            NULL);
        break;
    }

end:
  if (tmp_error) g_propagate_error (error, tmp_error);
  if (annotation_value) g_object_unref (annotation_value);
  if (array_value) g_object_unref (array_value);
  return ret;
}

static void
j2c_attribute_synthetic_class_init (J2cAttributeSyntheticClass *klass)
{
  return;
}

static void
j2c_attribute_deprecated_class_init (J2cAttributeDeprecatedClass *klass)
{
  return;
}

static void
j2c_attribute_signature_class_init (J2cAttributeSignatureClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_signature_set_property;
  object_class->get_property = j2c_attribute_signature_get_property;

  g_object_class_install_property (object_class, PROP_SIGNATURE_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_SIGNATURE_INDEX,
                                                      J2C_ATTRIBUTE_PROP_SIGNATURE_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_runtime_visible_annotations_class_init (J2cAttributeRuntimeVisibleAnnotationsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_runtime_visible_annotations_set_property;
  object_class->get_property = j2c_attribute_runtime_visible_annotations_get_property;
  object_class->dispose      = j2c_attribute_runtime_visible_annotations_dispose;

  g_object_class_install_property (object_class, PROP_ANNOTATIONS,
                                   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_ANNOTATIONS,
                                                       J2C_ATTRIBUTE_PROP_ANNOTATIONS,
                                                       "",
                                                       G_TYPE_PTR_ARRAY,
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_attribute_runtime_invisible_annotations_class_init (J2cAttributeRuntimeInvisibleAnnotationsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_attribute_runtime_invisible_annotations_set_property;
  object_class->get_property = j2c_attribute_runtime_invisible_annotations_get_property;
  object_class->dispose      = j2c_attribute_runtime_invisible_annotations_dispose;

  g_object_class_install_property (object_class, PROP_ANNOTATIONS,
                                   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_ANNOTATIONS,
                                                       J2C_ATTRIBUTE_PROP_ANNOTATIONS,
                                                       "",
                                                       G_TYPE_PTR_ARRAY,
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_annotation_class_init (J2cAnnotationClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_annotation_set_property;
  object_class->get_property = j2c_annotation_get_property;
  object_class->dispose      = j2c_annotation_dispose;

  g_object_class_install_property (object_class, PROP_TYPE_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TYPE_INDEX,
                                                      J2C_ATTRIBUTE_PROP_TYPE_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_ELEMENT_VALUE_PAIRS,
                                   g_param_spec_boxed (J2C_ATTRIBUTE_PROP_ELEMENT_VALUE_PAIRS,
                                                       J2C_ATTRIBUTE_PROP_ELEMENT_VALUE_PAIRS,
                                                       "",
                                                       G_TYPE_PTR_ARRAY,
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_element_value_pair_class_init (J2cElementValuePairClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_element_value_pair_set_property;
  object_class->get_property = j2c_element_value_pair_get_property;
  object_class->dispose      = j2c_element_value_pair_dispose;

  g_object_class_install_property (object_class, PROP_ELEMENT_NAME_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_ELEMENT_NAME_INDEX,
                                                      J2C_ATTRIBUTE_PROP_ELEMENT_NAME_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_VALUE,
                                   g_param_spec_object (J2C_ATTRIBUTE_PROP_VALUE,
                                                        J2C_ATTRIBUTE_PROP_VALUE,
                                                        "",
                                                        J2C_TYPE_ELEMENT_VALUE,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_element_value_class_init (J2cElementValueClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_element_value_set_property;
  object_class->get_property = j2c_element_value_get_property;
  object_class->dispose      = j2c_element_value_dispose;

  g_object_class_install_property (object_class, PROP_TAG,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TAG,
                                                      J2C_ATTRIBUTE_PROP_TAG,
                                                      "",
                                                      0, G_MAXUINT8,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_CONSTANT_VALUE_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_CONSTANT_VALUE_INDEX,
                                                      J2C_ATTRIBUTE_PROP_CONSTANT_VALUE_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_TYPE_NAME_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_TYPE_NAME_INDEX,
                                                      J2C_ATTRIBUTE_PROP_TYPE_NAME_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_CONST_NAME_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_CONST_NAME_INDEX,
                                                      J2C_ATTRIBUTE_PROP_CONST_NAME_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_CLASS_INFO_INDEX,
                                   g_param_spec_uint (J2C_ATTRIBUTE_PROP_CLASS_INFO_INDEX,
                                                      J2C_ATTRIBUTE_PROP_CLASS_INFO_INDEX,
                                                      "",
                                                      0, G_MAXUINT16,
                                                      0,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_ANNOTATION_VALUE,
                                   g_param_spec_object (J2C_ATTRIBUTE_PROP_ANNOTATION_VALUE,
                                                        J2C_ATTRIBUTE_PROP_ANNOTATION_VALUE,
                                                        "",
                                                        J2C_TYPE_ANNOTATION,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_ARRAY_VALUE,
                                   g_param_spec_object (J2C_ATTRIBUTE_PROP_ARRAY_VALUE,
                                                        J2C_ATTRIBUTE_PROP_ARRAY_VALUE,
                                                        "",
                                                        J2C_TYPE_OBJECT_ARRAY,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void j2c_attribute_synthetic_init (J2cAttributeSynthetic *self) { return; }
static void j2c_attribute_deprecated_init (J2cAttributeDeprecated *self) { return; }
static void j2c_attribute_signature_init (J2cAttributeSignature *self) { return; }
static void j2c_attribute_runtime_visible_annotations_init (J2cAttributeRuntimeVisibleAnnotations *self) { return; }
static void j2c_attribute_runtime_invisible_annotations_init (J2cAttributeRuntimeInvisibleAnnotations *self) { return; }
static void j2c_annotation_init (J2cAnnotation *self) { return; }
static void j2c_element_value_pair_init (J2cElementValuePair *self) { return; }
static void j2c_element_value_init (J2cElementValue *self) { return; }

/****
  DESTRUCTION METHODS
 ****/

static void
j2c_attribute_runtime_visible_annotations_dispose (GObject *object)
{
  J2cAttributeRuntimeVisibleAnnotations *self = J2C_ATTRIBUTE_RUNTIME_VISIBLE_ANNOTATIONS (object);

  if (self->annotations)
    {
      g_ptr_array_unref (self->annotations);
      self->annotations = NULL;
    }

  G_OBJECT_CLASS (j2c_attribute_runtime_visible_annotations_parent_class)->dispose (object);
}

static void
j2c_attribute_runtime_invisible_annotations_dispose (GObject *object)
{
  J2cAttributeRuntimeInvisibleAnnotations *self = J2C_ATTRIBUTE_RUNTIME_INVISIBLE_ANNOTATIONS (object);

  if (self->annotations)
    {
      g_ptr_array_unref (self->annotations);
      self->annotations = NULL;
    }

  G_OBJECT_CLASS (j2c_attribute_runtime_invisible_annotations_parent_class)->dispose (object);
}

static void
j2c_annotation_dispose (GObject *object)
{
  J2cAnnotation *self = J2C_ANNOTATION (object);

  if (self->element_value_pairs)
    {
      g_ptr_array_unref (self->element_value_pairs);
      self->element_value_pairs = NULL;
    }

  G_OBJECT_CLASS (j2c_annotation_parent_class)->dispose (object);
}

static void
j2c_element_value_pair_dispose (GObject *object)
{
  J2cElementValuePair *self = J2C_ELEMENT_VALUE_PAIR (object);

  if (self->value)
    {
      g_clear_object (&self->value);
    }

  G_OBJECT_CLASS (j2c_element_value_pair_parent_class)->dispose (object);
}

static void
j2c_element_value_dispose (GObject *object)
{
  J2cElementValue *self = J2C_ELEMENT_VALUE (object);

  if (self->tag == '@')
    {
      if (self->annotation_value)
        g_clear_object (&self->annotation_value);
    }
  else if (self->tag == '[')
    {
      if (self->array_value)
        g_clear_object (&self->array_value);
    }

  G_OBJECT_CLASS (j2c_element_value_parent_class)->dispose (object);
}

/****
  GETTERS AND SETTERS
 ****/

static void
j2c_attribute_signature_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeSignature *self = J2C_ATTRIBUTE_SIGNATURE (object);

  switch (property_id)
    {
      case PROP_SIGNATURE_INDEX:
        self->signature_index = g_value_get_uint (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_attribute_runtime_visible_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeRuntimeVisibleAnnotations *self = J2C_ATTRIBUTE_RUNTIME_VISIBLE_ANNOTATIONS (object);

  switch (property_id)
    {
      case PROP_ANNOTATIONS:
        if (self->annotations)
          g_ptr_array_unref (self->annotations);
        self->annotations = g_value_get_boxed (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_attribute_runtime_invisible_annotations_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAttributeRuntimeInvisibleAnnotations *self = J2C_ATTRIBUTE_RUNTIME_INVISIBLE_ANNOTATIONS (object);

  switch (property_id)
    {
      case PROP_ANNOTATIONS:
        if (self->annotations)
          g_ptr_array_unref (self->annotations);
        self->annotations = g_value_get_boxed (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_annotation_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cAnnotation *self = J2C_ANNOTATION (object);

  switch (property_id)
    {
      case PROP_TYPE_INDEX:
        self->type_index = g_value_get_uint (value);
        break;
      case PROP_ELEMENT_VALUE_PAIRS:
        if (self->element_value_pairs)
          g_ptr_array_unref (self->element_value_pairs);
        self->element_value_pairs = g_value_get_boxed (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_element_value_pair_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cElementValuePair *self = J2C_ELEMENT_VALUE_PAIR (object);

  switch (property_id)
    {
      case PROP_ELEMENT_NAME_INDEX:
        self->element_name_index = (guint16) g_value_get_uint (value);
        break;
      case PROP_VALUE:
        if (self->value)
          g_object_unref (self->value);
        self->value = g_value_dup_object (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}


static void
j2c_element_value_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cElementValue *self = J2C_ELEMENT_VALUE (object);

  if (property_id == PROP_TAG)
    {
      self->tag = g_value_get_uint (value);
      return;
    }

  switch (self->tag)
    {
      case 'B':
      case 'C':
      case 'D':
      case 'F':
      case 'I':
      case 'J':
      case 'S':
      case 'Z':
      case 's':
        if (property_id == PROP_CONSTANT_VALUE_INDEX)
          self->const_value_index = g_value_get_uint (value);
        else
          goto invalid_for_this_tag;
        break;
      case 'e':
        if (property_id == PROP_TYPE_NAME_INDEX)
          self->type_name_index = g_value_get_uint (value);
        else if (property_id == PROP_CONST_NAME_INDEX)
          self->const_name_index = g_value_get_uint (value);
        else
          goto invalid_for_this_tag;
        break;
      case 'c':
        if (property_id == PROP_CLASS_INFO_INDEX)
          self->class_info_index = g_value_get_uint (value);
        else
          goto invalid_for_this_tag;
        break;
      case '@':
        if (property_id == PROP_ANNOTATION_VALUE)
          {
            if (self->annotation_value)
              g_object_unref (self->annotation_value);
            self->annotation_value = g_value_dup_object (value);
          }
        else
          goto invalid_for_this_tag;
        break;
      case '[':
        if (property_id == PROP_ARRAY_VALUE)
          {
            if (self->array_value)
              g_object_unref (self->array_value);
            self->array_value = g_value_dup_object (value);
          }
        else
          goto invalid_for_this_tag;
        break;
      default:
        g_warning ("Could not parse J2cElementValue tag %c", self->tag);
    }

invalid_for_this_tag:
  if ( (property_id == PROP_CONSTANT_VALUE_INDEX)
    || (property_id == PROP_TYPE_NAME_INDEX)
    || (property_id == PROP_CONST_NAME_INDEX)
    || (property_id == PROP_CLASS_INFO_INDEX)
    || (property_id == PROP_ANNOTATION_VALUE)
    || (property_id == PROP_ARRAY_VALUE) )
    return; /* Valid property id, but should be ignored given the value of tag */
  else
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static void
j2c_attribute_signature_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeSignature *self = J2C_ATTRIBUTE_SIGNATURE (object);

  switch (property_id)
    {
      case PROP_SIGNATURE_INDEX:
        g_value_set_uint (value, self->signature_index);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_attribute_runtime_visible_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeRuntimeVisibleAnnotations *self = J2C_ATTRIBUTE_RUNTIME_VISIBLE_ANNOTATIONS (object);

  switch (property_id)
    {
      case PROP_ANNOTATIONS:
        g_value_set_boxed (value, self->annotations);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_attribute_runtime_invisible_annotations_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAttributeRuntimeInvisibleAnnotations *self = J2C_ATTRIBUTE_RUNTIME_INVISIBLE_ANNOTATIONS (object);

  switch (property_id)
    {
      case PROP_ANNOTATIONS:
        g_value_set_boxed (value, self->annotations);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_annotation_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cAnnotation *self = J2C_ANNOTATION (object);

  switch (property_id)
    {
      case PROP_TYPE_INDEX:
        g_value_set_uint (value, self->type_index);
        break;
      case PROP_ELEMENT_VALUE_PAIRS:
        g_value_set_boxed (value, self->element_value_pairs);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_element_value_pair_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cElementValuePair *self = J2C_ELEMENT_VALUE_PAIR (object);

  switch (property_id)
    {
      case PROP_ELEMENT_NAME_INDEX:
        g_value_set_uint (value, (guint) self->element_name_index);
        break;
      case PROP_VALUE:
        g_value_set_object (value, self->value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}


static void
j2c_element_value_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cElementValue *self = J2C_ELEMENT_VALUE (object);

  if (property_id == PROP_TAG)
    {
      g_value_set_uint (value, self->tag);
      return;
    }

  switch (self->tag)
    {
      case 'B':
      case 'C':
      case 'D':
      case 'F':
      case 'I':
      case 'J':
      case 'S':
      case 'Z':
      case 's':
        if (property_id == PROP_CONSTANT_VALUE_INDEX)
          g_value_set_uint (value, self->const_value_index);
        else
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
      case 'e':
        if (property_id == PROP_TYPE_NAME_INDEX)
          g_value_set_uint (value, self->type_name_index);
        else if (property_id == PROP_CONST_NAME_INDEX)
          g_value_set_uint (value, self->const_name_index);
        else
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
      case 'c':
        if (property_id == PROP_CLASS_INFO_INDEX)
          g_value_set_uint (value, self->class_info_index);
        else
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
      case '@':
        if (property_id == PROP_ANNOTATION_VALUE)
          g_value_set_object (value, self->annotation_value);
        else
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
      case '[':
        if (property_id == PROP_ARRAY_VALUE)
          g_value_set_object (value, self->array_value);
        else
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
      default:
        g_warning ("Could not parse J2cElementValue tag %c", self->tag);
    }
}
