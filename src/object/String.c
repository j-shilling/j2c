#include "String.h"
#include "StringPriv.h"

#include "Object.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

const char *
string_to_cstring (const struct _String *_self)
{
  if (!_self)
    return "";

  return _self->text;
}

void *
string_constructor (void *_self, va_list *args)
{
  char *str = va_arg (*args, char *);
  if (str == NULL)
    str = "";

  int len = strlen (str);
  struct _String *self = _self;

  self->text = calloc (1, len + 1);
  strcpy ((char *)self->text, str);

  return self;
}

void *
string_destructor (void *_self)
{
  struct _String *self = _self;
  if (self && self->text)
    free ((void *)self->text);
  return self;
}

bool
string_equals (const void *_self, const void *o)
{
  if (!isObject (_self) || !is (_self, String))
    return false;
  if (!isObject (o) || !is (o, String))
    return false;

  struct _String *self = cast (_self, String);
  struct _String *obj = cast (o, String);

  return strcmp (self->text, obj->text) == 0;
}

struct _String *
string_to_string (const void *_self)
{
  struct _String *self = cast (_self, String);
  return self;
}

unsigned int
string_length (const _String *self)
{
  return strlen (self->text);
}

_String *
string_concat_object (const _String *self, const void *o)
{
  const struct _String *str = toString (o);

  int size = snprintf (0, 0, "%s%s",
		       string_to_cstring (self),
		       string_to_cstring (str));
  char *buf = calloc (size, sizeof (char));
  if (!buf)
    return NULL;
  snprintf (buf, size, "%s%s",
	    string_to_cstring (self),
	    string_to_cstring (str));

  struct _String *ret = new (String, buf);
  free (buf);
  return ret;
}
_String *
string_concat_cstring (const _String *self, const char *str)
{
    int size = snprintf (0, 0, "%s%s",
  		       string_to_cstring (self),
  		       str);
    char *buf = calloc (size, sizeof (char));
    if (!buf)
      return NULL;
    snprintf (buf, size, "%s%s",
  	    string_to_cstring (self),
  	    str);

    struct _String *ret = new (String, buf);
    free (buf);
    return ret;
}

bool
string_ends_with_object (const _String *self, const void *o)
{
  _String *str = toString (o);

  int mylen = string_length (self); int yourlen = string_length (str);
  if (yourlen > mylen)
    return false;

 int i = mylen - 1; int j = yourlen - 1;
  while (i >= 0 && j >= 0)
    {
      if (str->text[j] != self->text[i])
	return false;

      i--; j--;
    }

  return true;
}

bool
string_ends_with_cstring (const _String *self, const char *str)
{
  int mylen = string_length (self); int yourlen = strlen (str);
    if (yourlen > mylen)
      return false;

    int i = mylen - 1; int j = yourlen - 1;
    while (i >= 0 && j >= 0)
      {
        if (str[j] != self->text[i])
  	return false;

        i--; j--;
      }

    return true;
}
