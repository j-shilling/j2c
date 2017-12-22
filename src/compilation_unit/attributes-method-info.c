
#include <j2c/attributes-method-info.h>

struct _J2cAttributeCode
{
  GObject parent;

  guint16 max_stack;
  guint16 max_locals;

  GByteArray *code;
  GPtrArray *exception_table; /* J2cExceptionInfo[] */
  GPtrArray *attributes; /* J2cAttribute*[] */
};

G_DEFINE_TYPE(J2cAttributeCode, j2c_attribute_code, G_TYPE_OBJECT)

struct _J2cExceptionInfo
{
  GObject parent;

  guint16 start_pc;
  guint16 end_pc;
  guint16 handler_pc;
  guint16 catch_type;
};

G_DEFINE_TYPE (J2cExceptionInfo, j2c_exception_info, G_TYPE_OBJECT)

struct _J2cAttributeExceptions
{
  GObject parent;

  GArray *exception_index_table; /* guint16[] */
};

G_DEFINE_TYPE(J2cAttributeExceptions, j2c_attribute_exceptions, G_TYPE_OBJECT)

struct _J2cAttributeRuntimeVisibleParameterAnnotations
{
  GObject parent;

  GPtrArray *paremeter_annotations; /* J2cParameterAnnotation[] */
};

G_DEFINE_TYPE(J2cAttributeRuntimeVisibleParameterAnnotations, j2c_attribute_runtime_visible_parameter_annotations, G_TYPE_OBJECT)

struct _J2cAttributeRuntimeInvisibleParameterAnnotations
{
  GObject parent;

  GPtrArray *paremeter_annotations; /* J2cParameterAnnotation[] */
};

G_DEFINE_TYPE(J2cAttributeRuntimeInvisibleParameterAnnotations, j2c_attribute_runtime_invisible_parameter_annotations, G_TYPE_OBJECT)

struct _J2cParameterAnnotations
{
  GObject parent;

  GPtrArray *annotations; /* J2cAnnotation[] */
};

G_DEFINE_TYPE (J2cParameterAnnotations, j2c_parameter_annotations, G_TYPE_OBJECT)

struct _J2cAttributeMethodParameters
{
  GObject parent;

  GPtrArray *parameters; /* J2cParameter[] */
};

G_DEFINE_TYPE (J2cAttributeMethodParameters, j2c_attribute_method_parameters, G_TYPE_OBJECT)

struct _J2cParameter
{
  GObject parent;

  guint16 name_index;
  guint16 access_flags;
};

G_DEFINE_TYPE (J2cParameter, j2c_parameter, G_TYPE_OBJECT)

struct _J2cAttributeAnnotationDefault
{
  GObject parent;

  GBytes *default_value;
};

G_DEFINE_TYPE (J2cAttributeAnnotationDefault, j2c_attribute_annotation_default, G_TYPE_OBJECT)
