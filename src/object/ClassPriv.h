/*
 * ClassPriv.h
 *
 *  Created on: Jul 7, 2017
 *      Author: jake
 */

#ifndef CLASSPRIV_H_
#define CLASSPRIV_H_

#include "Object.h"
#include "ObjectPriv.h"
#include "String.h"

#include <stdarg.h>
#include <stdbool.h>

void *class_constructor (void *_self, va_list *app);
void *class_destructor (void *_self);
struct _String *class_to_string (const void *_self);

struct _Class {
  const _Object _;
  const char *name;
  const struct _Class *super;
  size_t size;

  void * (*construct) (void *_self, va_list *args);
  void * (*destruct) (void *_self);
  bool   (*equals) (const void *_self, const void *o);
  struct _String *(*toString) (const void *_self);
};

#endif /* CLASSPRIV_H_ */
