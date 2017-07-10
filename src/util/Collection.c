#include "Collection.h"
#include "CollectionPriv.h"
#include "Iterator.h"

#include <String.h>
#include <ErrorHandling.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

const _Class *CollectionClass;
const _Class *Collection;

void
initCollection()
{
  initIterator();
  if (!CollectionClass)
    CollectionClass = new (Class, "CollectionClass",
	Class,
	sizeof (struct _CollectionClass),
	construct, collection_class_constructor,
	0);
  if (!Collection)
    Collection = new (CollectionClass, "Collection",
	Object,
	sizeof (struct _Collection),
	construct, collection_constructor,
	equals, collection_equals,
	toString, collection_to_string,
	0);
}

_Iterator *
collection_iterator (const void *_self)
{
  const struct _Collection *self = cast (_self, Collection);
  const struct _CollectionClass *class = cast (classOf(self), CollectionClass);
  const _String *className = toString (class);

  exit_if_not (class->iterator != NULL,
	       "%s is not a valid CollectionClass; it does not implement CollectionClass:iterator",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return class->iterator(self);
}

bool
collection_contains (const void *_self, const void *o)
{
  const struct _Collection *self = cast (_self, Collection);
  const struct _CollectionClass *class = cast (classOf(self), CollectionClass);
  const _String *className = toString (class);

  exit_if_not (class->contains != NULL,
	       "%s is not a valid CollectionClass; it does not implement CollectionClass:contains",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return class->contains(self, o);
}

_Class *
collection_content_type (const void *_self)
{
  const struct _Collection *self = cast (_self, Collection);
  const struct _CollectionClass *class = cast (classOf(self), CollectionClass);
  const _String *className = toString (class);

  exit_if_not (class->content_type != NULL,
	       "%s is not a valid CollectionClass; it does not implement CollectionClass:content_type",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return class->content_type(self);
}

unsigned int
collection_size (const void *_self)
{
  const struct _Collection *self = cast (_self, Collection);
  const struct _CollectionClass *class = cast (classOf(self), CollectionClass);
  const _String *className = toString (class);

  exit_if_not (class->size != NULL,
	       "%s is not a valid CollectionClass; it does not implement CollectionClass:size",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return class->size(self);
}

void *
collection_class_constructor (void *_self, va_list *args)
{
    struct _CollectionClass *self = super_construct (CollectionClass, _self, args);

    typedef void (*voidf)();
    voidf selector;

    while ((selector = va_arg (*args, voidf)))
      {
        voidf method = va_arg (*args, voidf);

        if (selector == (voidf)collection_content_type)
  	* (voidf *) & self->content_type = method;
        else if (selector == (voidf)collection_size)
  	* (voidf *) & self->size = method;
        else if (selector == (voidf)collection_contains)
  	* (voidf *) & self->contains = method;
        else if (selector == (voidf)collection_iterator)
  	* (voidf *) & self->iterator = method;
      }

    return self;
}

void *
collection_constructor (void *_self, va_list *args)
{
    struct _Collection *self = super_construct (Collection, _self, args);
    struct _CollectionClass *class = cast (classOf (self), CollectionClass);

    _String *className = toString(class);
    exit_if_not (class->contains != NULL
		 && class->content_type != NULL
		 && class->size != NULL
		 && class->iterator != NULL,
  	       "Cannot instantiate %s as a Collection; not all abstract methods are defined.",
  	       string_to_cstring (className));
    unref ((_Object *)className);

    return self;
}

bool
collection_equals (const void *_self, const void *o)
{
  struct _Collection *self = cast (_self, Collection);

  if (!isDescendant (o, Collection))
    return false;

  struct _Collection *obj = cast (o, Collection);

  if (collection_size (self) != collection_size (obj))
    return false;

  _Iterator *it = collection_iterator (self);
  while (iterator_has_next (it))
    if (!collection_contains (obj, iterator_get_next (it)))
      return false;

  return true;
}

_String *
collection_to_string (const void *_self)
{
  _String *sb = new (String, "{");
  _Iterator *it = collection_iterator (_self);
  _String *old;

  while (iterator_has_next (it))
    {
      old = sb;
      _String *obj = toString (iterator_get_next (it));

      if (string_ends_with (sb, "]"))
	{
	  sb = string_concat (sb, ", ");
	  unref ((_Object *)old);
	}

      sb = string_concat (sb, "[");
      unref ((_Object *)old);
      old = sb;
      sb = string_concat (sb, obj);
      unref ((_Object *)old);
      old = sb;
      sb = string_concat (sb, "]");
      unref ((_Object *)old);
    }

  old = sb;
  sb = string_concat (sb, "}");
  unref ((_Object *)old);

  return sb;
}

bool
collection_is_empty (const void *_self)
{
  return collection_size (_self) == 0;
}
