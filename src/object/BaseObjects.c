#include "ObjectPriv.h"
#include "ClassPriv.h"
#include "StringPriv.h"

#include <stdlib.h>

static const struct _Class object[] = {
  {
    {
      MAGIC_NUMBER,
      object + 1
    },
    "Object", object, sizeof (struct _Object),
    object_constructor, object_destructor, object_equals, object_to_string
  },
  {
    {
      MAGIC_NUMBER,
      object + 1
    },
    "Class", object, sizeof (struct _Class),
    class_constructor, class_destructor, object_equals, class_to_string
  },
  {
    {
      MAGIC_NUMBER,
      object + 1
    },
    "String", object, sizeof (struct _String),
    string_constructor, string_destructor, string_equals, string_to_string
  }
};
const struct _Class *Object = object;
const struct _Class *Class  = object + 1;
const struct _Class *String = object + 2;
