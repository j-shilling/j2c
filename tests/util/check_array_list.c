#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include <Object.h>
#include <Collection.h>
#include <MutableCollection.h>
#include <List.h>
#include <ArrayList.h>
#include <Iterator.h>
#include <String.h>

#define TESTS		1000

START_TEST(test_array_list_create)
{
  _ArrayList *list = new (ArrayList, String);

  ck_assert_ptr_eq (collection_content_type (list), String);
  ck_assert_int_eq (collection_size (list), 0);

  unref ((_Object *)list);
}
END_TEST

START_TEST(test_array_list_add)
{
  _ArrayList *list = new (ArrayList, String);
  _String *str = new (String, "one");

  ck_assert (!collection_add (list, ArrayList));
  ck_assert_int_eq (collection_size (list), 0);

  ck_assert (collection_add (list, str));
  ck_assert_int_eq (collection_size (list), 1);
  ck_assert (collection_contains (list, str));

  unref ((_Object *)list);
  unref ((_Object *)str);
}
END_TEST

START_TEST(test_array_list_add_stress)
{
  _ArrayList *list = new (ArrayList, String);
  const static int SIZE = TESTS;
  char buf [5];
  memset (buf, '\0', sizeof (buf));

  for (int i = 0; i < SIZE; i++)
    {

      snprintf (buf, sizeof (buf), "%d", i);
      _String *str = new (String, buf);
      collection_add (list, str);

      ck_assert_int_eq (collection_size (list), i + 1);
      ck_assert (collection_contains (list, str));

      unref ((_Object *)str);
    }

  unref ((_Object *)list);
}
END_TEST

START_TEST (test_array_list_del)
{
  _ArrayList *list = new (ArrayList, String);
  _String *one = new (String, "one");
  _String *two = new (String, "two");
  _String *three = new (String, "three");

  ck_assert (collection_add (list, one));
  ck_assert (collection_add (list, two));

  ck_assert_int_eq (collection_size (list), 2);

  ck_assert (!collection_del (list, three));
  ck_assert (collection_del (list, one));
  ck_assert (collection_contains (list, two));
  ck_assert (!collection_contains (list, one));
  ck_assert_int_eq (collection_size (list), 1);

  unref ((_Object *)list);
  unref ((_Object *)one);
  unref ((_Object *)two);
  unref ((_Object *)three);
}
END_TEST

START_TEST (test_array_list_del_stress)
{
  _ArrayList *list = new (ArrayList, String);
    const static int SIZE = TESTS;
    char buf [5];
    memset (buf, '\0', sizeof (buf));

    for (int i = 0; i < SIZE; i++)
      {

        snprintf (buf, sizeof (buf), "%d", i);
        _String *str = new (String, buf);
        collection_add (list, str);

        ck_assert_int_eq (collection_size (list), i + 1);
        ck_assert (collection_contains (list, str));

        unref ((_Object *)str);
      }

    for (int i = SIZE-1; i >= 0; i--)
          {

            snprintf (buf, sizeof (buf), "%d", i);
            _String *str = new (String, buf);
            collection_del (list, str);

            ck_assert_int_eq (collection_size (list), i);
            ck_assert (!collection_contains (list, str));

            unref ((_Object *)str);
          }



    unref ((_Object *)list);
}
END_TEST

START_TEST(test_array_list_iterator)
{
  _ArrayList *list = new (ArrayList, String);
  const unsigned int SIZE = 10;
  char buf[3];

  for (int i = 0; i < SIZE; i++)
    {
      snprintf (buf, sizeof (buf), "%d", i);
      ck_assert (collection_add (list, new (String, buf)));
    }

  _Iterator *it = collection_iterator (list);
  for (int i = 0; i < SIZE; i++)
    {
      if (i < (SIZE - 1))
	ck_assert (iterator_has_next (it));

      snprintf (buf, sizeof (buf), "%d", i);
      _String *str = new (String, buf);
      _String *item = iterator_get_next (it);

      int size = snprintf (0, 0, "Wanted \"%s\", but got \"%s\"",
			   string_to_cstring (str),
			   string_to_cstring (item));
      char *msg = calloc (size + 1, sizeof (char));
      snprintf (msg, size, "Wanted \"%s\", but got \"%s\"",
		   string_to_cstring (str),
		   string_to_cstring (item));

      ck_assert_msg (equals (item, str), msg);
      unref ((_Object *)str);
    }

  unref ((_Object *)it);
  unref ((_Object *)list);
}
END_TEST

/*
int
main (void)
{
  initArrayList();
  int number_failed;

  Suite *s;
  SRunner *sr;
  TCase *tc_core;

  s = suite_create ("ArrayList");
  tc_core = tcase_create ("Core");

  tcase_add_test(tc_core, test_array_list_create);
  tcase_add_test(tc_core, test_array_list_add);
  tcase_add_test(tc_core, test_array_list_add_stress);
  tcase_add_test(tc_core, test_array_list_del);
  tcase_add_test(tc_core, test_array_list_del_stress);
  tcase_add_test(tc_core, test_array_list_iterator);

  suite_add_tcase (s, tc_core);

  sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}*/

TCase *
array_list_tcase ()
{
  TCase *tc_core = tcase_create ("ArrayList");

  tcase_add_test(tc_core, test_array_list_create);
    tcase_add_test(tc_core, test_array_list_add);
    tcase_add_test(tc_core, test_array_list_add_stress);
    tcase_add_test(tc_core, test_array_list_del);
    tcase_add_test(tc_core, test_array_list_del_stress);
    tcase_add_test(tc_core, test_array_list_iterator);

  return tc_core;
}
