#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Class.h"
#include "String.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct _Object _Object;

extern const _Class *Object;

void * new (const _Class *class, ...);
void delete (void *this);

const _Class *classOf (const void *_self);
size_t sizeOf (const _Class *class);
const _Class *super   (const _Class *_self);

bool is (const void *_self, const _Class *class);
bool isDescendant (const void *_self, const _Class *class);
bool isObject (const void *_self);
void *cast (const void *_self, const _Class *class);

void *ref (const _Object *ref);
void unref (const _Object *ref);

#endif /* __OBJECT_H__ */
