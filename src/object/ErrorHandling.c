/*
 * ErrorHandling.c
 *
 *  Created on: Jul 7, 2017
 *      Author: jake
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include <execinfo.h>

void
exit_if_not (bool condition, const char *_fmt, ...)
{
  if (condition)
    return;

  char *fmt;
  bool automatic = true;
  if (_fmt[strlen(_fmt) - 1] != '\n')
    {
      fmt = calloc (strlen (_fmt) + 2, sizeof(char));
      sprintf (fmt, "%s\n", _fmt);
      automatic = false;
    }
  else
    {
      fmt = (char *)_fmt;
    }

  va_list args;
  va_start (args, _fmt);
  vfprintf (stderr, fmt, args);
  va_end (args);

  if (!automatic)
    free (fmt);

  void *bt[20];
  size_t size = backtrace (bt, 20);
  char **out = backtrace_symbols (bt, size);

  for (int i = 0; i < size; i++)
    fprintf (stderr, "%s\n", out[i]);

  free (out);

  exit (1);
}
