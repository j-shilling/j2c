#ifndef __OPCODES_H__
#define __OPCODES_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define J2C_OPCODE_ERROR	g_quark_from_static_string ("j2c-opcode")
#define J2C_TYPE_OPCODE     j2c_opcode_get_type ()
#define J2C_TYPE_BYTE_INSTRUCTION    j2c_byte_instruction_get_type ()
G_DECLARE_FINAL_TYPE(J2cByteInstruction, j2c_byte_instruction, J2C, BYTE_INSTRUCTION, GObject)

typedef enum
{
  J2C_INVALID_OPCODE_ERROR,
  J2C_STREAM_TYPE_ERROR
} J2cOpcodeError;

typedef enum
{
  j2c_aaload            = 0x32, /* Load reference from array | ..., arrayref, index -> value */
  j2c_aastore           = 0x53, /* Store into reference array | ..., arrayref, index, value -> */
  j2c_aconst_null       = 0x01, /* Push null | ... -> null */
  j2c_aload             = 0x19, /* Load reference from local variable | aload <index> | ... -> objectref */
  j2c_aload_0           = 0x2A, /* aload 0 */
  j2c_aload_1           = 0x2B, /* aload 1 */
  j2c_aload_2           = 0x2C, /* aload 2 */
  j2c_aload_3           = 0x2D, /* aload 3 */
  j2c_anewarray         = 0xBD, /* Create new array of reference | anewarray indexbyte1 indexbyte2 | ..., count -> arrayref */
  j2c_areturn           = 0xB0, /* Return reference from method | ..., objectref -> */
  j2c_arraylength       = 0xBE, /* Get length of array | ..., arrayref -> value */
  j2c_astore            = 0x3A, /* Store reference in local variable | astore index | ...,objectref -> */
  j2c_astore_0          = 0x4B, /* astore 0 */
  j2c_astore_1          = 0x4C, /* astore 1 */
  j2c_astore_2          = 0x4D, /* astore 2 */
  j2c_astore_3          = 0x4E, /* astore 3 */
  j2c_athrow            = 0xBF, /* Throw exception or error | ..., objectref -> */
  j2c_baload            = 0x33, /* Load byte or boolean from array | ..., arrayref, index -> value */
  j2c_bastore           = 0x54, /* Store into byte or boolean array | ..., arrayref, index, value -> */
  j2c_bipush            = 0x10, /* Push byte | bipush byte | ... -> value */
  j2c_caload            = 0x34, /* Load char from array | ...,arrayref, index -> value */
  j2c_castore           = 0x55, /* Store into char array | ..., arrayref, index, value -> */
  j2c_checkcast         = 0xC0, /* Check whether an object is a given type | checkcast indexbyte1 indexbyte 2 | ..., objectref -> objectref */
  j2c_d2f               = 0x90, /* Convert double to float | ..., value -> result */
  j2c_d2i               = 0x8E, /* Convert double to int | ..., value -> result */
  j2c_d2l               = 0x8F, /* Convert double to long | ..., value -> result */
  j2c_dadd              = 0x63, /* Add double | ..., value, value -> result */
  j2c_daload            = 0x31, /* Load double from array | ..., arrayref, index -> value */
  j2c_dastore           = 0x52, /* Store into double array | ..., arrayref, index, value -> */
  j2c_dcmpg             = 0x98, /* Compare double | ..., value1, value2 -> result */
  j2c_dcmpl             = 0x97, /* Compare double | ..., value1, value2 -> result */
  j2c_dconst_0          = 0x0E, /* Push double constant 0.0 */
  j2c_dconst_1          = 0x0F, /* Push double constant 1.0 */
  j2c_ddiv              = 0x6F, /* Divide double | ..., value1, value 2 -> result */
  j2c_dload             = 0x18, /* Load double from local variable | dload index | ... -> value */
  j2c_dload_0           = 0x26, /* dload 0 */
  j2c_dload_1           = 0x27, /* dload 1 */
  j2c_dload_2           = 0x28, /* dload 2 */
  j2c_dload_3           = 0x29, /* dload 3 */
  j2c_dmul              = 0x6B, /* Multiply doubles | ..., value1, value2 -> result */
  j2c_dneg              = 0x77, /* Negate double | ..., value -> result */
  j2c_drem              = 0x73, /* Remainder double | ..., value1, value2 -> result */
  j2c_dreturn           = 0xAF, /* Return double | ..., value -> */
  j2c_dstore            = 0x39, /* Store double into local variable | dstore index | ...,value-> */
  j2c_dstore_0          = 0x47, /* dstore 0 */
  j2c_dstore_1          = 0x48, /* dstore 1 */
  j2c_dstore_2          = 0x49, /* dstore 2 */
  j2c_dstore_3          = 0x4A, /* dstore 3 */
  j2c_dsub              = 0x67, /* Subtract double | ..., value1, value2 -> result */
  j2c_dup               = 0x59, /* Duplicate top of stack | ..., value-> value, value */
  j2c_dup_x1            = 0x5A, /* Duplicate top of stack and insert it two values down | ..., value2, value1 -> value1, value2, value1 */
  j2c_dup_x2            = 0x5B, /* Duplicate top of stack and insert it three values down | ..., value3, value2, value1 -> value1, value3, value2, value1 */
  j2c_dup2              = 0x5C, /* Duplicate top two values of the stack | ..., x, y -> ..., x, y, x, y */
  j2c_dup2_x1           = 0x5D, /* Duplicate top two values of the stack and insert two values down | ..., x, y, z -> ..., y, z, x, y, z */
  j2c_dup2_x2           = 0x5E, /* Duplicate to ptwo values of the stack and insert three values down | ..., w, x, y, z -> ..., y, z, w, x, y, z */
  j2c_f2d               = 0x8D, /* cast float to double */
  j2c_f2i               = 0x8B, /* cast float to int */
  j2c_f2l               = 0x8C, /* cast float to long */
  j2c_fadd              = 0x62, /* add float | ..., x, y -> result */
  j2c_faload            = 0x30, /* Load float from array | ..., arrayref, index -> value */
  j2c_fastore           = 0x51, /* Store into float array | ..., arrayref, index, value -> */
  j2c_fcmpg             = 0x96, /* Compare float | ..., x, y -> result */
  j2c_fcmpl             = 0x95, /* Compare float | ..., x, y -> result */
  j2c_fconst_0          = 0x0B, /* push float */
  j2c_fconst_1          = 0x0C, /* push float */
  j2c_fconst_2          = 0x0D, /* push float */
  j2c_fdiv              = 0x6E, /* divide float | ..., x, y -> result */
  j2c_fload             = 0x17, /* load float from local variable | fload index | ..., -> result */
  j2c_fload_0           = 0x22, /* fload 0 */
  j2c_fload_1           = 0x23, /* fload 1 */
  j2c_fload_2           = 0x24, /* fload 2 */
  j2c_fload_3           = 0x25, /* fload 3 */
  j2c_fmul              = 0x6A, /* multiply float | ..., x, y -> result */
  j2c_fneg              = 0x76, /* negate float | ..., x -> result */
  j2c_frem              = 0x72, /* remainder float | ..., x, y -> result */
  j2c_freturn           = 0xAE, /* return float value */
  j2c_fstore            = 0x38, /* Store float to local variable | fstore index | ..., x -> */
  j2c_fstore_0          = 0x43, /* fstore 0 */
  j2c_fstore_1          = 0x44, /* fstore 1 */
  j2c_fstore_2          = 0x45, /* fstore 2 */
  j2c_fstore_3          = 0x46, /* fstore 3 */
  j2c_fsub              = 0x66, /* subtract float | .., x, y -> result */
  j2c_getfield          = 0xB4, /* get field from object | getfield byte1 byte2 | ..., objectref -> value */
  j2c_getstatic         = 0xB2, /* get static field from class | getstatic byte1 byte2 | ..., -> value */
  j2c_goto              = 0xA7, /* branch always | goto byte1 byte2 */
  j2c_goto_w            = 0xC8, /* branch always wide index | goto byte1 byte2 byte3 byte4 */
  j2c_i2b               = 0x91, /* cast int to byte */
  j2c_i2c               = 0x92, /* cast int to char */
  j2c_i2d               = 0x87, /* cast int to double */
  j2c_i2f               = 0x86, /* cast int to float */
  j2c_i2l               = 0x85, /* cast int to long */
  j2c_i2s               = 0x93, /* cast int to short */
  j2c_iadd              = 0x60, /* int add */
  j2c_iaload            = 0x2E, /* load int from array | ..., arrayref, index -> value */
  j2c_iand              = 0x7E, /* int & int | ..., x, y -> result */
  j2c_iastore           = 0x4F, /* store into int array | ..., arrayref, index, value -> */
  j2c_iconst_m1         = 0x02, /* push -1 */
  j2c_iconst_0          = 0x03, /* push 0 */
  j2c_iconst_1          = 0x04, /* push 1 */
  j2c_iconst_2          = 0x05, /* push 2 */
  j2c_iconst_3          = 0x06, /* push 3 */
  j2c_iconst_4          = 0x07, /* push 4 */
  j2c_iconst_5          = 0x08, /* push 5 */
  j2c_idiv              = 0x6C, /* int divide */
  j2c_ifacmpeq          = 0xA5, /* Branch if ref equals | ifacmpeq byte1 byte2 | ..., x, y -> */
  j2c_ifacmpne          = 0xA6, /* Branch if ref not equals | ifacmpne byte1 byte2 | ..., x, y -> */
  j2c_ificmpeq          = 0x9F, /* Branch if int == int | ificmpeq byte1 byte2 | ..., x, y -> */
  j2c_ificmpne          = 0xA0, /* Branch if int =/= int | ificmpne byte1 byte2 | ..., x, y -> */
  j2c_ificmplt          = 0xA1, /* Branch if int < int | ificmplt byte1 byte2 | ..., x, y -> */
  j2c_ificmpge          = 0xA2, /* Branch if int >= int | ificmpge byte1 byte2 | ..., x, y -> */
  j2c_ificmpgt          = 0xA3, /* Branch if int > int | ificmpeq byte1 byte2 | ..., x, y -> */
  j2c_ificmple          = 0xA4, /* Branch if int <= int | ificmpeq byte1 byte2 | ..., x, y -> */
  j2c_ifeq              = 0x99, /* Branch if int == 0 | ifeq byte1 byte2 | ..., x, y -> */
  j2c_ifne              = 0x9A, /* Branch if int =/= 0 | ifne byte1 byte2 | ..., x, y -> */
  j2c_iflt              = 0x9B, /* Branch if int < 0 | iflt byte1 byte2 | ..., x, y -> */
  j2c_ifge              = 0x9C, /* Branch if int >= 0 | ifge byte1 byte2 | ..., x, y -> */
  j2c_ifgt              = 0x9D, /* Branch if int > 0 | ifeq byte1 byte2 | ..., x, y -> */
  j2c_ifle              = 0x9E, /* Branch if int <= 0, | ifeq byte1 byte2 | ..., x, y -> */
  j2c_ifnonnull         = 0xC7, /* Branch if ref != null | ifnonnull byte1 byte2 | ..., x -> */
  j2c_ifnull            = 0xC6, /* Branch if ref == null | ifnull byte1 byte2 | ..., x -> */
  j2c_iinc              = 0x84, /* Increment local variable by const | iinc index const */
  j2c_iload             = 0x15, /* load int from local variable | iload index | ... -> x */
  j2c_iload_0           = 0x1A, /* iload 0 */
  j2c_iload_1           = 0x1B, /* iload 1 */
  j2c_iload_2           = 0x1C, /* iload 2 */
  j2c_iload_3           = 0x1D, /* iload 3 */
  j2c_imul              = 0x68, /* multiply int | ..., x, y -> result */
  j2c_ineg              = 0x74, /* negate int | ..., x -> result */
  j2c_instanceof        = 0xC1, /* check if ref is instance of index | instanceof index1 index2 | ..., objectref -> result */
  j2c_invokedynamic     = 0xBA, /* invoke dynamic method | invokedynamic index1 index2 0 0 | ..., [arg1, [arg2 ...]] -> */
  j2c_invokeinterface   = 0xB9, /* invoke interface method | invokeinterface index1 index2 count 0 | .., ref [arg1, [arg2 ...]] -> */
  j2c_invokespecial     = 0xB7, /* invoke superclass, private, init method | invokespecial index1 index2 | .., ref [arg1, [arg2 ...]] -> */
  j2c_invokestatic      = 0xB8, /* invoke static method | invokestatic index1 index 2 | .., [arg1, [arg2 ...]] -> */
  j2c_invokevirtual     = 0xB6, /* invoke instance method | invokevirtual index1 index2 | .., ref [arg1, [arg2 ...]] -> */
  j2c_ior               = 0x80, /* int | int | ..., x, y -> result */
  j2c_irem              = 0x70, /* int remainder | ..., x, y -> result */
  j2c_ireturn           = 0xAC, /* return int */
  j2c_ishl              = 0x78, /* x << y | .., x, y -> result */
  j2c_ishr              = 0x7A, /* x >> y | ..., x, y -> result */
  j2c_istore            = 0x36, /* store int in local variable | istore index | ...., x -> */
  j2c_istore_0          = 0x3B, /* istore 0 */
  j2c_istore_1          = 0x3C, /* istore 1 */
  j2c_istore_2          = 0x3D, /* istore 2 */
  j2c_istore_3          = 0x3E, /* istore 3 */
  j2c_isub              = 0x64, /* int subtract | ..., x, y -> result */
  j2c_iushr             = 0x7C, /* x >> y w/ 0 ex. | ..., x, y - > result */
  j2c_ixor              = 0x82, /* x | (bitwise xor ) y | ...., x, y -> result */
  j2c_jsr               = 0xA8, /* jump subroutine | jsr byte1 byte2 | ... -> addr */
  j2c_jsr_w             = 0xC9, /* jump subroutine wide | jsr byte1 byte2 byte3 byte 4 | ... -> addr */
  j2c_l2d               = 0x8A, /* cast long to double */
  j2c_l2f               = 0x89, /* cast long to float */
  j2c_l2i               = 0x88, /* cast long to int */
  j2c_ladd              = 0x61, /* long add | ..., x, y -> result */
  j2c_laload            = 0x2F, /* load long from array | ..., arrayref, index -> value */
  j2c_land              = 0x7F, /* long & long | ..., x, y -> result */
  j2c_lastore           = 0x50, /* store into long array | ..., arrayref, index, value -> */
  j2c_lcmp              = 0x94, /* long compare | ..., x, y -> result */
  j2c_lconst_0          = 0x09, /* push (long) 0 */
  j2c_lconst_1          = 0x0A, /* push (long) 1 */
  j2c_ldc               = 0x12, /* load from constant pool | ldc index | ..., -> value */
  j2c_ldc_w             = 0x13, /* load from constant pool (wide) | ldc index1 index2 | ..., -> value */
  j2c_ldc2_w            = 0x14, /* load long or double from constant pool (wide) | ldc index1 index2 | ..., -> value */
  j2c_ldiv              = 0x6D, /* long divide | ..., x, y -> result */
  j2c_lload             = 0x16, /* load long from local variable | lload index | ..., -> value */
  j2c_lload_0           = 0x1E, /* lload 0 */
  j2c_lload_1           = 0x1F, /* lload 1 */
  j2c_lload_2           = 0x20, /* lload 2 */
  j2c_lload_3           = 0x21, /* lload 3 */
  j2c_lmul              = 0x69, /* long multiply | ..., x, y -> result */
  j2c_lneg              = 0x75, /* long negate | ..., x -> result */
  j2c_lookupswitch      = 0xAB, /* access jump table | padding guint32 default guint18 npairs ... pairs | ..., key-> */
  j2c_lor               = 0x81, /* bitwise inclusive or | ...x, y, -> result */
  j2c_lrem              = 0x71, /* remainder long | ..., x, y -> result */
  j2c_lreturn           = 0xAD, /* return long */
  j2c_lshl              = 0x79, /* long << long | ..., x, y -> result */
  j2c_lshr              = 0x7B, /* long >> long | .., x, y -> result */
  j2c_lstore            = 0x37, /* store long in local variable | lstore index | ... value -> */
  j2c_lstore_0          = 0x3F, /* lstore 0 */
  j2c_lstore_1          = 0x40, /* lstore 1 */
  j2c_lstore_2          = 0x41, /* lstore 2 */
  j2c_lstore_3          = 0x42, /* lstore 3 */
  j2c_lsub              = 0x65, /* long subtract | ..., x, y -> result */
  j2c_lushr             = 0x7D, /* long >> long 0-ex | ..., x, y -> result */
  j2c_lxor              = 0x83, /* bitwise exclusive or | ..., x, y -> result */
  j2c_monitorenter      = 0xC2, /* Enter monitor for object | ..., objectref -> */
  j2c_monitorexit       = 0xC3, /* Exit monitor for object | ..., objectref -> */
  j2c_multianewarray    = 0xC5, /* Create new multidemensional array | multianewarray index1 index2 dimentions | count1, [count 2, ...]] -> arrayref */
  j2c_new               = 0xBB, /* Create new object | new index1 index2 | ..., -> objectref */
  j2c_newarray          = 0xBC, /* Create new array | newarray J2cAType | ..., count -> arrayref */
  j2c_nop               = 0x00, /* Do nothing */
  j2c_pop               = 0x57, /* Pop top of the stack | ..., x -> */
  j2c_pop2              = 0x58, /* Pop two off the stack | ..., x, y, -> */
  j2c_putfield          = 0xB5, /* Set field in object | putfield index1 index2 | ..., objectref, value -> */
  j2c_putstatic         = 0xB3, /* Set static field in class | putstatic index1 index2 | ..., value -> */
  j2c_ret               = 0xA9, /* Return from subroutine | ret index */
  j2c_return            = 0xB1, /* Return void from method */
  j2c_saload            = 0x35, /* Load short from array | array ref, index -> value */
  j2c_sastore           = 0x56, /* Store into short array | arrayref, index, value -> */
  j2c_sipush            = 0x11, /* Push short | sipush byte1 byte2 | ..., -> value */
  j2c_swap              = 0x5F, /* Swap top two stack values | ..., x, y -> y, x */
  j2c_tableswitch       = 0xAA, /* Access jump tale by index and jump | tableswitch pad guint32 default guint32 lowbytes guint32 highbytes jump offsets | ..., index -> */
  j2c_wide              = 0xC4  /* Extend local variable index | wide <opcode> index1 index2 [const1 const2] */
} J2cOpcode;

typedef enum
{
  T_BOOLEAN = 4,
  T_CHAR    = 5,
  T_FLOAT   = 6,
  T_DOUBLE  = 7,
  T_BYTE    = 8,
  T_SHORT   = 9,
  T_INT     = 10,
  T_LONG    = 11
} J2cAType;

GType j2c_opcode_get_type (void);
J2cOpcode j2c_byte_instruction_opcode (J2cByteInstruction *self);
guint8 j2c_byte_instruction_operand (J2cByteInstruction *self, const guint index);
J2cByteInstruction *j2c_byte_instruction_new_from_stream (GInputStream *in, GError **error);

G_END_DECLS

#endif // __OPCODES_H__
