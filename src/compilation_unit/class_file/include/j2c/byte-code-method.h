#ifndef __BYTE_CODE_METHOD_H__
#define __BYTE_CODE_METHOD_H__

#include <glib.h>
#include <glib-object.h>
#include <j2c/constant-pool.h>
#include <j2c/method-or-field-info.h>
#include <j2c/method.h>

G_BEGIN_DECLS

#define J2C_TYPE_BYTE_CODE_METHOD   j2c_byte_code_method_get_type ()
G_DECLARE_FINAL_TYPE(J2cByteCodeMethod, j2c_byte_code_method, J2C, BYTE_CODE_METHOD, J2cMethod)

J2cByteCodeMethod *j2c_byte_code_method_new (J2cMethodInfo *info, J2cConstantPool *constant_pool, GError **error);

G_END_DECLS

#endif // __BYTE_CODE_METHOD_H__
