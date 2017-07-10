#include "ArrayList.h"
#include "ArrayListPriv.h"
#include "List.h"
#include "Iterator.h"

#include <Object.h>
#include <Class.h>
#include <ErrorHandling.h>

#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

const _Class *ArrayList;
const _Class *ArrayListIterator;

void
initArrayList()
{
  initList();
  initIterator();

  if (!ArrayList)
    ArrayList = new (ListClass, "ArrayList",
	List,
	sizeof (struct _ArrayList),
	construct, array_list_constructor,
	destruct, array_list_destructor,
	0,
	collection_size, array_list_size,
	collection_content_type, array_list_content_type,
	collection_iterator, array_list_iterator,
	collection_contains, array_list_contains,
	0,
	collection_add, array_list_add,
	collection_del, array_list_del,
	0,
	list_get, array_list_get,
	list_index_of, array_list_index_of,
	0);

  if (!ArrayListIterator)
    ArrayListIterator = new (IteratorClass, "ArrayListIterator",
	Iterator,
	sizeof (struct _ArrayListIterator),
	construct, array_list_iterator_constructor,
	0,
	iterator_has_next, array_list_iterator_has_next,
	iterator_get_next, array_list_iterator_get_next,
	0);
}

void *
array_list_constructor (void *_self, va_list *args)
{
  struct _ArrayList *self = super_construct (ArrayList, _self, args);

  self->size = 0;
  self->array_length = 10;
  self->content_type = va_arg (*args, const _Class *);

  self->array = calloc (self->array_length, sizeof (void *));

  return self;
}

void *
array_list_destructor (void *_self)
{
  struct _ArrayList *self = cast (_self, ArrayList);
  for (int i = 0; i < self->size; i++)
    unref (self->array[i]);
  free (self->array);
  return super_destruct (ArrayList, self);
}

bool
array_list_add (void *_self, const void *o)
{
  struct _ArrayList *self = cast (_self, ArrayList);
  if (!is (o, self->content_type) && !isDescendant (o, self->content_type))
    return false;
  const void *obj = cast (o, self->content_type);
  exit_if_not (self->array != NULL,
	       "This array list does not have an array.");

  if (self->size >= self->array_length)
    {
      void **new = calloc (self->array_length + 10, sizeof (void *));
      if (!new)
	return false;

      memcpy (new, self->array, self->size * sizeof (void *));
      free (self->array);
      self->array = new;
      self->array_length += 10;
    }

  self->array[self->size] = ref(obj);
  self->size ++;

  return true;
}

bool
array_list_del (void *_self, const void *o)
{
  struct _ArrayList *self = cast (_self, ArrayList);
  for (int i = 0; i < self->size; i++)
    {
      if (equals(self->array[i], o))
	{
	  unref (self->array[i]);
	  for (int j = i; i < (self->size - 1); i++)
	    self->array[j] = self->array[j+1];
	  self->array[self->size-1] = NULL;
	  self->size --;

	  if (self->size < (self->array_length - 10))
	    {
	      void **new = calloc (self->array_length - 10, sizeof (void *));
	      if (!new)
		return true;

	      memcpy (new, self->array, self->size * sizeof (void *));
	      free (self->array);
	      self->array = new;
	      self->array_length -= 10;
	    }
	  return true;
	}
    }

  return false;
}

unsigned int
array_list_size (const void *_self)
{
  struct _ArrayList *self = cast (_self, ArrayList);
  return self->size;
}

_Class *
array_list_content_type (const void *_self)
{
  struct _ArrayList *self = cast (_self, ArrayList);
  return (_Class *)self->content_type;
}

void *
array_list_get (void *_self, unsigned int index)
{
  struct _ArrayList *self = cast (_self, ArrayList);
  if (index < 0 || index >= self->size)
    return NULL;

  return self->array[index];
}

int
array_list_index_of (void *_self, const void *o)
{
  struct _ArrayList *self = cast (_self, ArrayList);

  for (int i = 0; i < self->size; i++)
    if (equals (o, self->array[i]))
      return i;

  return -1;
}

void *
array_list_iterator (const void *_self)
{
  struct _ArrayList *self = cast (_self, ArrayList);
  return new (ArrayListIterator, self);
}

bool
array_list_contains (const void *_self, const void *o)
{
  struct _ArrayList *self = cast (_self, ArrayList);
    for (int i = 0; i < self->size; i++)
      {
        if (equals(self->array[i], o))
  	{
  	  return true;
  	}
      }

    return false;
}

void *
array_list_iterator_constructor (void *_self, va_list *args)
{
  struct _ArrayListIterator *self = super_construct (ArrayListIterator, _self, args);
  struct _ArrayList *list = va_arg (*args, struct _ArrayList *);

  self->array = list->array;
  self->index = 0;
  self->size = list->size;

  return self;
}

bool
array_list_iterator_has_next (const void *_self)
{
  struct _ArrayListIterator *self = cast (_self, ArrayListIterator);
  return self->index < self->size;
}

void *
array_list_iterator_get_next (void *_self)
{
  struct _ArrayListIterator *self = cast (_self, ArrayListIterator);
  if (self->index < 0 || self->index >= self->size)
    return NULL;

  void *ret = self->array[self->index];
  self->index ++;

  return ret;
}
