#include <j2c/opcodes.h>

gint
j2c_opcode_operand_count (const J2cOpcode opcode)
{
  switch (opcode)
    {
      case j2c_aload:           return 1;
      case j2c_anewarray:       return 2;
      case j2c_astore:          return 1;
      case j2c_bipush:          return 1;
      case j2c_checkcast:       return 2;
      case j2c_dload:           return 1;
      case j2c_dstore:          return 1;
      case j2c_fload:           return 1;
      case j2c_fstore:          return 1;
      case j2c_getfield:        return 2;
      case j2c_getstatic:       return 2;
      case j2c_goto:            return 2;
      case j2c_goto_w:          return 4;
      case j2c_ifacmpeq:        return 2;
      case j2c_ifacmpne:        return 2;
      case j2c_ificmpeq:        return 2;
      case j2c_ificmpne:        return 2;
      case j2c_ificmpge:        return 2;
      case j2c_ificmpgt:        return 2;
      case j2c_ificmple:        return 2;
      case j2c_ificmplt:        return 2;
      case j2c_ifeq:            return 2;
      case j2c_ifne:            return 2;
      case j2c_ifge:            return 2;
      case j2c_ifgt:            return 2;
      case j2c_ifle:            return 2;
      case j2c_iflt:            return 2;
      case j2c_ifnonnull:       return 2;
      case j2c_ifnull:          return 2;
      case j2c_iinc:            return 2;
      case j2c_iload:           return 1;
      case j2c_instanceof:      return 2;
      case j2c_invokedynamic:   return 4;
      case j2c_invokeinterface: return 4;
      case j2c_invokespecial:   return 2;
      case j2c_invokestatic:    return 2;
      case j2c_invokevirtual:   return 2;
      case j2c_istore:          return 1;
      case j2c_jsr:             return 2;
      case j2c_jsr_w:           return 4;
      case j2c_ldc:             return 1;
      case j2c_ldc_w:           return 2;
      case j2c_ldc2_w:          return 2;
      case j2c_lload:           return 1;
      case j2c_lookupswitch:    return -1; // signal var-length instruction
      case j2c_lstore:          return 1;
      case j2c_multianewarray:  return 3;
      case j2c_new:             return 2;
      case j2c_newarray:        return 1;
      case j2c_putfield:        return 2;
      case j2c_putstatic:       return 2;
      case j2c_ret:             return 1;
      case j2c_sipush:          return 2;
      case j2c_tableswitch:     return -1; // signal var-length instruction
      case j2c_wide:            return -1; // signal var-length instruction
      default:                  return 0;
    }
}

guint16
j2c_operand_to_index16 (const guint8 byte1, const guint8 byte2)
{
  return (guint16) ((byte1 << 8) | byte2);
}
