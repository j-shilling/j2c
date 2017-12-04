#ifndef __CLASSFILE_H__
#define __CLASSFILE_H__

#include <glib.h>

typedef enum
{
  INVALID_ARGUMENT,
  MEMORY_ALLOC_ERROR,
  FILE_PARSE_ERROR,
  UNKNOWN_ERROR
} ClassFileError;

/*
 * CONSTANT POOL INFO STRUCTS
 */
typedef struct
{
  guint8 tag;
  guint16 name_index;
} ClassInfo;

typedef struct
{
  guint8 tag;
  guint16 class_index;
  guint16 name_and_type_index;
} FieldrefInfo;

typedef FieldrefInfo MethodrefInfo;
typedef FieldrefInfo InterfaceMethodrefInfo;

typedef struct
{
  guint8 tag;
  guint16 string_index;
} StringInfo;

typedef struct
{
  guint8 tag;
  guint32 bytes;
} IntegerInfo;

typedef IntegerInfo FloatInfo;

typedef struct
{
  guint8 tag;
  guint32 high_bytes;
  guint32 low_bytes;
} LongInfo;

typedef LongInfo DoubleInfo;

typedef struct
{
  guint8 tag;
  guint16 name_index;
  guint16 descriptor_index;
} NameAndTypeInfo;

typedef struct
{
  guint8 tag;
  guint16 length;
  guint8 *bytes;
} Utf8Info;

typedef struct
{
  guint8 tag;
  guint8 reference_kind;
  guint16 reference_index;
} MethodHandleInfo;

typedef struct
{
  guint8 tag;
  guint16 descriptor_index;
} MethodTypeInfo;

typedef struct
{
  guint8 tag;
  guint16 bootstrap_method_attr_index;
  guint16 name_and_type_index;
} InvokeDynamicInfo;

/*
 * ATTRIBUTE INFO STRUCTS
 */
typedef struct
{
  guint16 attribute_name_index;
  guint32 attribute_length;
  guint16 constantvalue_index;
} ConstantValueAttribute;

typedef struct
{
  guint16 start_pc;
  guint16 end_pc;
  guint16 handler_pc;
  guint16 catch_type;
} ExceptionTable;

typedef struct
{
  guint16 attribute_name_index;
  guint32 attribute_length;
  guint16 max_stack;
  guint16 max_locals;
  guint32 code_length;
  guint8 *code;
  guint16 exception_table_length;
  ExceptionTable *exception_table;
  guint16 attributes_count;
  gpointer*attributes;
} CodeAttribute;

typedef struct
{
  guint8 tag;
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
  guint8 tag;
  guint16 cpool_index;
} ObjectVariableInfo;

typedef struct
{
  guint8 tag;
  guint16 offset;
} UninitializedVariableInfo;

typedef union
{
  TopVariableInfo top_variable_info;
  IntegerVariableInfo integer_variable_info;
  FloatVariableInfo float_variable_info;
  LongVariableInfo long_variable_info;
  DoubleVariableInfo double_variable_info;
  NullVariableInfo null_variable_info;
  UninitializedThisVariableInfo unitialized_this_variable_info;
  ObjectVariableInfo object_variable_info;
  UninitializedVariableInfo uninitialized_variable_info;
} VerificationTypeInfo;

typedef struct
{
  guint8 frame_type;
} SameFrame;

typedef struct
{
  guint8 frame_type;
  VerificationTypeInfo stack;
} SameLocals1StackItemFrame;

typedef struct
{
  guint8 frame_type;
  guint16 offset_delta;
  VerificationTypeInfo stack;
} SameLocals1StackItemFrameExtended;

typedef struct
{
  guint8 frame_type;
  guint16 offset_delta;
} ChopFrame;

typedef struct
{
  guint8 frame_type;
  guint16 offset_delta;
} SameFrameExtended;

typedef struct
{
  guint8 frame_type;
  guint16 offset_delta;
  VerificationTypeInfo *locals;
} AppendFrame;

typedef struct
{
  guint8 frame_type;
  guint16 offset_delta;
  guint16 number_of_locals;
  VerificationTypeInfo *locals;
  guint16 number_of_stack_items;
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
  guint16 attribute_name_index;
  guint32 attribute_length;
  guint16 number_of_entries;
  StackMapFrame *entries;
} StackMapTableAttribute;

typedef struct
{
  guint16 attribute_name_index;
  guint32 attribute_length;
  guint16 number_of_exceptions;
  guint16  *exception_index_table;
} ExceptionsAttribute;

typedef struct
{
  guint16 inner_class_info_index;
  guint16 outer_class_info_index;
  guint16 inner_name_index;
  guint16 inner_class_access_flags;
} InnerClassInfo;

typedef struct
{
  guint16 attribute_name_index;
  guint32 attribute_length;
  guint16 number_of_classes;
  InnerClassInfo *classes;
} InnerClassesAttribute;

typedef struct
{
  guint16 attribute_name_index;
  guint32 attribute_length;
} SyntheticAttribute;

