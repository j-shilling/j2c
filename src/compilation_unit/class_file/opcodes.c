#include <j2c/opcodes.h>

static GEnumValue opcode_values[] =
{
  { 0x32, "j2c_aaload", "aaload" },
  { 0x53, "j2c_aastore", "aastore" },
  { 0x01, "j2c_aconst_null", "aconst_null" },
  { 0x19, "j2c_aload", "aload" },
  { 0x2A, "j2c_aload_0", "aload_0" },
  { 0x2B, "j2c_aload_1", "aload_1" },
  { 0x2C, "j2c_aload_2", "aload_2" },
  { 0x2D, "j2c_aload_3", "aload_3" },
  { 0xBD, "j2c_anewarray", "anewarray" },
  { 0xB0, "j2c_areturn", "areturn" },
  { 0xBE, "j2c_arraylength", "arraylength" },
  { 0x3A, "j2c_astore", "astore" },
  { 0x4B, "j2c_astore_0", "astore_0" },
  { 0x4C, "j2c_astore_1", "astore_1" },
  { 0x4D, "j2c_astore_2", "astore_2" },
  { 0x4E, "j2c_astore_3", "astore_3" },
  { 0xBF, "j2c_athrow", "athrow" },
  { 0x33, "j2c_baload", "baload" },
  { 0x54, "j2c_bastore", "bastore" },
  { 0x10, "j2c_bipush", "bipush" },
  { 0x34, "j2c_caload", "caload" },
  { 0x55, "j2c_castore", "castore" },
  { 0xC0, "j2c_checkcast", "checkcast" },
  { 0x90, "j2c_d2f", "d2f" },
  { 0x8E, "j2c_d2i", "d2i" },
  { 0x8F, "j2c_d2l", "d2l" },
  { 0x63, "j2c_dadd", "dadd" },
  { 0x31, "j2c_daload", "daload" },
  { 0x52, "j2c_dastore", "dastore" },
  { 0x98, "j2c_dcmpg", "dcmpg" },
  { 0x97, "j2c_dcmpl", "dcmpl" },
  { 0x0E, "j2c_dconst_0", "dconst_0" },
  { 0x0F, "j2c_dconst_1", "dconst_1" },
  { 0x6F, "j2c_ddiv", "ddiv" },
  { 0x18, "j2c_dload", "dload" },
  { 0x26, "j2c_dload_0", "dload_0" },
  { 0x27, "j2c_dload_1", "dload_1" },
  { 0x28, "j2c_dload_2", "dload_2" },
  { 0x29, "j2c_dload_3", "dload_3" },
  { 0x6B, "j2c_dmul", "dmul" },
  { 0x77, "j2c_dneg", "dneg" },
  { 0x73, "j2c_drem", "drem" },
  { 0xAF, "j2c_dreturn", "dreturn" },
  { 0x39, "j2c_dstore", "dstore" },
  { 0x47, "j2c_dstore_0", "dstore_0" },
  { 0x48, "j2c_dstore_1", "dstore_1" },
  { 0x49, "j2c_dstore_2", "dstore_2" },
  { 0x4A, "j2c_dstore_3", "dstore_3" },
  { 0x67, "j2c_dsub", "dsub" },
  { 0x59, "j2c_dup", "dup" },
  { 0x5A, "j2c_dup_x1", "dup_x1" },
  { 0x5B, "j2c_dup_x2", "dup_x2" },
  { 0x5C, "j2c_dup2", "dup2" },
  { 0x5D, "j2c_dup2_x1", "dup2_x1" },
  { 0x5E, "j2c_dup2_x2", "dup2_x2" },
  { 0x8D, "j2c_f2d", "f2d" },
  { 0x8B, "j2c_f2i", "f2i" },
  { 0x8C, "j2c_f2l", "f2l" },
  { 0x62, "j2c_fadd", "fadd" },
  { 0x30, "j2c_faload", "faload" },
  { 0x51, "j2c_fastore", "fastore" },
  { 0x96, "j2c_fcmpg", "fcmpg" },
  { 0x95, "j2c_fcmpl", "fcmpl" },
  { 0x0B, "j2c_fconst_0", "fconst_0" },
  { 0x0C, "j2c_fconst_1", "fconst_1" },
  { 0x0D, "j2c_fconst_2", "fconst_2" },
  { 0x6E, "j2c_fdiv", "fdiv" },
  { 0x17, "j2c_fload", "fload" },
  { 0x22, "j2c_fload_0", "fload_0" },
  { 0x23, "j2c_fload_1", "fload_1" },
  { 0x24, "j2c_fload_2", "fload_2" },
  { 0x25, "j2c_fload_3", "fload_3" },
  { 0x6A, "j2c_fmul", "fmul" },
  { 0x76, "j2c_fneg", "fneg" },
  { 0x72, "j2c_frem", "frem" },
  { 0xAE, "j2c_freturn", "freturn" },
  { 0x38, "j2c_fstore", "fstore" },
  { 0x43, "j2c_fstore_0", "fstore_0" },
  { 0x44, "j2c_fstore_1", "fstore_1" },
  { 0x45, "j2c_fstore_2", "fstore_2" },
  { 0x46, "j2c_fstore_3", "fstore_3" },
  { 0x66, "j2c_fsub", "fsub" },
  { 0xB4, "j2c_getfield", "getfield" },
  { 0xB2, "j2c_getstatic", "getstatic" },
  { 0xA7, "j2c_goto", "goto" },
  { 0xC8, "j2c_goto_w", "goto_w" },
  { 0x91, "j2c_i2b", "i2b" },
  { 0x92, "j2c_i2c", "i2c" },
  { 0x87, "j2c_i2d", "i2d" },
  { 0x86, "j2c_i2f", "i2f" },
  { 0x85, "j2c_i2l", "i2l" },
  { 0x93, "j2c_i2s", "i2s" },
  { 0x60, "j2c_iadd", "iadd" },
  { 0x2E, "j2c_iaload", "iaload" },
  { 0x7E, "j2c_iand", "iand" },
  { 0x4F, "j2c_iastore", "iastore" },
  { 0x02, "j2c_iconst_m1", "iconst_m1" },
  { 0x03, "j2c_iconst_0", "iconst_0" },
  { 0x04, "j2c_iconst_1", "iconst_1" },
  { 0x05, "j2c_iconst_2", "iconst_2" },
  { 0x06, "j2c_iconst_3", "iconst_3" },
  { 0x07, "j2c_iconst_4", "iconst_4" },
  { 0x08, "j2c_iconst_5", "iconst_5" },
  { 0x6C, "j2c_idiv", "idiv" },
  { 0xA5, "j2c_ifacmpeq", "ifacmpeq" },
  { 0xA6, "j2c_ifacmpne", "ifacmpne" },
  { 0x9F, "j2c_ificmpeq", "ificmpeq" },
  { 0xA0, "j2c_ificmpne", "ificmpne" },
  { 0xA1, "j2c_ificmplt", "ificmplt" },
  { 0xA2, "j2c_ificmpge", "ificmpge" },
  { 0xA3, "j2c_ificmpgt", "ificmpgt" },
  { 0xA4, "j2c_ificmple", "ificmple" },
  { 0x99, "j2c_ifeq", "ifeq" },
  { 0x9A, "j2c_ifne", "ifne" },
  { 0x9B, "j2c_iflt", "iflt" },
  { 0x9C, "j2c_ifge", "ifge" },
  { 0x9D, "j2c_ifgt", "ifgt" },
  { 0x9E, "j2c_ifle", "ifle" },
  { 0xC7, "j2c_ifnonnull", "ifnonnull" },
  { 0xC6, "j2c_ifnull", "ifnull" },
  { 0x84, "j2c_iinc", "iinc" },
  { 0x15, "j2c_iload", "iload" },
  { 0x1A, "j2c_iload_0", "iload_0" },
  { 0x1B, "j2c_iload_1", "iload_1" },
  { 0x1C, "j2c_iload_2", "iload_2" },
  { 0x1D, "j2c_iload_3", "iload_3" },
  { 0x68, "j2c_imul", "imul" },
  { 0x74, "j2c_ineg", "ineg" },
  { 0xC1, "j2c_instanceof", "instanceof" },
  { 0xBA, "j2c_invokedynamic", "invokedynamic" },
  { 0xB9, "j2c_invokeinterface", "invokeinterface" },
  { 0xB7, "j2c_invokespecial", "invokespecial" },
  { 0xB8, "j2c_invokestatic", "invokestatic" },
  { 0xB6, "j2c_invokevirtual", "invokevirtual" },
  { 0x80, "j2c_ior", "ior" },
  { 0x70, "j2c_irem", "irem" },
  { 0xAC, "j2c_ireturn", "ireturn" },
  { 0x78, "j2c_ishl", "ishl" },
  { 0x7A, "j2c_ishr", "ishr" },
  { 0x36, "j2c_istore", "istore" },
  { 0x3B, "j2c_istore_0", "istore_0" },
  { 0x3C, "j2c_istore_1", "istore_1" },
  { 0x3D, "j2c_istore_2", "istore_2" },
  { 0x3E, "j2c_istore_3", "istore_3" },
  { 0x64, "j2c_isub", "isub" },
  { 0x7C, "j2c_iushr", "iushr" },
  { 0x82, "j2c_ixor", "ixor" },
  { 0xA8, "j2c_jsr", "jsr" },
  { 0xC9, "j2c_jsr_w", "jsr_w" },
  { 0x8A, "j2c_l2d", "l2d" },
  { 0x89, "j2c_l2f", "l2f" },
  { 0x88, "j2c_l2i", "l2i" },
  { 0x61, "j2c_ladd", "ladd" },
  { 0x2F, "j2c_laload", "laload" },
  { 0x7F, "j2c_land", "land" },
  { 0x50, "j2c_lastore", "lastore" },
  { 0x94, "j2c_lcmp", "lcmp" },
  { 0x09, "j2c_lconst_0", "lconst_0" },
  { 0x0A, "j2c_lconst_1", "lconst_1" },
  { 0x12, "j2c_ldc", "ldc" },
  { 0x13, "j2c_ldc_w", "ldc_w" },
  { 0x14, "j2c_ldc2_w", "ldc2_w" },
  { 0x6D, "j2c_ldiv", "ldiv" },
  { 0x16, "j2c_lload", "lload" },
  { 0x1E, "j2c_lload_0", "lload_0" },
  { 0x1F, "j2c_lload_1", "lload_1" },
  { 0x20, "j2c_lload_2", "lload_2" },
  { 0x21, "j2c_lload_3", "lload_3" },
  { 0x69, "j2c_lmul", "lmul" },
  { 0x75, "j2c_lneg", "lneg" },
  { 0xAB, "j2c_lookupswitch", "lookupswitch" },
  { 0x81, "j2c_lor", "lor" },
  { 0x71, "j2c_lrem", "lrem" },
  { 0xAD, "j2c_lreturn", "lreturn" },
  { 0x79, "j2c_lshl", "lshl" },
  { 0x7B, "j2c_lshr", "lshr" },
  { 0x37, "j2c_lstore", "lstore" },
  { 0x3F, "j2c_lstore_0", "lstore_0" },
  { 0x40, "j2c_lstore_1", "lstore_1" },
  { 0x41, "j2c_lstore_2", "lstore_2" },
  { 0x42, "j2c_lstore_3", "lstore_3" },
  { 0x65, "j2c_lsub", "lsub" },
  { 0x7D, "j2c_lushr", "lushr" },
  { 0x83, "j2c_lxor", "lxor" },
  { 0xC2, "j2c_monitorenter", "monitorenter" },
  { 0xC3, "j2c_monitorexit", "monitorexit" },
  { 0xC5, "j2c_multianewarray", "multianewarray" },
  { 0xBB, "j2c_new", "new" },
  { 0xBC, "j2c_newarray", "newarray" },
  { 0x00, "j2c_nop", "nop" },
  { 0x57, "j2c_pop", "pop" },
  { 0x58, "j2c_pop2", "pop2" },
  { 0xB5, "j2c_putfield", "putfield" },
  { 0xB3, "j2c_putstatic", "putstatic" },
  { 0xA9, "j2c_ret", "ret" },
  { 0xB1, "j2c_return", "return" },
  { 0x35, "j2c_saload", "saload" },
  { 0x56, "j2c_sastore", "sastore" },
  { 0x11, "j2c_sipush", "sipush" },
  { 0x5F, "j2c_swap", "swap" },
  { 0xAA, "j2c_tableswitch", "tableswitch" },
  { 0xC4, "j2c_wide", "wide" },
  0
};

