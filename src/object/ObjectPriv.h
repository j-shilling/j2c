/*
 * ObjectPriv.h
 *
 *  Created on: Jul 7, 2017
 *      Author: jake
 */

#ifndef OBJECTPRIV_H_
#define OBJECTPRIV_H_

#include "Class.h"

#define MAGIC_NUMBER 0x0effaced

struct _Object {
  unsigned long magic_number;
  const _Class *class;
};

void *object_constructor (void *_self, va_list *app);
void *object_destructor (void *_self);
bool  object_equals (const void *_self, const void *o);
struct _String *object_to_string (const void *_self);

#endif /* OBJECTPRIV_H_ */
