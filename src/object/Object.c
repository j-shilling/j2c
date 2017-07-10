#include "Object.h"
#include "ObjectPriv.h"
#include "ClassPriv.h"

#include "ErrorHandling.h"

#include "Class.h"
#include "String.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

void *
new (const _Class *class, ...)
{
  exit_if_not (class != NULL,
	       "Object.c:new was given a null class.");

  struct _Object *object;
  va_list ap;

  object = calloc (1, sizeOf (class));
  if (!object)
    return NULL;

  object->class = class;
  object->magic_number = MAGIC_NUMBER;
  
  va_start (ap, class);
  object = construct (object, &ap);
  va_end (ap);

  return ref(object);
}

void
delete (void *_self)
{
  if (_self)
    free (destruct (_self));
}

const _Class *
classOf (const void *_self)
{
  exit_if_not (isObject (_self),
	       "Object.c:classOf was given a pointer to something other than an _Object.");
  const struct _Object *self = _self;
  return self->class;
}

const _Class *
super (const struct _Class *class)
{
  return class->super;
}

size_t
sizeOf (const struct _Class *class)
{
  return class->size;
}

bool
is (const void *_self, const struct _Class *class)
{
  return _self && classOf (_self) == class;
}

bool
isDescendant (const void *_self, const struct _Class *class)
{
  if (_self)
    {
      exit_if_not (isObject (_self),
		   "Object.c:isDescendant was given a pointer to something not an _Object.");
      const struct _Object *self = _self;
      const struct _Class *myClass = classOf (self);

      if (class != Object)
        while (myClass != class)
          if (myClass != Object)
	    myClass = super (myClass);
          else
	    return false;

      return true;      
    }

  return false;
}

bool
isObject (const void *_self)
{
  return _self && ((struct _Object *)_self)->magic_number == MAGIC_NUMBER;
}

void *
cast (const void *_self, const _Class *class)
{
  exit_if_not ((_self != NULL) && (class != NULL),
	       "Object.c:cast was given a null parameter.");
  exit_if_not (isObject(_self),
	       "Object.c:cast was given a pointer to something not an _Object.");

  exit_if_not (isDescendant (_self, class),
	       "Object.c:cast cannot make a %s from a %s.",
	       class->name,
	       ((const _Object *)_self)->class->name);

  return (void *)_self;
}

void *
object_constructor (void *_self, va_list *args)
{
  return _self;
}

void *
object_destructor (void *_self)
{
  return _self;
}

bool
object_equals (const void *this, const void *that)
{
  return this == that;
}

_String *
object_to_string (const void *_self)
{
  exit_if_not (isObject (_self),
	       "Object.c:object_to_string was given a pointer to something other than an _Object.");
  const struct _Object *self = _self;

  const char *className = string_to_cstring (toString(classOf(self)));
  int len = snprintf (0, 0, "%s@%p", className, _self);

  char *buf = calloc (1, len + 1);
  sprintf (buf, "%s@%p", className, _self);

  struct _String *ret = new (String, buf);
  free (buf);

  return ret;
}
