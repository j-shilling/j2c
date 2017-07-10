/*
 * CollectionPriv.h
 *
 *  Created on: Jul 9, 2017
 *      Author: jake
 */

#ifndef COLLECTIONPRIV_H_
#define COLLECTIONPRIV_H_

#include <Iterator.h>
#include <Class.h>

#include <ClassPriv.h>
#include <ObjectPriv.h>

#include <stdbool.h>

struct _CollectionClass
{
  const _Class _;

  _Iterator *(*iterator) (const void *_self);
  bool (*contains) (const void *_self, const void *o);
  _Class *(*content_type) (const void *_self);
  unsigned int (*size) (const void *_self);
};

struct _Collection
{
  const _Object _;
};

void *collection_class_constructor (void *_self, va_list *args);
void *collection_constructor (void *_self, va_list *args);
bool collection_equals (const void *_self, const void *o);
_String *collection_to_string (const void *_self);

#endif /* COLLECTIONPRIV_H_ */
