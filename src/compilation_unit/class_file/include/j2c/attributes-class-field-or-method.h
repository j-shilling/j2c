#ifndef __ATTRIBUTES_CLASS_FIELD_OR_METHOD_H__
#define __ATTRIBUTES_CLASS_FIELD_OR_METHOD_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define J2C_TYPE_ATTRIBUTE_SYNTHETIC    j2c_attribute_synthetic_get_type()
G_DECLARE_FINAL_TYPE(J2cAttributeSynthetic, j2c_attribute_synthetic, J2C, ATTRIBUTE_SYNTHETIC, GObject)
static gchar const *const J2C_SYNTHETIC = "Synthetic";
#define J2C_TYPE_ATTRIBUTE_DEPRECATED    j2c_attribute_deprecated_get_type()
G_DECLARE_FINAL_TYPE(J2cAttributeDeprecated, j2c_attribute_deprecated, J2C, ATTRIBUTE_DEPRECATED, GObject)
static gchar const *const J2C_DEPRECATED = "Deprecated";
#define J2C_TYPE_ATTRIBUTE_SIGNATURE    j2c_attribute_signature_get_type()
G_DECLARE_FINAL_TYPE(J2cAttributeSignature, j2c_attribute_signature, J2C, ATTRIBUTE_SIGNATURE, GObject)
static gchar const *const J2C_SIGNATURE = "Signature";
#define J2C_TYPE_ATTRIBUTE_RUNTIME_VISIBLE_ANNOTATIONS   j2c_attribute_runtime_visible_annotations_get_type ()
G_DECLARE_FINAL_TYPE(J2cAttributeRuntimeVisibleAnnotations, j2c_attribute_runtime_visible_annotations, J2C, ATTRIBUTE_RUNTIME_VISIBLE_ANNOTATIONS, GObject)
static gchar const *const J2C_RUNTIME_VISIBLE_ANNOTATIONS = "RuntimeVisibleAnnotations";
#define J2C_TYPE_ATTRIBUTE_RUNTIME_INVISIBLE_ANNOTATIONS   j2c_attribute_runtime_invisible_annotations_get_type ()
G_DECLARE_FINAL_TYPE(J2cAttributeRuntimeInvisibleAnnotations, j2c_attribute_runtime_invisible_annotations, J2C, ATTRIBUTE_RUNTIME_INVISIBLE_ANNOTATIONS, GObject)
static gchar const *const J2C_RUNTIME_INVISIBLE_ANNOTATIONS = "RuntimeInvisibleAnnotations";

#define J2C_TYPE_ANNOTATION j2c_annotation_get_type ()
G_DECLARE_FINAL_TYPE(J2cAnnotation, j2c_annotation, J2C, ANNOTATION, GObject)
#define J2C_TYPE_ELEMENT_VALUE j2c_element_value_get_type ()
G_DECLARE_FINAL_TYPE(J2cElementValue, j2c_element_value, J2C, ELEMENT_VALUE, GObject)
#define J2C_TYPE_ELEMENT_VALUE_PAIR j2c_element_value_pair_get_type ()
G_DECLARE_FINAL_TYPE(J2cElementValuePair, j2c_element_value_pair, J2C, ELEMENT_VALUE_PAIR, GObject)

J2cAnnotation *j2c_annotation_new_from_stream (GDataInputStream *in, GError **error);
J2cElementValuePair *j2c_element_value_pair_new_from_stream (GDataInputStream *in, GError **error);
J2cElementValue *j2c_element_value_new_from_stream (GDataInputStream *in, GError **error);

G_END_DECLS

#endif // __ATTRIBUTES_CLASS_FIELD_OR_METHOD_H__
