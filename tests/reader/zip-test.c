#include <glib.h>
#include <gio/gio.h>

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

#include <j2c/zip.h>

static void
zip_test_zip_new (void)
{

}

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/zip/zip-new", zip_test_zip_new);

  return g_test_run();
}
