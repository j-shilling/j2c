#ifndef __ATTRIBUTES_H__
#define __ATTRIBUTES_H__

/* Include types assiciated with different parts of the .class file */
#include <j2c/attributes-class-file.h>
#include <j2c/attributes-field-info.h>
#include <j2c/attributes-method-info.h>
#include <j2c/attributes-code.h>
#include <j2c/attributes-class-field-or-method.h>
#include <j2c/attributes-any.h>
#include <j2c/constant-pool.h>

#include <glib.h>
#include <gio/gio.h>

#define J2C_ATTRIBUTE_ERROR g_quark_from_static_string ("j2c-attribute")

typedef enum
{
  J2C_UNKNOWN_ATTRIBUTE_ERROR
} J2cAttributeError;

gpointer j2c_read_attribute (GType type, GDataInputStream *in, J2cConstantPool *cp, GError **error);

/* Define property names */

static gchar const *const J2C_ATTRIBUTE_PROP_SOURCE_FILE_INDEX = "attribute-source-file-index";
static gchar const *const J2C_ATTRIBUTE_PROP_CLASSES = "attribute-classes";
static gchar const *const J2C_ATTRIBUTE_PROP_INNER_CLASS_INFO_INDEX = "attribute-inner-class-info";
static gchar const *const J2C_ATTRIBUTE_PROP_OUTER_CLASS_INFO_INDEX = "attribute-outer-class-info";
static gchar const *const J2C_ATTRIBUTE_PROP_INNER_NAME_INDEX = "attribute-inner-name-index";
static gchar const *const J2C_ATTRIBUTE_PROP_INNER_CLASS_ACCESS_FLAGS = "attribute-inner-class-access-flags";
static gchar const *const J2C_ATTRIBUTE_PROP_CLASS_INDEX = "attribute-class-index";
static gchar const *const J2C_ATTRIBUTE_PROP_METHOD_INDEX = "attribute-method-index";
static gchar const *const J2C_ATTRIBUTE_PROP_DEBUG_EXTENSION = "attribute-debug-extension";
static gchar const *const J2C_ATTRIBUTE_PROP_BOOTSTRAP_METHODS = "attribute-bootstrap-methods";
static gchar const *const J2C_ATTRIBUTE_PROP_BOOTSTRAP_METHOD_REF = "attribute-bootstrap-method-ref";
static gchar const *const J2C_ATTRIBUTE_PROP_BOOTSTRAP_ARGUMENTS = "attribute-bootstrap-arguments";

static gchar const *const J2C_ATTRIBUTE_PROP_CONSTANT_VALUE_INDEX = "attribute-constant-value-index";

static gchar const *const J2C_ATTRIBUTE_PROP_MAX_STACK = "attribute-max-stack";
static gchar const *const J2C_ATTRIBUTE_PROP_MAX_LOCALS = "attribute-max-locals";
static gchar const *const J2C_ATTRIBUTE_PROP_CODE = "attribute-code";
static gchar const *const J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE= "attribute-exception_table";
static gchar const *const J2C_ATTRIBUTE_PROP_START_PC= "attribute-start-pc";
static gchar const *const J2C_ATTRIBUTE_PROP_END_PC= "attribute-end-pc";
static gchar const *const J2C_ATTRIBUTE_PROP_HANDLER_PC= "attribute-handler-pc";
static gchar const *const J2C_ATTRIBUTE_PROP_CATCH_TYPE = "attribute-catch-type";
static gchar const *const J2C_ATTRIBUTE_PROP_ATTRIBUTES = "attribute-attributes";
static gchar const *const J2C_ATTRIBUTE_PROP_EXCEPTION_INDEX_TABLE = "attribute-exception-index-table";
static gchar const *const J2C_ATTRIBUTE_PROP_PARAMETER_ANNOTATIONS = "attribute-parameter-annotations";
static gchar const *const J2C_ATTRIBUTE_PROP_ANNOTATIONS = "attribute-annotations";
static gchar const *const J2C_ATTRIBUTE_PROP_PARAMETERS = "attribute-parameters";
static gchar const *const J2C_ATTRIBUTE_PROP_DEFAULT_VALUE = "attribute-default-value";
static gchar const *const J2C_ATTRIBUTE_PROP_NAME_INDEX = "attribute-name-index";
static gchar const *const J2C_ATTRIBUTE_PROP_ACCESS_FLAGS = "attribute-access-flags";

