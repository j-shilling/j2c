/*
 * ArrayListPriv.h
 *
 *  Created on: Jul 9, 2017
 *      Author: jake
 */

#ifndef ARRAYLISTPRIV_H_
#define ARRAYLISTPRIV_H_

#include "List.h"
#include "ListPriv.h"
#include "Iterator.h"
#include "IteratorPriv.h"

#include <Class.h>
#include <stdbool.h>
#include <stdarg.h>

struct _ArrayList {
  const _List _;

  unsigned int size;
  unsigned int array_length;
  const _Class *content_type;

  void **array;
};

struct _ArrayListIterator {
  const _Iterator _;

  void **array;
  unsigned int index;
  unsigned int size;
};

void *array_list_constructor (void *_self, va_list *args);
void *array_list_destructor (void *_self);
bool array_list_add (void *_self, const void *o);
bool array_list_del (void *_self, const void *o);
unsigned int array_list_size (const void *_self);
_Class *array_list_content_type (const void *_self);
void *array_list_iterator (const void *_self);
bool array_list_contains (const void *_self, const void *o);
void *array_list_get (void *_self, unsigned int index);
int array_list_index_of (void *_self, const void *o);

void *array_list_iterator_constructor (void *_self, va_list *args);
bool array_list_iterator_has_next (const void *_self);
void *array_list_iterator_get_next (void *_self);

#endif /* ARRAYLISTPRIV_H_ */
