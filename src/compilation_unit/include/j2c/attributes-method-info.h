#ifndef __ATTRIBUTES_METHOD_INFO_H__
#define __ATTRIBUTES_METHOD_INFO_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_TYPE_ATTRIBUTE_CODE j2c_attribute_code_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeCode, j2c_attribute_code, J2C, ATTRIBUTE_CODE, GObject)
static gchar const *const J2C_CODE = "Code";
#define J2C_TYPE_ATTRIBUTE_EXCEPTIONS j2c_attribute_exceptions_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeExceptions, j2c_attribute_exceptions, J2C, ATTRIBUTE_EXCEPTIONS, GObject)
static gchar const *const J2C_EXCEPTIONS = "Exceptions";
#define J2C_TYPE_ATTRIBUTE_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS j2c_attribute_runtime_visible_parameter_annotations_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeRuntimeVisibleParameterAnnotations, j2c_attribute_runtime_visible_parameter_annotations, J2C, ATTRIBUTE_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS, GObject)
static gchar const *const J2C_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS = "RuntimeVisibleParameterAnnotations";
#define J2C_TYPE_ATTRIBUTE_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS j2c_attribute_runtime_invisible_parameter_annotations_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeRuntimeInvisibleParameterAnnotations, j2c_attribute_runtime_invisible_parameter_annotations, J2C, ATTRIBUTE_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS, GObject)
static gchar const *const J2C_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS = "RuntimeInvisibleParameterAnnotations";
#define J2C_TYPE_ATTRIBUTE_METHOD_PARAMETERS j2c_attribute_method_parameters_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeMethodParameters, j2c_attribute_method_parameters, J2C, ATTRIBUTE_METHOD_PARAMETERS, GObject)
static gchar const *const J2C_METHOD_PARAMETERS = "MethodParameters";
#define J2C_TYPE_ATTRIBUTE_ANNOTATION_DEFAULT j2c_attribute_annotation_default_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeAnnotationDefault, j2c_attribute_annotation_default, J2C, ATTRIBUTE_ANNOTATION_DEFAULT, GObject)
static gchar const *const J2C_ANNOTATION_DEFAULT = "AnnotationDefault";





#define J2C_TYPE_EXCEPTION_INFO j2c_exception_info_get_type ()
G_DECLARE_FINAL_TYPE (J2cExceptionInfo, j2c_exception_info, J2C, EXCEPTION_INFO, GObject)
#define J2C_TYPE_PARAMETER_ANNOTATIONS j2c_parameter_annotations_get_type ()
G_DECLARE_FINAL_TYPE (J2cParameterAnnotations, j2c_parameter_annotations, J2C, PARAMETER_ANNOTATIONS, GObject)
#define J2C_TYPE_PARAMETER j2c_parameter_get_type ()
G_DECLARE_FINAL_TYPE (J2cParameter, j2c_parameter, J2C, PARAMETER, GObject)







G_END_DECLS

#endif /* __ATTRIBUTES_METHOD_INFO_H__ */
