#include <check.h>

#include "CheckArrayList.h"

Suite *
util_suite()
{
  Suite *s = suite_create ("Util");

  suite_add_tcase (s, array_list_tcase());

  return s;
}
