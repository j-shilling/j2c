#include <glib.h>
#include <gio/gio.h>

#include <locale.h>

#include <j2c/constant-pool.h>
#include <j2c/constant-pool-item.h>

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);

  return g_test_run();
}