GType
j2c_opcode_get_type(void)
{
  static GType type = 0;
  if (type == 0)
    type = g_enum_register_static ("J2cOpcode", opcode_values);
  return type;
}

static gint
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

struct _J2cByteInstruction
{
  GObject parent;

  J2cOpcode opcode;
  GBytes *operands;
};

G_DEFINE_TYPE(J2cByteInstruction, j2c_byte_instruction, G_TYPE_OBJECT)

enum
{
  PROP_OPCODE,
  PROP_OPERANDS,
  N_PROPERTIES
};

GParamSpec *object_properties[N_PROPERTIES] = { NULL, };

static void
j2c_byte_instruction_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
  J2cByteInstruction *self = J2C_BYTE_INSTRUCTION (object);

  switch (property_id)
    {
      case PROP_OPCODE:
        self->opcode = g_value_get_enum (value);
        break;
      case PROP_OPERANDS:
        if (self->operands)
          g_bytes_unref (self->operands);
        self->operands = g_bytes_ref (g_value_get_boxed (value));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_byte_instruction_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
  J2cByteInstruction *self = J2C_BYTE_INSTRUCTION (object);

  switch (property_id)
    {
      case PROP_OPCODE:
        g_value_set_enum (value, self->opcode);
        break;
      case PROP_OPERANDS:
        g_value_set_boxed (value, g_bytes_ref (self->operands));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
j2c_byte_instruction_dispose (GObject *object)
{
  J2cByteInstruction *self = J2C_BYTE_INSTRUCTION (object);

  if (self->operands)
    {
      g_bytes_unref (self->operands);
      self->operands = NULL;
    }

  G_OBJECT_CLASS (j2c_byte_instruction_parent_class)->dispose (object);
}

static void
j2c_byte_instruction_class_init (J2cByteInstructionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->set_property = j2c_byte_instruction_set_property;
  object_class->get_property = j2c_byte_instruction_get_property;
  object_class->dispose      = j2c_byte_instruction_dispose;

  object_properties[PROP_OPCODE] =
    g_param_spec_enum ("opcode",
                       "opcode",
                       "opcode",
                       J2C_TYPE_OPCODE,
                       j2c_nop,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  object_properties[PROP_OPERANDS] =
    g_param_spec_boxed ("operands",
                        "operands",
                        "operands",
                        G_TYPE_BYTE_ARRAY,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  g_object_class_install_properties (object_class, N_PROPERTIES, object_properties);
}

static void
j2c_byte_instruction_init(J2cByteInstruction *self)
{
  self->operands = NULL;
}

J2cOpcode
j2c_byte_instruction_opcode (J2cByteInstruction *self)
{
  g_return_val_if_fail (self != NULL, j2c_nop);
  return self->opcode;
}

guint8
j2c_byte_instruction_operand (J2cByteInstruction *self, const guint index)
{
  g_return_val_if_fail (self != NULL, 0);
  g_return_val_if_fail (self->operands != NULL, 0);
  g_return_val_if_fail (index < g_bytes_get_size (self->operands), 0);

  guint8 const *const data = g_bytes_get_data (self->operands, NULL);
  return data[index];
}

gsize
j2c_byte_instruction_operand_size (J2cByteInstruction *self)
{
  g_return_val_if_fail (self != NULL, 0);
  if (self->operands == NULL)
    return 0;
  else
    return g_bytes_get_size (self->operands);
}

J2cByteInstruction *
j2c_byte_instruction_new_from_stream (GInputStream *in, GError **error)
{
  g_return_val_if_fail (in != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  GError *tmp_error = NULL;

  /* Read first byte as opcode */
  guint8 opcode;
  g_input_stream_read (in, &opcode, 1, NULL, &tmp_error);
  if (tmp_error)
    {
      g_propagate_error (error, tmp_error);
      return NULL;
    }

  /* Validate opcode */
  GEnumClass *enum_class = g_type_class_ref (J2C_TYPE_OPCODE);
  GEnumValue *enum_value = g_enum_get_value (enum_class, opcode);
  g_type_class_unref (enum_class);
  if (enum_value == NULL)
    {
      g_set_error (error,
                   J2C_OPCODE_ERROR,
                   J2C_INVALID_OPCODE_ERROR,
                   "%x is not a valid opcode.",
                   opcode);
      return NULL;
    }

  /* Get operands */
  guint8 *operands = NULL;
  gsize size = 0;
  goffset offset;
  guint8 *defaultbytes = NULL;
  switch (opcode)
    {
      case j2c_lookupswitch:
         /* Skip padding */
        if (!g_seekable_can_seek (G_SEEKABLE (in)))
          {
            g_set_error (error,
                         J2C_OPCODE_ERROR,
                         J2C_STREAM_TYPE_ERROR,
                         "Cannot read lookupswitch instruction from a non-seekable stream.");
            return NULL;
          }
        offset = g_seekable_tell (G_SEEKABLE (in));
        g_input_stream_skip (in, offset % 4, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (operands);
            g_propagate_error (error, tmp_error);
            return NULL;
          }

        /* Determine operand size */
        defaultbytes = g_malloc (4);
        g_input_stream_read (in, defaultbytes, 4, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (defaultbytes);
            g_propagate_error (error, tmp_error);
            return NULL;
          }

        guint8 *npairsbytes = g_malloc (4);
        g_input_stream_read (in, npairsbytes, 4, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (defaultbytes);
            g_free (npairsbytes);
            g_propagate_error (error, tmp_error);
            return NULL;
          }
        gint32 npairs = (npairsbytes[0] << 24) | (npairsbytes[1] << 16) | (npairsbytes[2] << 8) | npairsbytes[3];

        size = 4 + 4 + (npairs * 2 * 4);
        operands = g_malloc (size);

        operands[0] = defaultbytes[0];
        operands[1] = defaultbytes[1];
        operands[2] = defaultbytes[2];
        operands[3] = defaultbytes[3];
        g_free (defaultbytes);
        operands[4] = npairsbytes[0];
        operands[5] = npairsbytes[1];
        operands[6] = npairsbytes[2];
        operands[7] = npairsbytes[3];
        g_free (npairsbytes);

        g_input_stream_read (in, operands + 8, npairs * 2 * 4, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (operands);
            g_propagate_error (error, tmp_error);
            return NULL;
          }
        break;
      case j2c_tableswitch:
        /* Skip padding */
        if (!g_seekable_can_seek (G_SEEKABLE (in)))
          {
            g_set_error (error,
                         J2C_OPCODE_ERROR,
                         J2C_STREAM_TYPE_ERROR,
                         "Cannot read typeswitch instruction from a non-seekable stream.");
            return NULL;
          }
        offset = g_seekable_tell (G_SEEKABLE (in));
        g_input_stream_skip (in, offset % 4, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (operands);
            g_propagate_error (error, tmp_error);
            return NULL;
          }

        /* Determine operand size */
        defaultbytes = g_malloc (4);
        g_input_stream_read (in, defaultbytes, 4, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (defaultbytes);
            g_propagate_error (error, tmp_error);
            return NULL;
          }

        guint8 *lowbytes = g_malloc (4);
        g_input_stream_read (in, lowbytes, 4, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (defaultbytes);
            g_free (lowbytes);
            g_propagate_error (error, tmp_error);
            return NULL;
          }
        gint32 low = (lowbytes[0] << 24) | (lowbytes[1] << 16) | (lowbytes[2] << 8) | lowbytes[3];

        guint8 *highbytes = g_malloc (4);
        g_input_stream_read (in, highbytes, 4, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (defaultbytes);
            g_free (lowbytes);
            g_free (highbytes);
            g_propagate_error (error, tmp_error);
            return NULL;
          }
        gint32 high = (highbytes[0] << 24) | (highbytes[1] << 16) | (highbytes[2] << 8) | highbytes[3];

        guint offsets = high - low + 1;
        size = 4 + 4 + 4 + (offsets * 4);
        operands = g_malloc (size);

        operands[0] = defaultbytes[0];
        operands[1] = defaultbytes[1];
        operands[2] = defaultbytes[2];
        operands[3] = defaultbytes[3];
        g_free (defaultbytes);
        operands[4] = lowbytes[0];
        operands[5] = lowbytes[1];
        operands[6] = lowbytes[2];
        operands[7] = lowbytes[3];
        g_free (lowbytes);
        operands[8]  = highbytes[0];
        operands[9]  = highbytes[1];
        operands[10] = highbytes[2];
        operands[11] = highbytes[3];
        g_free (highbytes);

        g_input_stream_read (in, operands + 12, offsets * 4, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (operands);
            g_propagate_error (error, tmp_error);
            return NULL;
          }
        break;
      case j2c_wide:
        size = 5;
        operands = g_malloc (size);
        g_input_stream_read (in, operands, 3, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (operands);
            g_propagate_error (error, tmp_error);
            return NULL;
          }
        if (operands[0] == j2c_iinc)
          {
            g_input_stream_read (in, operands + 3, 2, NULL, &tmp_error);
            if (tmp_error)
              {
                g_free (operands);
                g_propagate_error (error, tmp_error);
                return NULL;
              }
          }
        else
          {
            size = 3;
          }
        break;
      default:
        size = j2c_opcode_operand_count (opcode);
        if (size == 0)
          break;

        operands = g_malloc (size);
        g_input_stream_read (in, operands, size, NULL, &tmp_error);
        if (tmp_error)
          {
            g_free (operands);
            g_propagate_error (error, tmp_error);
            return NULL;
          }
        break;
    }

  /* Build object */
  if (size == 0)
    return g_object_new (J2C_TYPE_BYTE_INSTRUCTION,
                         "opcode", opcode,
                         NULL);

  GBytes *bytes = g_bytes_new (operands, size);
  J2cByteInstruction *ret = g_object_new (J2C_TYPE_BYTE_INSTRUCTION,
                                          "opcode", opcode,
                                          "operands", bytes,
                                          NULL);
  g_bytes_unref (bytes);
  return ret;
}

