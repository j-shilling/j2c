/*
 * StringPriv.h
 *
 *  Created on: Jul 7, 2017
 *      Author: jake
 */

#ifndef STRINGPRIV_H_
#define STRINGPRIV_H_

#include "Object.h"
#include "ObjectPriv.h"

#include <stdarg.h>
#include <stdbool.h>

void *string_constructor (void *_self, va_list *args);
void *string_destructor (void *_self);
bool string_equals (const void *_self, const void *o);
struct _String *string_to_string (const void *_self);

struct _String {
  const _Object _;
  const char *text;
};

#endif /* STRINGPRIV_H_ */
