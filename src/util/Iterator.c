#include "List.h"
#include "IteratorPriv.h"

#include <Object.h>
#include <Class.h>
#include <ClassPriv.h>
#include <String.h>
#include <ErrorHandling.h>

#include <stdbool.h>
#include <stdarg.h>

const _Class *IteratorClass;
const _Class *Iterator;

void
initIterator()
{
  if (!IteratorClass)
    IteratorClass = new (Class, "IteratorClass", Class,
	sizeof (struct _IteratorClass),
	construct, iterator_class_constructor,
	0);
  if (!Iterator)
    Iterator = new (IteratorClass, "Iterator", Object,
	sizeof (struct _Iterator),
	construct, iterator_constructor,
	0);
}

bool
iterator_has_next (const void *_self)
{
  struct _Iterator *self = cast (_self, Iterator);
  struct _IteratorClass *class = cast (classOf(self), IteratorClass);
  _String *str = toString (classOf (self));

  exit_if_not (class->has_next != NULL,
	       "%s is not a valid Iterator; method has_next is not implemented",
	       string_to_cstring (str));
  unref ((_Object *)str);

  return class->has_next(self);
}

void *
iterator_get_next (void *_self)
{
  struct _Iterator *self = cast (_self, Iterator);
  struct _IteratorClass *class = cast (classOf(self), IteratorClass);
  _String *str = toString (classOf (self));

  exit_if_not (class->has_next != NULL,
	       "%s is not a valid Iterator; method get_next is not implemented",
	       string_to_cstring (str));
  unref ((_Object *)str);

  return class->get_next(self);
}

void *
iterator_class_constructor (void *_self, va_list *args)
{
  struct _IteratorClass *self = super_construct (IteratorClass, _self, args);

  typedef void (* voidf) ();
    voidf selector = NULL;
    while ((selector = va_arg (*args, voidf)))
      {
        voidf method = va_arg (*args, voidf);

        if (selector == (voidf) iterator_has_next)
          *(voidf *) & self->has_next = method;
        else if (selector == (voidf) iterator_get_next)
          *(voidf *) & self->get_next = method;
      }

    return self;
}

void *
iterator_constructor (void *_self, va_list *args)
{
  struct _Iterator *self = super_construct (Iterator, _self, args);
  struct _IteratorClass *class = cast (classOf (self), IteratorClass);

  _String *className = toString(class);
  exit_if_not (class->get_next != NULL && class->has_next != NULL,
	       "Cannot instantiate %s as an iterator; not all abstract methods are defined.",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return self;
}
