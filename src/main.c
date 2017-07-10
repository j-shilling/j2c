#include <stdio.h>

#include <Object.h>
#include <String.h>
#include <ArrayList.h>

int
main (int argc, char *argv[])
{
  initArrayList();

  _List *msgs = new (ArrayList, String);

  collection_add (msgs, new (String, "Hello, "));
  collection_add (msgs, new (String, "World!"));
  collection_add (msgs, new (String, "\n"));

  _Iterator *it = collection_iterator (msgs);
  while (iterator_has_next (it))
    printf ("%s", string_to_cstring (iterator_get_next (it)));

  unref ((_Object *)msgs);
  unref ((_Object *)it);

  return 0;
}
