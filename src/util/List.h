/*
 * List.h
 *
 *  Created on: Jul 7, 2017
 *      Author: jake
 */

#ifndef LIST_H_
#define LIST_H_

#include <Class.h>
#include <MutableCollection.h>
#include <Iterator.h>
#include <stdbool.h>

typedef struct _List _List;
extern const _Class *ListClass;
extern const _Class *List;

void initList();
void *list_get (void *_self, unsigned int index);
int list_index_of (void *_self, const void *o);

#endif /* LIST_H_ */
