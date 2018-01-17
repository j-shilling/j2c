#include <config.h>
#include <j2c/jre.h>

#include <glib-object.h>

static GWeakRef weak_ref;

static const gchar *
j2c_jre_get_jre_path (void)
{
  static gchar *path = NULL;
  if (!path)
    {
      GFile *file = g_file_new_for_path (JAVA_HOME "/jre");
      if (g_file_query_exists (file, NULL))
	path = JAVA_HOME "/jre";
      else
	path = JAVA_HOME;
    }
  return path;
}

GFile *
j2c_jre_get_lib_dir (void)
{
  static GWeakRef *ref = NULL;
  if (!ref)
    {
      ref = &weak_ref;
      g_weak_ref_init (ref, NULL);
    }

  GFile *ret = g_weak_ref_get (ref);
  if (!ret)
    {
      ret = g_file_new_for_path (j2c_jre_get_jre_path ());
      g_weak_ref_set (ref, ret);
    }

  return ret;
}


GFile *
j2c_jre_get_file (const gchar *name)
{
  GFile *lib = j2c_jre_get_lib_dir();
  GFile *ret = g_file_get_child (lib, name);
  g_object_unref (lib);

  return ret;
}
