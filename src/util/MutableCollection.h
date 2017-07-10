/*
 * MutableCollection.h
 *
 *  Created on: Jul 9, 2017
 *      Author: jake
 */

#ifndef MUTABLECOLLECTION_H_
#define MUTABLECOLLECTION_H_

#include <Class.h>
#include <Collection.h>

#include <stdbool.h>

typedef struct _MutableCollectionClass _MutableCollectionClass;
typedef struct _MutableCollection _MutableCollection;

extern const _Class *MutableCollectionClass;
extern const _Class *MutableCollection;

void initMutableCollection();

bool collection_add (void *_self, const void *o);
bool collection_del (void *_self, const void *o);

#endif /* MUTABLECOLLECTION_H_ */
