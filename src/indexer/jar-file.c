#include <j2c/jar-file.h>
#include <j2c/indexed-file.h>
#include <j2c/zip-input-stream.h>

#include <string.h>
#include <zip.h>

struct _J2cJarFile
{
  GObject parent_instance;
  GFile *file;
  gint64 entries;
};

struct _J2cJarMember
{
  GObject parent_instance;
  GFile *file;
  gchar *name;
  gint64 index;
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

G_DEFINE_TYPE_WITH_CODE (J2cJarMember, j2c_jar_member, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (J2C_TYPE_READABLE,
						j2c_jar_member_readable_interface_init))

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

static J2cJarMember *
j2c_jar_member_new (GFile *file, gint64 index)
{
  return g_object_new (J2C_TYPE_JAR_MEMBER,
		       J2C_JAR_FILE_PROPERTY_FILE, file,
		       J2C_JAR_FILE_PROPERTY_INDEX, index,
		       NULL);
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
j2c_jar_member_class_init (J2cJarMemberClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = j2c_jar_member_set_property;
  object_class->get_property = j2c_jar_member_get_property;
  object_class->dispose = j2c_jar_member_dispose;
  object_class->constructed = j2c_jar_member_constructed;

  g_object_class_install_property (object_class, PROP_FILE,
    g_param_spec_object (J2C_JAR_FILE_PROPERTY_FILE,
			 "file",
			 "Identifier for the local file",
			 G_TYPE_FILE,
			 G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  g_object_class_install_property (object_class, PROP_INDEX,
    g_param_spec_int64 (J2C_JAR_FILE_PROPERTY_INDEX,
			 "index",
			 "index within the jar file",
			 G_MININT64, G_MAXINT64, -1,
			 G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
j2c_jar_file_init (J2cJarFile *self)
{
  return;
}

static void
j2c_jar_member_init (J2cJarMember *self)
{
  return;
}

static void
j2c_jar_file_initable_interface_init (GInitableIface *iface)
{
  iface->init = j2c_jar_file_initable_init;
}

static void
j2c_jar_member_readable_interface_init (J2cReadableInterface *iface)
{
  iface->read = j2c_jar_member_read;
  iface->name = j2c_jar_member_name;
}

static void
j2c_jar_member_constructed (GObject *object)
{
  J2cJarMember *self = J2C_JAR_MEMBER (object);

  zip_t *zip = zip_open (g_file_get_path (self->file), ZIP_RDONLY, NULL);
  g_return_if_fail (NULL != zip);

  const gchar *name = zip_get_name (zip, self->index, ZIP_FL_ENC_GUESS);
  g_return_if_fail (NULL != name);

  self->name = g_strdup (name);
  zip_close (zip);
}

/****
 * DESTRUCTION METHODS
 */

static void
j2c_jar_file_dispose (GObject *object)
{
  J2cJarFile *self = J2C_JAR_FILE (object);

  g_clear_object (&self->file);

  G_OBJECT_CLASS (j2c_jar_file_parent_class)->dispose (object);
}

static void
j2c_jar_member_dispose (GObject *object)
{
  J2cJarMember *self = J2C_JAR_MEMBER (object);

  g_clear_object (&self->file);

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

static void
j2c_jar_member_set_property (GObject *object,
	     		     guint property_id,
			     const GValue *value,
			     GParamSpec *pspec)
{
  J2cJarMember *self = J2C_JAR_MEMBER (object);

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

    case PROP_INDEX:
      self->index = g_value_get_int64 (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
j2c_jar_member_get_property (GObject *object,
			      guint property_id,
			      GValue *value,
			      GParamSpec *pspec)
{
  J2cJarMember *self = J2C_JAR_MEMBER (object);

  switch (property_id)
    {
    case PROP_FILE:
      g_value_set_object (value, self->file);
      break;

    case PROP_INDEX:
      g_value_set_int64 (value, self->index);
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
      return FALSE;
    }

  self->entries = (gint64) zip_get_num_entries (zip, ZIP_FL_UNCHANGED);
  if (0 != zip_close (zip))
    {
      zip_error_t *zer = zip_get_error (zip);
      j2c_zip_input_stream_set_error (zer, path, error);
      return FALSE;
    }

  return TRUE;
}

/****
 * READABLE METHODS
 */

static GInputStream *
j2c_jar_member_read (J2cReadable *self, GError **error)
{
  J2cJarMember *jar = J2C_JAR_MEMBER (self);
  return G_INPUT_STREAM(j2c_zip_input_stream_open (jar->file, jar->index, error));
}

const gchar *
j2c_jar_member_name (J2cReadable *self)
{
  J2cJarMember *jar = J2C_JAR_MEMBER (self);
  return jar->name;
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
  for (guint64 i = 0; i < len; i++)
    {
      J2cReadable *readable = J2C_READABLE (j2c_jar_member_new (self->file, i));
      const gchar *name = j2c_readable_name (readable);

      if (name[strlen (name) - 1] == '/')
	{
	  g_object_unref (readable);
	}
      else
	{
	  ret[index] = readable;
	  index ++;
	}
    }

  return ret;
}
