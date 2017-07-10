/*
 * Class.h
 *
 *  Created on: Jul 7, 2017
 *      Author: jake
 */

#ifndef CLASS_H_
#define CLASS_H_

#include "String.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct _Class _Class;
extern const _Class *Class;

size_t class_sizeof (const _Class *class);
const _Class *class_super (const _Class *class);

void *construct (void *_self, va_list *args);
void *super_construct (const void *class, void *_self, va_list *args);
void *destruct  (void *_self);
void *super_destruct (const void *class, void *_self);
bool  equals    (const void *_self, const void *o);
_String *toString (const void *_self);

#endif /* CLASS_H_ */
