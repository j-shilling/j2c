#ifndef __CLASS_FILE_H__
#define __CLASS_FILE_H__

#include <glib-object.h>
#include <glib.h>

#include <j2c/indexed-file.h>
#include <j2c/readable.h>

/* CLASS FILE CONSTANT VALUES */

static const guint32 J2C_CLASS_FILE_MAGIC = 0xCAFEBABE;

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

G_BEGIN_DECLS

#define J2C_TYPE_CLASS_FILE j2c_class_file_get_type ()
G_DECLARE_FINAL_TYPE (J2cClassFile, j2c_class_file, J2C, CLASS_FILE, J2cIndexedFile);

J2cClassFile *j2c_class_file_new (J2cReadable *readable, GError **error);

G_END_DECLS

#endif /* __CLASS_FILE_H__ */
