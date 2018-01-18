#include <j2c/jar-file-priv.h>
#include <j2c/indexed-file.h>
#include <j2c/zip-input-stream.h>
#include <j2c/jar-member.h>

#include <string.h>
#include <zip.h>

struct _J2cJarFile
{
  GObject parent_instance;

  GFile *file;
  GMutex *mutex;
  GWeakRef *zref;
};


/****
 * PRIV METHOD PROTOTYPES
 */

/* Destruction */
static void j2c_jar_file_dispose (GObject *object);

/* Initable Methods */
static gboolean j2c_jar_file_initable_init (GInitable *initable,
					      GCancellable *cancellable,
					      GError **error);

G_DEFINE_TYPE (J2cJarFile, j2c_jar_file, G_TYPE_OBJECT)

/* Error Code Quark */
GQuark
j2c_jar_file_quark (void)
{
  return g_quark_from_static_string ("j2c-jar-file");
}

/****
 * CONSTRUCTION METHODS
 */

J2cJarFile *
j2c_jar_file_new (GFile *file, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (file != NULL, NULL);
  GError *tmp_error = NULL;

  J2cZip *zip = j2c_zip_new (file, &tmp_error);
  if (tmp_error)
    {
      if (g_error_matches (tmp_error, J2C_ZIP_ERROR, J2C_ZIP_NOZIP_ERROR))
	{
	  g_set_error (error,
		       J2C_JAR_FILE_ERROR,
		       J2C_JAR_FILE_NOT_JAR_ERROR,
		       "This is not a jar file");
	  g_error_free (tmp_error);
	}
      else
	{
	  g_propagate_error (error, tmp_error);
	}
    }

  J2cJarFile *ret = g_object_new (J2C_TYPE_JAR_FILE, NULL);
  ret->file = g_object_ref (file);
  return ret;
}

static void
j2c_jar_file_class_init (J2cJarFileClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = j2c_jar_file_dispose;
}

static void
j2c_jar_file_init (J2cJarFile *self)
{
  self->mutex = g_malloc (sizeof (GMutex));
  self->zref = g_malloc (sizeof (GWeakRef));

  g_mutex_init (self->mutex);
  g_weak_ref_init (self->zref, NULL);
}

/****
 * DESTRUCTION METHODS
 */

static void
j2c_jar_file_dispose (GObject *object)
{
  J2cJarFile *self = J2C_JAR_FILE (object);

  if (self->mutex);
    g_mutex_lock (self->mutex);

  if (self->file)
    g_clear_object (&self->file);

  if (self->zref)
    {
      g_weak_ref_clear (self->zref);
      g_free (self->zref);
      self->zref = NULL;
    }

  if (self->mutex)
    {
      g_mutex_unlock (self->mutex);
      g_mutex_clear (self->mutex);
      g_free (self->mutex);
      self->mutex = NULL;
    }

  G_OBJECT_CLASS (j2c_jar_file_parent_class)->dispose (object);
}

/****
 * PUBLIC METHODS
 */

J2cReadable **
j2c_jar_file_expand (J2cJarFile *self, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (NULL != self, NULL);

  GError *tmp_error = NULL;
  J2cReadable **ret = NULL;
  J2cZip *zip = j2c_jar_file_open (self, &tmp_error);
  if (tmp_error)
    goto end;

  guint64 len = j2c_zip_num_entries (zip, &tmp_error);
  if (tmp_error)
    goto end;
  ret = g_malloc0 (sizeof (J2cReadable *) * (len + 1));

  gint index = 0;
  for (guint64 i = 0; i < len; i++)
    {
      gchar *name = j2c_zip_get_name (zip, i, &tmp_error);
      if (tmp_error)
	goto end;

      if (name[strlen(name) - 1] == '/')
	{
	  g_free (name);
	  continue;
	}

      J2cReadable *readable = J2C_READABLE (j2c_jar_member_new (self, i, name));
      g_free (name);
      ret[index] = readable;
      index ++;
    }

end:
  if (zip)
    g_object_unref (zip);
  if (tmp_error)
    g_propagate_error (error, tmp_error);
  return ret;
}

J2cZip *
j2c_jar_file_open (J2cJarFile *self, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->zref != NULL, NULL);

  J2cZip *ret = NULL;

  g_mutex_lock (self->mutex);

  ret = g_weak_ref_get (self->zref);
  if (!ret)
    {
      ret = j2c_zip_new (self->file, error);
      g_weak_ref_set (self->zref, ret);
    }

  g_mutex_unlock (self->mutex);

  return ret;
}
