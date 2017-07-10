#include <check.h>
#include <stdlib.h>

#include <CheckObject.h>
#include <CheckUtil.h>

int
main (void)
{
  int number_failed;
  SRunner *sr;

  sr = srunner_create (object_suite());
  srunner_add_suite (sr, util_suite());

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
