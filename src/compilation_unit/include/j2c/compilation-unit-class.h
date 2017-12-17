#ifndef __COMPILATION_UNIT_CLASS_H__
#define __COMPILATION_UNIT_CLASS_H__

#include <glib.h>
#include <glib-object.h>

#include <j2c/compilation-unit.h>
#include <j2c/indexed-file.h>

G_BEGIN_DECLS

static const guint16 ACC_PUBLIC     = 0x0001;
static const guint16 ACC_FINAL      = 0x0010;
static const guint16 ACC_SUPER      = 0x0020;
static const guint16 ACC_INTERFACE  = 0x0200;
static const guint16 ACC_ABSTRACT   = 0x0400;
static const guint16 ACC_SYNTHETIC  = 0x1000;
static const guint16 ACC_ANNOTATION = 0x2000;
static const guint16 ACC_ENUM       = 0x4000;

#define J2C_TYPE_COMPILATION_UNIT_CLASS	j2c_compilation_unit_class_get_type ()
G_DECLARE_FINAL_TYPE (J2cCompilationUnitClass, j2c_compilation_unit_class, J2C, COMPILATION_UNIT_CLASS, GObject)

J2cCompilationUnitClass *j2c_compilation_unit_class_new (J2cIndexedFile *file, GError **error);

G_END_DECLS

#endif /* __COMPILATION_UNIT_CLASS_H__ */
