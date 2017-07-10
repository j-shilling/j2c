#include "Object.h"
#include "String.h"

#include "Class.h"
#include "ClassPriv.h"

#include "ErrorHandling.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

void *
construct (void *_self, va_list *app)
{
  const struct _Class *class = classOf (_self);

  exit_if_not (class->construct != NULL,
	       "%s does not implement required method Class:construct.",
	       class->name);
  return class->construct (_self, app);
}

void *
super_construct (const void *_class,
		void *_self, va_list *app)
{
  const struct _Class *superclass = super (_class);

  exit_if_not (_self != NULL,
	       "Cannot access the superclass of a null pointer.");
  exit_if_not (superclass->construct != NULL,
  	     "%s does not implement required method Class:construct.",
  	     superclass->name);
  return superclass->construct (_self, app);
}

void *
destruct (void *_self)
{
  const struct _Class *class = classOf (_self);

 exit_if_not (class->destruct != NULL,
  	       "%s does not implement required method Class:destruct.",
  	       class->name);
  return class->destruct (_self);
}

void *
super_destruct (const void *_class, void *_self)
{
  const struct _Class *superclass = super (_class);

  exit_if_not (_self != NULL,
	       "Cannot access the superclass of a null pointer.");
  exit_if_not (superclass->destruct != NULL,
  	       "%s does not implement required method Class:destruct.",
  	       superclass->name);
  return superclass->destruct (_self);
}

bool
equals (const void *_self, const void *o)
{
  if (!_self)
    return _self == o;

  const struct _Class *class = classOf (_self);

  exit_if_not (class->equals != NULL,
  	       "%s does not implement required method Class:equals.",
  	       class->name);
  return class->equals (_self, o);
}

struct _String *
toString (const void *_self)
{
  const struct _Class *class = classOf (_self);

  exit_if_not (class->toString != NULL,
  	       "%s does not implement required method Class:toString.",
  	       class->name);
  return class->toString (_self);
}

size_t
class_sizeof (const struct _Class *class)
{
  return class->size;
}

const struct _Class *
class_super (const struct _Class *class)
{
  return class->super;
}

void *
class_constructor (void *_self, va_list *args)
{
  struct _Class *self = _self;

  self->name = va_arg(*args, char *);
  self->super = va_arg(*args, struct _Class *);
  self->size = va_arg(*args, size_t);

  exit_if_not (self->super != NULL,
	       "%s is missing its superclass",
	       self->name);

  const size_t offset = offsetof (struct _Class, construct);
  memcpy ((char *) self + offset, (char *) self->super + offset,
		  sizeOf (Class) - offset);

  typedef void (* voidf) ();
  voidf selector;
  while ((selector = va_arg (*args, voidf)))
    {
      voidf method = va_arg (*args, voidf);

      if (selector == (voidf) construct)
        *(voidf *) & self->construct = method;
      else if (selector == (voidf) destruct)
        *(voidf *) & self->destruct = method;
      else if (selector == (voidf) equals)
        *(voidf *) & self->equals = method;
    }

  return self;
}

void *
class_destructor (void *this)
{
  return NULL;
}

struct _String *
class_to_string (const void *_self)
{
  struct _Class *class = cast (_self, Class);
  return new (String, class->name);
}


