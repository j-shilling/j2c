#include "MutableCollection.h"
#include "MutableCollectionPriv.h"

#include <Class.h>
#include <ErrorHandling.h>

#include <stdbool.h>
#include <stdarg.h>

const _Class *MutableCollectionClass;
const _Class *MutableCollection;

void
initMutableCollection()
{
  initCollection();

  if (!MutableCollectionClass)
    MutableCollectionClass = new (CollectionClass, "MutableCollectionClass",
	CollectionClass, sizeof (struct _MutableCollectionClass),
	construct, mutable_collection_class_constructor,
	0);

  if (!MutableCollection)
    MutableCollection = new (MutableCollectionClass, "MutableCollection",
	Collection, sizeof (struct _MutableCollection),
	construct, mutable_collection_constructor,
	0);
}

bool
collection_add (void *_self, const void *o)
{
  struct _MutableCollection *self = cast (_self, MutableCollection);
  struct _MutableCollectionClass *class = cast (classOf (self), MutableCollectionClass);
  _String *className = toString (class);

  exit_if_not (class->add != NULL,
	       "%s is not a valid MutableCollectionClass; it does not implement MutableCollection:add",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return class->add(self, o);
}

bool
collection_del (void *_self, const void *o)
{
  struct _MutableCollection *self = cast (_self, MutableCollection);
  struct _MutableCollectionClass *class = cast (classOf (self), MutableCollectionClass);
  _String *className = toString (class);

  exit_if_not (class->del != NULL,
	       "%s is not a valid MutableCollectionClass; it does not implement MutableCollection:del",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return class->del(self, o);
}

void *
mutable_collection_class_constructor (void *_self, va_list *args)
{
  struct _MutableCollectionClass *self = super_construct (MutableCollectionClass, _self, args);

  typedef void (*voidf)();
  voidf selector;

  while ((selector = va_arg (*args, voidf)))
    {
      voidf method = va_arg (*args, voidf);

      if (selector == (voidf)collection_add)
	* (voidf *) & self->add = method;
      else if (selector == (voidf)collection_del)
	* (voidf *) & self->del = method;
    }

  return self;
}

void *
mutable_collection_constructor (void *_self, va_list *args)
{
  struct _MutableCollection *self = super_construct (MutableCollection, _self, args);
  struct _MutableCollectionClass *class = cast (classOf (self), MutableCollectionClass);

  _String *className = toString(class);
  exit_if_not (class->add != NULL
		 && class->del != NULL,
	       "Cannot instantiate %s as a MutableCollection; not all abstract methods are defined.",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return self;
}
