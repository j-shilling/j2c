#ifndef __ATTRIBUTES_H__
#define __ATTRIBUTES_H__

/* Include types assiciated with different parts of the .class file */
#include <j2c/attributes-class-file.h>
#include <j2c/attributes-field-info.h>
#include <j2c/attributes-method-info.h>

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

#endif // __ATTRIBUTES_H__