typedef struct
{
  guint16 attribute_name_index;
  guint32 attribute_length;
  guint16 signature_index;
} SignatureAttribute;
 
typedef struct
{
  guint16 attribute_name_index;
  guint32 attribute_length;
  guint16 sourcefile_index;
} SourceFileAttribute;
 
/*
 * CLASS FILE STRUCTS
 */
typedef struct
{
  guint16 access_flags;
  guint16 name_index;
  guint16 descriptor_index;
  guint16 attributes_count;
  gpointer*attributes;
} FieldInfo;

typedef struct
{
  guint16 access_flags;
  guint16 name_index;
  guint16 descriptor_index;
  guint16 attributes_count;
  gpointer*attributes;
} MethodInfo;

typedef struct
{

  guint32 magic;
  guint16 minor_version;
  guint16 major_version;

  guint16 constant_pool_count;
  gpointer*constant_pool;

  guint16 access_flags;
  guint16 this_class;
  guint16 super_class;

  guint16 interfaces_count;
  guint16 *interfaces;

  guint16 fields_count;
  FieldInfo *fields;

  guint16 methods_count;
  MethodInfo *methods;

  guint16 attributes_count;
  gpointer*attributes;

} ClassFile;

/*
 * CONSTANT VALUES
 */

/* Identifies a file as a .class file */
static const guint32 CLASS_FILE_MAGIC_NUMBER = 0xCAFEBABE;

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

/* Tag values identify types of structures in the constant pool */
static const guint8 CONSTANT_Class              =  7;
static const guint8 CONSTANT_Fieldref           =  9;
static const guint8 CONSTANT_Methodref          = 10;
static const guint8 CONSTANT_InterfaceMethodref = 11;
static const guint8 CONSTANT_String             =  8;
static const guint8 CONSTANT_Integer            =  3;
static const guint8 CONSTANT_Float              =  4;
static const guint8 CONSTANT_Long               =  5;
static const guint8 CONSTANT_Double             =  6;
static const guint8 CONSTANT_NameAndType        = 12;
static const guint8 CONSTANT_Utf8               =  1;
static const guint8 CONSTANT_MethodHandle       = 15;
static const guint8 CONSTANT_MethodType         = 16;
static const guint8 CONSTANT_InvokeDynamic      = 1;

/* Constant strings identifying types of attribute structures */
static guint8 const *const ATTRIBUTE_ConstantValue                        = "ConstantValue";
static guint8 const *const ATTRIBUTE_Code                                 = "Code";
static guint8 const *const ATTRIBUTE_StackMapTable                        = "StackMapTable";
static guint8 const *const ATTRIBUTE_Exceptions                           = "Exceptions";
static guint8 const *const ATTRIBUTE_InnerClass                           = "InnerClass";
static guint8 const *const ATTRIBUTE_EnclosingMethod                      = "EnclosingMethod";
static guint8 const *const ATTRIBUTE_Synthetic                            = "Synthetic";
static guint8 const *const ATTRIBUTE_Signature                            = "Signature";
static guint8 const *const ATTRIBUTE_SourceFile                           = "SourceFile";
static guint8 const *const ATTRIBUTE_SourceDebugExtension                 = "SourceDebugExtension";
static guint8 const *const ATTRIBUTE_LineNumberTable                      = "LineNumberTable";
static guint8 const *const ATTRIBUTE_LocalVariableTable                   = "LocalVariableTable";
static guint8 const *const ATTRIBUTE_LocalVariableTypeTable               = "LocalVariableTypeTable";
static guint8 const *const ATTRIBUTE_Deprecated                           = "Deprecated";
static guint8 const *const ATTRIBUTE_RuntimeVisibleAnnotations            = "RuntimeVisibleAnnotations";
static guint8 const *const ATTRIBUTE_RuntimeInvisibleAnnotations          = "RuntimeInvisibleAnnotations";
static guint8 const *const ATTRIBUTE_RuntimeVisibleParameterAnnotations   = "RuntimeVisibleParameterAnnotations";
static guint8 const *const ATTRIBUTE_RuntimeInvisibleParameterAnnotations = "RuntimeInvisibleParameterAnnotations";
static guint8 const *const ATTRIBUTE_AnnotationDefault                    = "AnnotationDefault";
static guint8 const *const ATTRIBUTE_BootstrapMethods                     = "BootstrapMethods";

/* Constant tags used in VariableTypeInfo */
static const guint8 ITEM_Top               = 0;
static const guint8 ITEM_Integer           = 1;
static const guint8 ITEM_Float             = 2;
static const guint8 ITEM_Long              = 3;
static const guint8 ITEM_Double            = 4;
static const guint8 ITEM_Null              = 5;
static const guint8 ITEM_UninitializedThis = 6;
static const guint8 ITEM_Object            = 7;
static const guint8 ITEM_Unititialized     = 8;

#endif /* __CLASSFILE_H__ */
