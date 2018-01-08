#ifndef __ATTRIBUTES_ANY_H__
#define __ATTRIBUTES_ANY_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_TYPE_ATTRIBUTE_RUNTIME_VISIBLE_TYPE_ANNOTATIONS j2c_attribute_runtime_visible_type_annotations_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeRuntimeVisibleTypeAnnotations, j2c_attribute_runtime_visible_type_annotations, J2C, ATTRIBUTE_RUNTIME_VISIBLE_TYPE_ANNOTATIONS, GObject)
static gchar const *const J2C_RUNTIME_VISIBLE_TYPE_ANNOTATIONS = "RuntimeVisibleTypeAnnotations";
#define J2C_TYPE_ATTRIBUTE_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS j2c_attribute_runtime_invisible_type_annotations_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeRuntimeInvisibleTypeAnnotations, j2c_attribute_runtime_invisible_type_annotations, J2C, ATTRIBUTE_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS, GObject)
static gchar const *const J2C_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS = "RuntimeInvisibleTypeAnnotations";

#define J2C_TYPE_T_ANNOTATION    j2c_t_annotation_get_type ()
G_DECLARE_FINAL_TYPE(J2cTAnnotation, j2c_t_annotation, J2C, T_ANNOTATION, GObject)
#define J2C_TYPE_TARGET_INFO    j2c_target_info_get_type ()
G_DECLARE_FINAL_TYPE(J2cTargetInfo, j2c_target_info, J2C, TARGET_INFO, GObject);
#define J2C_TYPE_LOCALVAR  j2c_localvar_get_type ()
G_DECLARE_FINAL_TYPE(J2cLocalvar, j2c_localvar, J2C, LOCALVAR, GObject)
#define J2C_TYPE_T_PATH j2c_t_path_get_type ()
G_DECLARE_FINAL_TYPE(J2cTPath, j2c_t_path, J2C, T_PATH, GObject)
#define J2C_TYPE_PATH j2c_path_get_type ()
G_DECLARE_FINAL_TYPE(J2cPath, j2c_path, J2C, PATH, GObject)

G_END_DECLS

#endif // __ATTRIBUTES_ANY_H__
