#ifndef __OPCODES_H__
#define __OPCODES_H__

#include <glib.h>

typedef enum
{
  j2c_aaload        = 0x32, /* Load reference from array | ..., arrayref, index -> value */
  j2c_aastore       = 0x53, /* Store into reference array | ..., arrayref, index, value -> */
  j2c_aconst_null   = 0x01, /* Push null | ... -> null */
  j2c_aload         = 0x19, /* Load reference from local variable | aload <index> | ... -> objectref */
  j2c_aload_0       = 0x2A, /* aload 0 */
  j2c_aload_1       = 0x2B, /* aload 1 */
  j2c_aload_2       = 0x2C, /* aload 2 */
  j2c_aload_3       = 0x2D, /* aload 3 */
  j2c_anewarray     = 0xBD, /* Create new array of reference | anewarray indexbyte1 indexbyte2 | ..., count -> arrayref */
  j2c_areturn       = 0xB0, /* Return reference from method | ..., objectref -> */
  j2c_arraylength   = 0xBE, /* Get length of array | ..., arrayref -> value */
  j2c_astore        = 0x3A, /* Store reference in local variable | astore index | ...,objectref -> */
  j2c_astore_0      = 0x4B, /* astore 0 */
  j2c_astore_1      = 0x4C, /* astore 1 */
  j2c_astore_2      = 0x4D, /* astore 2 */
  j2c_astore_3      = 0x4E, /* astore 3 */
  j2c_athrow        = 0xBF, /* Throw exception or error | ..., objectref -> */
  j2c_baload        = 0x33, /* Load byte or boolean from array | ..., arrayref, index -> value */
  j2c_bastore       = 0x54, /* Store into byte or boolean array | ..., arrayref, index, value -> */
  j2c_bipush        = 0x10, /* Push byte | bipush byte | ... -> value */
  j2c_caload        = 0x34, /* Load char from array | ...,arrayref, index -> value */
  j2c_castore       = 0x55, /* Store into char array | ..., arrayref, index, value -> */
  j2c_checkcast     = 0xC0, /* Check whether an object is a given type | checkcast indexbyte1 indexbyte 2 | ..., objectref -> objectref */
  j2c_d2f           = 0x90, /* Convert double to float | ..., value -> result */
  j2c_d2i           = 0x8E, /* Convert double to int | ..., value -> result */
  j2c_d2l           = 0x8F, /* Convert double to long | ..., value -> result */
  j2c_dadd          = 0x63, /* Add double | ..., value, value -> result */
  j2c_daload        = 0x31, /* Load double from array | ..., arrayref, index -> value */
  j2c_dastore       = 0x52, /* Store into double array | ..., arrayref, index, value -> */
  j2c_dcmpg         = 0x98, /* Compare double | ..., value1, value2 -> result */
  j2c_dcmpl         = 0x97, /* Compare double | ..., value1, value2 -> result */
  j2c_dconst_0      = 0x0E, /* Push double constant 0.0 */
  j2c_dconst_1      = 0x0F, /* Push double constant 1.0 */
  j2c_ddiv          = 0x6F, /* Divide double | ..., value1, value 2 -> result */
  j2c_dload         = 0x18, /* Load double from local variable | dload index | ... -> value */
  j2c_dload_0       = 0x26, /* dload 0 */
  j2c_dload_1       = 0x27, /* dload 1 */
  j2c_dload_2       = 0x28, /* dload 2 */
  j2c_dload_3       = 0x29, /* dload 3 */
  j2c_dmul          = 0x6B, /* Multiply doubles | ..., value1, value2 -> result */
  j2c_dneg          = 0x77, /* Negate double | ..., value -> result */
  j2c_drem          = 0x73, /* Remainder double | ..., value1, value2 -> result */
  j2c_dreturn       = 0xAF, /* Return double | ..., value -> */
  j2c_dstore        = 0x39, /* Store double into local variable | dstore index | ...,value-> */
  j2c_dstore_0      = 0x47, /* dstore 0 */
  j2c_dstore_1      = 0x48, /* dstore 1 */
  j2c_dstore_2      = 0x49, /* dstore 2 */
  j2c_dstore_3      = 0x4A, /* dstore 3 */
  j2c_dsub          = 0x67, /* Subtract double | ..., value1, value2 -> result */
  j2c_dup           = 0x59, /* Duplicate top of stack | ..., value-> value, value */
  j2c_dup_x1        = 0x5A, /* Duplicate top of stack and insert it two values down | ..., value2, value1 -> value1, value2, value1 */
  j2c_dup_x2        = 0x5B, /* Duplicate top of stack and insert it three values down | ..., value3, value2, value1 -> value1, value3, value2, value1 */
  j2c_dup2          = 0x5C, /* Duplicate top two values of the stack | ..., x, y -> ..., x, y, x, y */
  j2c_dup2_x1       = 0x5D, /* Duplicate top two values of the stack and insert two values down | ..., x, y, z -> ..., y, z, x, y, z */
  j2c_dup2_x2       = 0x5E, /* Duplicate to ptwo values of the stack and insert three values down | ..., w, x, y, z -> ..., y, z, w, x, y, z */
  j2c_f2d           = 0x8D, /* cast float to double */
  j2c_f2i           = 0x8B, /* cast float to int */
  j2c_f2l           = 0x8C, /* cast float to long */
  j2c_fadd          = 0x62, /* add float | ..., x, y -> result */
  j2c_faload        = 0x30, /* Load float from array | ..., arrayref, index -> value */
  j2c_fastore       = 0x51, /* Store into float array | ..., arrayref, index, value -> */
  j2c_fcmpg         = 0x96, /* Compare float | ..., x, y -> result */
  j2c_fcmpl         = 0x95, /* Compare float | ..., x, y -> result */
  j2c_fconst_0      = 0x0B, /* push float */
  j2c_fconst_1      = 0x0C, /* push float */
  j2c_fconst_2      = 0x0D, /* push float */
  j2c_fdiv          = 0x6E, /* divide float | ..., x, y -> result */
  j2c_fload         = 0x17, /* load float from local variable | fload index | ..., -> result */
  j2c_fload_0       = 0x22, /* fload 0 */
  j2c_fload_1       = 0x23, /* fload 1 */
  j2c_fload_2       = 0x24, /* fload 2 */
  j2c_fload_3       = 0x25, /* fload 3 */
  j2c_fmul          = 0x6A, /* multiply float | ..., x, y -> result */
  j2c_fneg          = 0x76, /* negate float | ..., x -> result */
  j2c_frem          = 0x72, /* remainder float | ..., x, y -> result */
  j2c_freturn       = 0xAE, /* return float value */
  j2c_fstore        = 0x38, /* Store float to local variable | fstore index | ..., x -> */
  j2c_fstore_0      = 0x43, /* fstore 0 */
  j2c_fstore_1      = 0x44, /* fstore 1 */
  j2c_fstore_2      = 0x45, /* fstore 2 */
  j2c_fstore_3      = 0x46, /* fstore 3 */
  j2c_fsub          = 0x66, /* subtract float | .., x, y -> result */
  j2c_getfield      = 0xB4, /* get field from object | getfield byte1 byte2 | ..., objectref -> value */
  j2c_getstatic     = 0xB2, /* get static field from class | getstatic byte1 byte2 | ..., -> value */
  j2c_goto          = 0xA7, /* branch always | goto byte1 byte2 */
  j2c_goto_w        = 0xC8, /* branch always wide index | goto byte1 byte2 byte3 byte4 */
  j2c_i2b           = 0x91, /* cast int to byte */
  j2c_i2c           = 0x92, /* cast int to char */
  j2c_i2d           = 0x87, /* cast int to double */
  j2c_i2f           = 0x86, /* cast int to float */
  j2c_i2l           = 0x85, /* cast int to long */
  j2c_i2s           = 0x93, /* cast int to short */
  j2c_iadd          = 0x60, /* int add */
  j2c_iaload        = 0x2E, /* load int from array | ..., arrayref, index -> value */
  j2c_iand          = 0x7E, /* int & int | ..., x, y -> result */
  j2c_iastore       = 0x4F, /* store into int array | ..., arrayref, index, value -> */
  j2c_iconst_m1     = 0x02, /* push -1 */
  j2c_iconst_0      = 0x03, /* push 0 */
  j2c_iconst_1      = 0x04, /* push 1 */
  j2c_iconst_2      = 0x05, /* push 2 */
  j2c_iconst_3      = 0x06, /* push 3 */
  j2c_iconst_4      = 0x07, /* push 4 */
  j2c_iconst_5      = 0x08, /* push 5 */
} J2cOpcode;

guint j2c_opcode_operand_count (const J2cOpcode opcode);
guint16 j2c_operand_to_index16 (const guint8 byte1, const guint8 byte2);

#endif // __OPCODES_H__
