/*
 * String.h
 *
 *  Created on: Jul 7, 2017
 *      Author: jake
 */

#ifndef STRING_H_
#define STRING_H_

#include <stdbool.h>

#define string_concat(x, y) _Generic ((y), \
				       char *: string_concat_cstring, \
				 const char *: string_concat_cstring, \
				      default: string_concat_object) (x, y)
#define string_ends_with(x, y) _Generic ((y), \
				       char *: string_ends_with_cstring, \
				 const char *: string_ends_with_cstring, \
				      default: string_ends_with_object) (x, y)

typedef struct _String _String;
extern const struct _Class *String;

const char *string_to_cstring (const _String *string);
unsigned int string_length (const _String *self);
_String *string_concat_object (const _String *self, const void *o);
_String *string_concat_cstring (const _String *self, const char *str);
bool string_ends_with_object (const _String *self, const void *o);
bool string_ends_with_cstring (const _String *self, const char *str);

#endif /* STRING_H_ */
