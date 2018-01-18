#include <j2c/jar-member.h>
#include <j2c/zip-input-stream.h>
#include <j2c/jar-file-priv.h>

struct _J2cJarMember
{
  GObject parent;

  J2cJarFile *source;
  gint64 index;
  gchar *name;
};

static void j2c_jar_member_readable_interface_init (J2cReadableInterface *iface);

G_DEFINE_TYPE_WITH_CODE (J2cJarMember, j2c_jar_member, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (J2C_TYPE_READABLE,
						j2c_jar_member_readable_interface_init))

static GInputStream *
j2c_jar_member_read (J2cReadable *self, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  J2cJarMember *jar = J2C_JAR_MEMBER (self);
  g_return_val_if_fail (jar->source != NULL, NULL);

  GError *tmp_error = NULL;
  J2cZip *zip = j2c_jar_file_open (jar->source, &tmp_error);
  if (tmp_error)
    {
      g_propagate_error (error, tmp_error);
      return NULL;
    }

  J2cZipInputStream *ret = j2c_zip_input_stream_open (zip, jar->index, &tmp_error);
  if (tmp_error)
    {
      g_propagate_error (error, tmp_error);
      return NULL;
    }
  
  g_object_unref (zip);
  return G_INPUT_STREAM (ret);
}

const gchar *
j2c_jar_member_name (J2cReadable *self)
{
  J2cJarMember *jar = J2C_JAR_MEMBER (self);
  return jar->name;
}

static void
j2c_jar_member_dispose (GObject *object)
{
  J2cJarMember *self = J2C_JAR_MEMBER (object);

  if (self->source)
    g_clear_object (&self->source);
  if (self->name)
    {
      g_free (self->name);
      self->name = NULL;
    }

  G_OBJECT_CLASS (j2c_jar_member_parent_class)->dispose (object);
}

static void
j2c_jar_member_readable_interface_init (J2cReadableInterface *iface)
{
  iface->read = j2c_jar_member_read;
  iface->name = j2c_jar_member_name;
}

static void
j2c_jar_member_class_init (J2cJarMemberClass *klass)
{
  G_OBJECT_CLASS (klass)->dispose = j2c_jar_member_dispose;
}

static void
j2c_jar_member_init (J2cJarMember *self)
{
  return;
}

J2cJarMember *
j2c_jar_member_new (J2cJarFile *file, guint index, const gchar *jar_name)
{
  J2cJarMember *ret = g_object_new (J2C_TYPE_JAR_MEMBER, NULL);
  ret->source = g_object_ref (file);
  ret->index = index;
  ret->name = g_strdup (jar_name);

  return ret;
}
