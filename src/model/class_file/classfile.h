#ifndef __CLASSFILE_H__
#define __CLASSFILE_H__

#include <stdint.h>

/*
 * CONSTANT POOL INFO STRUCTS
 */
typedef struct
{

  uint8_t tag;
  uint8_t *info;

} CPInfo;

typedef struct
{
  uint8_t tag;
  uint16_t name_index;
} ClassInfo;

typedef struct
{
  uint8_t tag;
  uint16_t class_index;
  uint16_t name_and_type_index;
} FieldrefInfo;

typedef FieldrefInfo MethodrefInfo;
typedef FieldrefInfo InterfaceMethodrefInfo;

typedef struct
{
  uint8_t tag;
  uint16_t string_index;
} StringInfo;

typedef struct
{
  uint8_t tag;
  uint32_t bytes;
} IntegerInfo;

typedef IntegerInfo FloatInfo;

typedef struct
{
  uint8_t tag;
  uint32_t high_bytes;
  uint32_t low_bytes;
} LongInfo;

typedef LongInfo DoubleInfo;

typedef struct
{
  uint8_t tag;
  uint16_t name_index;
  uint16_t descriptor_index;
} NameAndTypeInfo;

typedef struct
{
  uint8_t tag;
  uint16_t length;
  uint8_t *bytes;
} Utf8Info;

typedef struct
{
  uint8_t tag;
  uint8_t reference_kind;
  uint16_t reference_index;
} MethodHandleInfo;

typedef struct
{
  uint8_t tag;
  uint16_t descriptor_index;
} MethodTypeInfo;

typedef struct
{
  uint8_t tag;
  uint16_t bootstrap_method_attr_index;
  uint16_t name_and_type_index;
} InvokeDynamicInfo;

/*
 * ATTRIBUTE INFO STRUCTS
 */
typedef struct
{
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint8_t *info;
} AttributeInfo;

typedef struct
{
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint16_t constantvalue_index;
} ConstantValueAttribute;

typedef struct
{
  uint16_t start_pc;
  uint16_t end_pc;
  uint16_t handler_pc;
  uint16_t catch_type;
} ExceptionTable;

typedef struct
{
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint16_t max_stack;
  uint16_t max_locals;
  uint32_t code_length;
  uint8_t *code;
  uint16_t exception_table_length;
  ExceptionTable *exception_table;
  uint16_t attributes_count;
  AttributeInfo *attributes;
} CodeAttribute;

typedef struct
{
  uint8_t tag;
} VariableInfoTag;

typedef VariableInfoTag TopVariableInfo;
typedef VariableInfoTag VariableInfo;
typedef VariableInfoTag IntegerVariableInfo;
typedef VariableInfoTag FloatVariableInfo;
typedef VariableInfoTag LongVariableInfo;
typedef VariableInfoTag DoubleVariableInfo;
typedef VariableInfoTag NullVariableInfo;
typedef VariableInfoTag UninitializedThisVariableInfo;

typedef struct
{
  uint8_t tag;
  uint16_t cpool_index;
} ObjectVariableInfo;

typedef struct
{
  uint8_t tag;
  uint16_t offset;
} UninitializedVariableInfo;

typedef union
{
  TopVariableInfo top_variable_info;
  IntegerVariableInfo integer_variable_info;
  FloatVariableInfo float_variable_info;
  LongVariableInfo long_variable_info;
  DoubleVariableInfo double_variable_info;
  NullVariableInfo null_variable_info;
  UnitializedThisVariableInfo unitialized_this_variable_info;
  ObjectVariableInfo object_variable_info;
  UninitializedVariableInfo uninitialized_variable_info;
} VerificationTypeInfo;

typedef struct
{
  uint8_t frame_type;
} SameFrame;

typedef struct
{
  uint8_t frame_type;
  VerificationTypeInfo stack[1];
} SameLocals1StackItemFrame;

typedef struct
{
  uint8_t frame_type;
  uint16_t offset_delta;
  VerificationTypeInfo stack[1];
} SameLocals1StackItemFrameExtended;

typedef struct
{
  uint8_t frame_type;
  uint16_t offset_delta;
} ChopFrame;

typedef struct
{
  uint8_t frame_type;
  uint16_t offset_delta;
} SameFrameExtended;

typedef struct
{
  uint8_t frame_type;
  uint16_t offset_delta;
  VerifictionTypeInfo *locals;
} AppendFrame;

typedef struct
{
  uint8_t frame_type;
  uint16_t offset_dela;
  uint16_t number_of_locals;
  VerificationTypeInfo *locals;
  uint16_t number_of_stack_items;
  VerificationTypeInfo *stack;
} FullFrame;

typedef union
{
  SameFrame same_frame;
  SameLocals1StackItemFrame same_locals_1_stack_item_frame;
  SameLocals1StackItemFrameExtended same_locals_1_stack_item_frame_extended;
  ChopFrame chop_frame;
  SameFrameExtended same_frame_extended;
  AppendFrame append_frame;
  FullFrame full_frame;
} StackMapFrame;

typedef struct
{
  uint16_t attriute_name_index;
  uint32_t attribute_length;
  uint16_t number_of_entries;
  StackMapFrame *entries;
} StackMapTableAttribute;

typedef struct
{
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint16_t number_of_exceptions;
  uint16  *exception_index_table;
} ExceptionsAttribute;

