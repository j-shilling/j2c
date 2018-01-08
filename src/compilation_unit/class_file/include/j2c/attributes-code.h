#ifndef __ATTRIBUTES_CODE_H__
#define __ATTRIBUTES_CODE_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define J2C_TYPE_ATTRIBUTE_LINE_NUMBER_TABLE    j2c_attribute_line_number_table_get_type ()
G_DECLARE_FINAL_TYPE(J2cAttributeLineNumberTable, j2c_attribute_line_number_table, J2C, ATTRIBUTE_LINE_NUMBER_TABLE, GObject)
static gchar const *const J2C_LINE_NUMBER_TABLE = "LineNumberTable";
#define J2C_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TABLE j2c_attribute_local_variable_table_get_type ()
G_DECLARE_FINAL_TYPE(J2cAttributeLocalVariableTable, j2c_attribute_local_variable_table, J2C, ATTRIBUTE_LOCAL_VARIABLE_TABLE, GObject)
static gchar const *const J2C_LOCAL_VARIABLE_TABLE = "LocalVariableTable";
#define J2C_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE j2c_attribute_local_variable_type_table_get_type ()
G_DECLARE_FINAL_TYPE(J2cAttributeLocalVariableTypeTable, j2c_attribute_local_variable_type_table, J2C, ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE, GObject)
static gchar const *const J2C_LOCAL_VARIABLE_TYPE_TABLE = "LocalVariableTypeTable";
#define J2C_TYPE_ATTRIBUTE_STACK_MAP_TABLE  j2c_attribute_stack_map_table_get_type ()
G_DECLARE_FINAL_TYPE(J2cAttributeStackMapTable, j2c_attribute_stack_map_table, J2C, ATTRIBUTE_STACK_MAP_TABLE, GObject)
static gchar const *const J2C_STACK_MAP_TABLE = "StackMapTable";

#define J2C_TYPE_LINE_NUMBER    j2c_line_number_get_type ()
G_DECLARE_FINAL_TYPE(J2cLineNumber, j2c_line_number, J2C, LINE_NUMBER, GObject)
#define J2C_TYPE_LOCAL_VARIABLE j2c_local_variable_get_type ()
G_DECLARE_FINAL_TYPE(J2cLocalVariable, j2c_local_variable, J2C, LOCAL_VARIABLE, GObject)
#define J2C_TYPE_LOCAL_VARIABLE_TYPE j2c_local_variable_type_get_type ()
G_DECLARE_FINAL_TYPE(J2cLocalVariableType, j2c_local_variable_type, J2C, LOCAL_VARIABLE_TYPE, GObject)
#define J2C_TYPE_STACK_MAP_FRAME    j2c_stack_map_frame_get_type ()
G_DECLARE_FINAL_TYPE(J2cStackMapFrame, j2c_stack_map_frame, J2C, STACK_MAP_FRAME, GObject)
#define J2C_TYPE_VERIFICATION_TYPE_INFO j2c_verification_type_info_get_type ()
G_DECLARE_FINAL_TYPE(J2cVerificationTypeInfo, j2c_verification_type_info, J2C, VERIFICATION_TYPE_INFO, GObject)

G_END_DECLS

#endif // __ATTRIBUTES_CODE_H__
