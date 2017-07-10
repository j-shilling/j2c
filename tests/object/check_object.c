#include "CheckReferenceTable.h"

#include <check.h>

Suite *
object_suite()
{
  Suite *s = suite_create ("Object");

  suite_add_tcase (s, reference_table_tcase());

  return s;
}
