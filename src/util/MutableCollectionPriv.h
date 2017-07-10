/*
 * MutableCollectionPriv.h
 *
 *  Created on: Jul 9, 2017
 *      Author: jake
 */

#ifndef MUTABLECOLLECTIONPRIV_H_
#define MUTABLECOLLECTIONPRIV_H_

#include "Collection.h"
#include "CollectionPriv.h"

#include <stdbool.h>
#include <stdarg.h>

struct _MutableCollectionClass
{
  const _CollectionClass _;

  bool (*add) (void *_self, const void *o);
  bool (*del) (void *_self, const void *o);
};

struct _MutableCollection
{
  const _Collection _;
};

void *mutable_collection_class_constructor (void *_self, va_list *args);
void *mutable_collection_constructor (void *_self, va_list *args);

#endif /* MUTABLECOLLECTIONPRIV_H_ */
