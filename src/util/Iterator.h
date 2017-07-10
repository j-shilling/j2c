/*
 * Iterator.h
 *
 *  Created on: Jul 7, 2017
 *      Author: jake
 */

#ifndef ITERATOR_H_
#define ITERATOR_H_

#include <Class.h>
#include <stdbool.h>

typedef struct _Iterator _Iterator;
typedef struct _IteratorClass _IteratorClass;

extern const _Class *Iterator;
extern const _Class *IteratorClass;

void initIterator();
void *iterator_get_next (void *_self);
bool iterator_has_next (const void *_self);

#endif /* ITERATOR_H_ */
