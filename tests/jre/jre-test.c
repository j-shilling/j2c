#include <glib.h>
#include <gio/gio.h>

#include <locale.h>

#include <j2c/jre.h>

static void
j2c_jre_test_exists (void)
{
  GFile *rt = j2c_jre_get_file ("rt.jar");
  g_assert (g_file_query_exists (rt, NULL));
  g_object_unref (rt);
}

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/jre/exists", j2c_jre_test_exists);

  return g_test_run();
}
