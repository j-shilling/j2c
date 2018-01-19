#include <glib.h>
#include <gio/gio.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <locale.h>
#include <j2c/index.h>

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/index/index-jre", j2c_index_init);

  return g_test_run();
}