typedef struct
{
  uint16_t inner_class_info_index;
  uint16_t outer_class_info_index;
  uint16_t inner_name_index;
  uint16_t inner_class_access_flags;
} InnerClassInfo;

typedef struct
{
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint16_t number_of_classes;
  InnerClassInfo *classes;
} InnerClassesAttribute;

typedef struct
{
  uint16_t attribute_name_index;
  uint32_t attribute_length;
} SyntheticAttribute;

typedef struct
{
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint16_t signature_index;
} SignatureAttribute;
 
typedef struct
{
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint16_t sourcefile_index;
} SourceFileAttribute;
 
/*
 * CLASS FILE STRUCTS
 */
typedef struct
{
  uint16_t access_flags;
  uint16_t name_index;
  uint16_t descriptor_index;
  uint16_t attributes_count;
  AttributeInfo *attributes;
} FieldInfo;

typedef struct
{
  uint16_t access_flags;
  uint16_t name_index;
  uint16_t descriptor_index;
  uint16_t attributes_count;
  AttributeInfo *attributes;
} MethodInfo;

typedef struct
{

  uint32_t magic;
  uint16_t minor_version;
  uint16_t major_version;

  uint16_t constant_pool_count;
  CPInfo *constant_pool;

  uint16_t access_flags;
  uint16_t this_class;
  uint16_t super_class;

  uint16_t interfaces_count;
  uint16_t *interfaces;

  uint16_t fields_count;
  FieldInfo *fields;

  uint16_t methods_count;
  MethodInfo *methods;

  uint16_t attributes_count;
  AttributeInfo *attributes;

} ClassFile;

/*
 * CONSTANT VALUES
 */
static const uint32_t CLASS_FILE_MAGIC_NUMBER = 0xCAFEBABE;

typedef enum
{

  ACC_PUBLIC       = 0x0001, /* Declared public; may be accessed from outside its package */
  ACC_PRIVATE      = 0x0002, /* Declared private; usable only within defining class. */
  ACC_PROTECTED    = 0x0004, /* Declared protected; may be accessed within subclass. */
  ACC_STATIC       = 0x0008, /* Declared static. */
  ACC_FINAL        = 0x0010, /* Declared final; no subclasses allowed */
  ACC_SUPER        = 0x0020, /* Treat superclass methods specially when invoked by the invokespecial instruction */
  ACC_SYNCHRONIZED = 0x0020, /* Declared synchronized; invocation is wrapped by a monitor use. */
  ACC_VOLATILE     = 0x0040, /* Declared volatile; cannot be cached. */
  ACC_BRIDGE       = 0x0040, /* A bridge method, generated by the compiler */
  ACC_TRANSIENT    = 0x0080, /* Declared transient; not written or read by a persisten object manager. */
  ACC_VARARGS      = 0x0080, /* Declared with variable number of arguments */
  ACC_NATIVE       = 0x0100, /* Declared native; implemented in a language other than java. */
  ACC_INTERFACE    = 0x0200, /* Is an inteface, not a class */
  ACC_ABSTRACT     = 0x0400, /* Declared abstract; must not be instantiated */
  ACC_STRICT       = 0x0800, /* Declared strictfp; floating-point mode is FP-strict */
  ACC_SYNTHETIC    = 0x1000, /* Declared synthetic; not present in the source code */
  ACC_ANNOTATION   = 0x2000, /* Declared as an annotation type */
  ACC_ENUM         = 0x4000  /* Declared as an enum type */

} AccessFlag;

typedef enum
{
  
  BYTE    = 'B', /* Signed byte */
  CHAR    = 'C', /* Unicode character code point in the Basic Multilingual Plane, encoded with UTF-16 */
  DOUBLE  = 'D', /* Double-precision floating-point value */
  FLOAT   = 'F', /* Single-precision floating-point value */
  INT     = 'I', /* Integer */
  LONG    = 'J', /* Long integer */
  OBJECT  = 'L', /* An instance of a class */
  SHORT   = 'S', /* Signed short */
  BOOLEAN = 'Z', /* True or false */
  ARRAY   = '['  /* One array dimension */

} FieldType;

const int VOID = 'V'; /* Void descriptor */

typedef enum
{

  CONSTANT_Class              =  7,
  CONSTANT_Fieldref           =  9,
  CONSTANT_Methodref          = 10,
  CONSTANT_InterfaceMethodref = 11,
  CONSTANT_String             =  8,
  CONSTANT_Integer            =  3,
  CONSTANT_Float              =  4,
  CONSTANT_Long               =  5,
  CONSTANT_Double             =  6,
  CONSTANT_NameAndType        = 12,
  CONSTANT_Utf8               =  1,
  CONSTANT_MethodHandle       = 15,
  CONSTANT_MethodType         = 16,
  CONSTANT_InvokeDynamic      = 18

} ConstantPoolTags;

typedef enum
{
  ITEM_Top = 0,
  ITEM_Integer = 1,
  ITEM_Float = 2,
  ITEM_Long = 3,
  ITEM_Double = 4,
  ITEM_Null = 5,
  ITEM_UninitializedThis = 6,
  ITEM_Object = 7,
  ITEM_Unititialized = 8
} ItemTag;

#endif /* __CLASSFILE_H__ */
