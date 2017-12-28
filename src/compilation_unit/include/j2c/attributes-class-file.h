#ifndef __ATTRIBUTES_CLASS_FILE_H__
#define __ATTRIBUTES_CLASS_FILE_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_TYPE_ATTRIBUTE_SOURCE_FILE j2c_attribute_source_file_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeSourceFile, j2c_attribute_source_file, J2C, ATTRIBUTE_SOURCE_FILE, GObject)
static gchar const *const J2C_SOURCE_FILE = "SourceFile";
#define J2C_TYPE_ATTRIBUTE_INNER_CLASSES j2c_attribute_inner_classes_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeInnerClasses, j2c_attribute_inner_classes, J2C, ATTRIBUTE_INNER_CLASSES, GObject)
static gchar const *const J2C_INNER_CLASSES = "InnerClasses";
#define J2C_TYPE_ATTRIBUTE_ENCLOSING_METHOD j2c_attribute_enclosing_method_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeEnclosingMethod, j2c_attribute_enclosing_method, J2C, ATTRIBUTE_ENCLOSING_METHOD, GObject)
static gchar const *const J2C_ENCLOSING_METHOD = "EnclosingMethod";
#define J2C_TYPE_ATTRIBUTE_SOURCE_DEBUG_EXTENSION j2c_attribute_source_debug_extension_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeSourceDebugExtension, j2c_attribute_source_debug_extension, J2C, ATTRIBUTE_SOURCE_DEBUG_EXTENSION, GObject)
static gchar const *const J2C_SOURCE_DEBUG_EXTENSION = "SourceDebugExtension";
#define J2C_TYPE_ATTRIBUTE_BOOTSTRAP_METHODS j2c_attribute_bootstrap_methods_get_type ()
G_DECLARE_FINAL_TYPE (J2cAttributeBootstrapMethods, j2c_attribute_bootstrap_methods, J2C, ATTRIBUTE_BOOTSTRAP_METHODS, GObject)
static gchar const *const J2C_BOOTSTRAP_METHODS = "BootstrapMethods";

#define J2C_TYPE_INNER_CLASS_INFO j2c_inner_class_info_get_type()
G_DECLARE_FINAL_TYPE (J2cInnerClassInfo, j2c_inner_class_info, J2C, INNER_CLASS_INFO, GObject)

#define J2C_TYPE_BOOTSTRAP_METHOD_INFO j2c_bootstrap_method_info_get_type ()
G_DECLARE_FINAL_TYPE (J2cBootstrapMethodInfo, j2c_bootstrap_method_info, J2C, BOOTSTRAP_METHOD_INFO, GObject)



G_END_DECLS

#endif /* __ATTRIBUTES_CLASS_FILE_H__ */
