/*
 * ListPriv.h
 *
 *  Created on: Jul 9, 2017
 *      Author: jake
 */

#ifndef LISTPRIV_H_
#define LISTPRIV_H_

#include "MutableCollection.h"
#include "MutableCollectionPriv.h"

#include <stdarg.h>

struct _ListClass
{
  const _MutableCollectionClass _;

  void *(*get) (void *_self, unsigned int index);
  int (*index_of) (void *_self, const void *o);
};

struct _List
{
  const _MutableCollection _;
};

void *list_class_constructor (void *_self, va_list *args);
void *list_constructor (void *_self, va_list *args);

#endif /* LISTPRIV_H_ */