static gchar const *const J2C_ATTRIBUTE_PROP_TYPE_INDEX = "attribute-type-index";
static gchar const *const J2C_ATTRIBUTE_PROP_ELEMENT_VALUE_PAIRS = "attribute-element-value-pairs";
static gchar const *const J2C_ATTRIBUTE_PROP_ELEMENT_NAME_INDEX = "attribute-element-name-index";
static gchar const *const J2C_ATTRIBUTE_PROP_VALUE = "attribute-value";
static gchar const *const J2C_ATTRIBUTE_PROP_TAG = "attribute-tag";
static gchar const *const J2C_ATTRIBUTE_PROP_TYPE_NAME_INDEX = "attribute-type-name-index";
static gchar const *const J2C_ATTRIBUTE_PROP_CONST_NAME_INDEX = "attribute-const-name-index";
static gchar const *const J2C_ATTRIBUTE_PROP_CLASS_INFO_INDEX = "attribue-class-info-index";
static gchar const *const J2C_ATTRIBUTE_PROP_ANNOTATION_VALUE = "attribute-annotation-value";
static gchar const *const J2C_ATTRIBUTE_PROP_ARRAY_VALUE = "attribute-array_value";

static gchar const *const J2C_ATTRIBUTE_PROP_LINE_NUMBER_TABLE = "attribute-line-number-table";
static gchar const *const J2C_ATTRIBUTE_PROP_LINE_NUMBER = "attribute-line-number";
static gchar const *const J2C_ATTRIBUTE_PROP_LOCAL_VARIABLE_TABLE = "attribute-local-variable-table";
static gchar const *const J2C_ATTRIBUTE_PROP_LENGTH = "attribute-length";
static gchar const *const J2C_ATTRIBUTE_PROP_DESCRIPTOR_INDEX = "attribute-descriptor-index";
static gchar const *const J2C_ATTRIBUTE_PROP_INDEX = "attribute-index";
static gchar const *const J2C_ATTRIBUTE_PROP_SIGNATURE_INDEX = "attribute-signature-index";
static gchar const *const J2C_ATTRIBUTE_PROP_LOCAL_VARIABLE_TYPE_TABLE = "attribute-local-variable-type-table";
static gchar const *const J2C_ATTRIBUTE_PROP_ENTRIES = "attribute-entries";
static gchar const *const J2C_ATTRIBUTE_PROP_OFFSET_DELTA = "attribute-offset-delta";
static gchar const *const J2C_ATTRIBUTE_PROP_LOCALS = "attribute-locals";
static gchar const *const J2C_ATTRIBUTE_PROP_STACK = "attribute-stack";
static gchar const *const J2C_ATTRIBUTE_PROP_CPOOL_INDEX = "attribute-cpool-index";
static gchar const *const J2C_ATTRIBUTE_PROP_OFFSET = "attribute-offset";

static gchar const *const J2C_ATTRIBUTE_PROP_TYPE_ANNOTATIONS = "attribute-type-annotations";
static gchar const *const J2C_ATTRIBUTE_PROP_TARGET_TYPE = "attribute-target-type";
static gchar const *const J2C_ATTRIBUTE_PROP_SUPERTYPE_INDEX = "attribute-supertype-index";
static gchar const *const J2C_ATTRIBUTE_PROP_TYPE_PARAMETER_INDEX = "attribute-type-parameter-index";
static gchar const *const J2C_ATTRIBUTE_PROP_BOUND_INDEX = "attribute-bound-index";
static gchar const *const J2C_ATTRIBUTE_PROP_FORMAL_PARAMETER_INDEX = "attribute-formal-parameter-index";
static gchar const *const J2C_ATTRIBUTE_PROP_THROWS_TYPE_INDEX = "attribute-throws-type-index";
static gchar const *const J2C_ATTRIBUTE_PROP_LOCALVAR_TARGET = "attribute-localvar-target";
static gchar const *const J2C_ATTRIBUTE_PROP_EXCEPTION_TABLE_INDEX = "attribute-exception-table-index";
static gchar const *const J2C_ATTRIBUTE_PROP_TYPE_ARGUMENT_INDEX = "attribute-argument-index";
static gchar const *const J2C_ATTRIBUTE_PROP_TARGET_PATH = "attribute-target-path";
static gchar const *const J2C_ATTRIBUTE_PROP_PATH = "attribute-path";
static gchar const *const J2C_ATTRIBUTE_PROP_TYPE_PATH_KIND = "attribute-path-kind";
static gchar const *const J2C_ATTRIBUTE_PROP_TARGET_INFO = "attribute-target-info";


#endif // __ATTRIBUTES_H__
