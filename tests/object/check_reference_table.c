#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <Object.h>

START_TEST (test_reference_table_stress)
{
  static const int TESTS = 100000;
  void **objs = calloc (TESTS, sizeof (void *));

  for (int i = 0; i < TESTS; i++)
    {
      objs[i] = new (Object);
    }

  for (int i = 0; i < TESTS; i++)
    {
      unref (objs[i]);
    }

  free (objs);
}
END_TEST

/*
int
main (void)
{
  int number_failed;

  Suite *s;
  SRunner *sr;
  TCase *tc_core;

  s = suite_create ("ReferenceTable");
  tc_core = tcase_create ("Core");

  tcase_add_test(tc_core, test_reference_table_stress);

  suite_add_tcase (s, tc_core);

  sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}*/

TCase *
reference_table_tcase()
{
  TCase *tc_core = tcase_create ("ReferenceTable");

  tcase_add_test (tc_core, test_reference_table_stress);

  return tc_core;
}
