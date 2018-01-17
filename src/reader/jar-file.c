#include <j2c/jar-file.h>
#include <j2c/indexed-file.h>
#include <j2c/zip-input-stream.h>
#include <j2c/jar-member.h>

#include <string.h>
#include <zip.h>

struct _J2cJarFile
{
  GObject parent_instance;

  GFile *file;
  gint64 entries;

  zip_t *zip;
  GMutex mutex;
  guint open_files;
};


/* structures related to object properties */
enum
{
  PROP_FILE = 1,
  PROP_INDEX = 2
};

static const gchar *const J2C_JAR_FILE_PROPERTY_FILE = "jar-file-file";
static const gchar *const J2C_JAR_FILE_PROPERTY_INDEX = "jar-file-index";

/****
 * PRIV METHOD PROTOTYPES
 */

/* Interfaces */
static void j2c_jar_member_readable_interface_init (J2cReadableInterface *iface);
static void j2c_jar_file_initable_interface_init (GInitableIface *iface);

static void j2c_jar_member_constructed (GObject *object);
/* Destruction */
static void j2c_jar_file_dispose (GObject *object);
static void j2c_jar_member_dispose (GObject *object);

/* Setters and Getters */
static void j2c_jar_file_set_property (GObject *object,
					 guint property_id,
					 const GValue *value,
					 GParamSpec *pspec);
static void j2c_jar_file_get_property (GObject *object,
					 guint property_id,
					 GValue *value,
					 GParamSpec *pspec);
static void j2c_jar_member_set_property (GObject *object,
					 guint property_id,
					 const GValue *value,
					 GParamSpec *pspec);
static void j2c_jar_member_get_property (GObject *object,
					 guint property_id,
					 GValue *value,
					 GParamSpec *pspec);
/* Initable Methods */
static gboolean j2c_jar_file_initable_init (GInitable *initable,
					      GCancellable *cancellable,
					      GError **error);

/* Readable Methods */
static GInputStream *j2c_jar_member_read (J2cReadable *self, GError **error);
const gchar *j2c_jar_member_name (J2cReadable *self);

G_DEFINE_TYPE_WITH_CODE (J2cJarFile, j2c_jar_file, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
						j2c_jar_file_initable_interface_init))

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
  GError *tmp_error = NULL;
  gpointer ret = g_initable_new (J2C_TYPE_JAR_FILE,
				 NULL,
				 &tmp_error,
				J2C_JAR_FILE_PROPERTY_FILE, file,
				NULL);

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
  return ret;
}

static void
j2c_jar_file_class_init (J2cJarFileClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = j2c_jar_file_set_property;
  object_class->get_property = j2c_jar_file_get_property;
  object_class->dispose = j2c_jar_file_dispose;

  g_object_class_install_property (object_class, PROP_FILE,
    g_param_spec_object (J2C_JAR_FILE_PROPERTY_FILE,
			 "file",
			 "Identifier for the local file",
			 G_TYPE_FILE,
			 G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}



static void
j2c_jar_file_init (J2cJarFile *self)
{
  g_mutex_init (&self->mutex);
}

static void
j2c_jar_file_initable_interface_init (GInitableIface *iface)
{
  iface->init = j2c_jar_file_initable_init;
}

/****
 * DESTRUCTION METHODS
 */

static void
j2c_jar_file_dispose (GObject *object)
{
  J2cJarFile *self = J2C_JAR_FILE (object);

  g_mutex_lock (&self->mutex);
  if (self->file)
    g_clear_object (&self->file);
  if (self->zip)
    j2c_jar_file_close (self, NULL);
  g_mutex_unlock (&self->mutex);
  g_mutex_clear (&self->mutex);

  G_OBJECT_CLASS (j2c_jar_file_parent_class)->dispose (object);
}

/****
 * SETTERS AND GETTERS
 */

static void
j2c_jar_file_set_property (GObject *object,
	     		     guint property_id,
			     const GValue *value,
			     GParamSpec *pspec)
{
  J2cJarFile *self = J2C_JAR_FILE (object);

  switch (property_id)
    {
    case PROP_FILE:
      if (self->file)
	{
	  g_warning ("%s can only be set once.", J2C_JAR_FILE_PROPERTY_FILE);
	  break;
	}

      self->file = g_value_dup_object (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_jar_file_get_property (GObject *object,
			   guint property_id,
			   GValue *value,
			   GParamSpec *pspec)
{
  J2cJarFile *self = J2C_JAR_FILE (object);

  switch (property_id)
    {
    case PROP_FILE:
      g_value_set_object (value, self->file);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

/****
 * INITABLE METHODS
 */

static gboolean
j2c_jar_file_initable_init (GInitable *initable, GCancellable *cancellable, GError **error)
{
  J2cJarFile *self = J2C_JAR_FILE (initable);
  gchar *path = g_file_get_path (self->file);

  int zerror = 0;
  zip_t *zip = zip_open (path, ZIP_RDONLY, &zerror);
  if (!zip)
    {
      j2c_zip_input_stream_set_error_from_code (zerror, path, error);
      g_free (path);
      return FALSE;
    }

  self->entries = (gint64) zip_get_num_entries (zip, ZIP_FL_UNCHANGED);
  if (0 != zip_close (zip))
    {
      zip_error_t *zer = zip_get_error (zip);
      j2c_zip_input_stream_set_error (zer, path, error);
      g_free (path);
      return FALSE;
    }

  g_free (path);
  return TRUE;
}

/****
 * PUBLIC METHODS
 */

J2cReadable **
j2c_jar_file_expand (J2cJarFile *self)
{
  g_return_val_if_fail (NULL != self, NULL);

  guint64 len = self->entries < 0 ? 0 : self->entries;
  J2cReadable **ret = g_malloc0 (sizeof (J2cReadable *) * (len + 1));

  gint index = 0;
  zip_t *zip = j2c_jar_file_open (self, NULL);
  for (guint64 i = 0; i < len; i++)
    {
      const gchar *name = zip_get_name (zip, i, ZIP_FL_ENC_GUESS);

      if (!name || name[strlen(name) - 1] == '/')
	continue;

      J2cReadable *readable = J2C_READABLE (j2c_jar_member_new (self, i, name));
      ret[index] = readable;
      index ++;
    }
  j2c_jar_file_close (self, NULL);

  return ret;
}

zip_t *
j2c_jar_file_open (J2cJarFile *self, GError **error)
{
  g_return_val_if_fail (self != NULL, NULL);

  g_mutex_lock (&self->mutex);
  if (!self->zip)
    {
      gchar *path = g_file_get_path (self->file);

      int zerror = 0;
      self->zip = zip_open (path, ZIP_RDONLY, &zerror);
      if (!self->zip)
	j2c_zip_input_stream_set_error_from_code (zerror, path, error);
      
      g_free (path);
    }

  self->open_files++;
  g_mutex_unlock (&self->mutex);

  return self->zip;
}

gboolean
j2c_jar_file_close (J2cJarFile *self, GError **error)
{
  g_return_val_if_fail (self != NULL, FALSE);

  g_mutex_lock (&self->mutex);
  self->open_files --;
  if (!self->open_files && self->zip)
    {
      if (0 != zip_close (self->zip))
	{
	  gchar *path = g_file_get_path (self->file);
	  zip_error_t *zer = zip_get_error (self->zip);
	  j2c_zip_input_stream_set_error (zer, path, error);
	  g_free (path);
	  g_mutex_unlock (&self->mutex);
	  return FALSE;
	}
      self->zip = NULL;
    }

  g_mutex_unlock (&self->mutex);
  return TRUE;
}
