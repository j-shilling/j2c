#include <j2c/opcodes.h>

guint
j2c_opcode_operand_count (const J2cOpcode opcode)
{
  switch (opcode)
    {
      case j2c_aload:       return 1;
      case j2c_anewarray:   return 2;
      case j2c_astore:      return 1;
      case j2c_bipush:      return 1;
      case j2c_checkcast:   return 2;
      case j2c_dload:       return 1;
      case j2c_dstore:      return 1;
      case j2c_fload:       return 1;
      case j2c_fstore:      return 1;
      case j2c_getfield:    return 2;
      case j2c_getstatic:   return 2;
      case j2c_goto:        return 2;
      case j2c_goto_w:      return 4;
      default:              return 0;
    }
}

guint16
j2c_operand_to_index16 (const guint8 byte1, const guint8 byte2)
{
  return (guint16) ((byte1 << 8) | byte2);
}
