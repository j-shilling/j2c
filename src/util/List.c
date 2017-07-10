#include "List.h"
#include "ListPriv.h"

#include <Class.h>
#include <Object.h>
#include <ErrorHandling.h>

const _Class *ListClass;
const _Class *List;

void
initList()
{
  initMutableCollection();

  if (!ListClass)
    ListClass = new (MutableCollectionClass, "ListClass",
	MutableCollectionClass, sizeof (struct _ListClass),
	construct, list_class_constructor,
	0);

  if (!List)
    List = new (ListClass, "List",
	MutableCollection, sizeof (struct _List),
	construct, list_constructor,
	0);
}

void *
list_get (void *_self, unsigned int index)
{
  struct _List *self = cast (_self, List);
  struct _ListClass *class = cast (classOf (self), ListClass);
  _String *className = toString (class);

  exit_if_not (class->get != NULL,
	       "%s is not a valid ListClass; it does not implement List:get",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return class->get(self, index);
}

int
list_index_of (void *_self, const void *o)
{
  struct _List *self = cast (_self, List);
  struct _ListClass *class = cast (classOf (self), ListClass);
  _String *className = toString (class);

  exit_if_not (class->index_of != NULL,
	       "%s is not a valid ListClass; it does not implement List:index_of",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return class->index_of(self, o);
}

void *
list_class_constructor (void *_self, va_list *args)
{
  struct _ListClass *self = super_construct (ListClass, _self, args);

    typedef void (*voidf)();
    voidf selector;

    while ((selector = va_arg (*args, voidf)))
      {
        voidf method = va_arg (*args, voidf);

        if (selector == (voidf)list_get)
  	* (voidf *) & self->get = method;
        else if (selector == (voidf)list_index_of)
  	* (voidf *) & self->index_of = method;
      }

    return self;
}

void *
list_constructor (void *_self, va_list *args)
{
  struct _List *self = super_construct (List, _self, args);
  struct _ListClass *class = cast (classOf (self), ListClass);

  _String *className = toString(class);
  exit_if_not (class->get != NULL
		 && class->index_of != NULL,
	       "Cannot instantiate %s as a ListClass; not all abstract methods are defined.",
	       string_to_cstring (className));
  unref ((_Object *)className);

  return self;
}
