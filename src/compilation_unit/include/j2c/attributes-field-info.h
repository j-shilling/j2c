#ifndef __ATTRIBUTES_FIELD_INFO_H__
#define __ATTRIBUTES_FIELD_INFO_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_TYPE_ATTRIBUTE_CONSTANT_VALUE j2c_attribute_constant_value_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeConstantValue, j2c_attribute_constant_value, J2C, ATTRIBUTE_CONSTANT_VALUE, GObject)
static gchar const *const J2C_CONSTANT_VALUE = "ConstantValue";

static gchar const *const J2C_ATTRIBUTE_PROP_CONSTANT_VALUE_INDEX = "attribute-constant-value-index";

G_END_DECLS

#endif /* __ATTRIBUTES_FIELD_INFO_H__ */
