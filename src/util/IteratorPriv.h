/*
 * IteratorPriv.h
 *
 *  Created on: Jul 9, 2017
 *      Author: jake
 */

#ifndef ITERATORPRIV_H_
#define ITERATORPRIV_H_

#include <stdbool.h>
#include <stdarg.h>

#include <Object.h>
#include <ObjectPriv.h>
#include <ClassPriv.h>

void *iterator_class_constructor (void *_self, va_list *args);
void *iterator_constructor (void *_self, va_list *args);

struct _Iterator {
  const _Object _;
};

struct _IteratorClass {
  const _Class _;

  bool (*has_next) (const void *_self);
  void *(*get_next) (void *_self);
};

#endif /* ITERATORPRIV_H_ */
