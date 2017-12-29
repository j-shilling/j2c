#ifndef __ATTRIBUTES_CLASS_FIELD_OR_METHOD_H__
#define __ATTRIBUTES_CLASS_FIELD_OR_METHOD_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

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
