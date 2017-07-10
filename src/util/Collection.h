/*
 * Collection.h
 *
 *  Created on: Jul 9, 2017
 *      Author: jake
 */

#ifndef COLLECTION_H_
#define COLLECTION_H_

#include <Class.h>
#include <Object.h>

#include <Iterator.h>

#include <stdbool.h>

typedef struct _CollectionClass _CollectionClass;
typedef struct _Collection _Collection;

extern const _Class *CollectionClass;
extern const _Class *Collection;

void initCollection();

_Iterator *collection_iterator (const void *_self);
bool collection_contains (const void *_self, const void *o);
_Class *collection_content_type (const void *_self);
unsigned int collection_size (const void *_self);
bool collection_is_empty (const void *_self);

#endif /* COLLECTION_H_ */
