#include <j2c/zip.h>
#include <zip.h>

/****
  ERROR HANDLING
 ****/

static void
j2c_zip_set_error (zip_error_t *zer, GError **error)
{
  g_return_if_fail(error == NULL || *error == NULL);
  g_return_if_fail(NULL != zer);

  if (zip_error_code_zip (zer) == J2C_ZIP_OK)
    return;

  const gchar *msg = zip_error_strerror (zer);

  g_set_error (error,
	       J2C_ZIP_ERROR,
	       zip_error_code_zip (zer), "%s", msg);
}

static void
j2c_zip_set_error_from_code (gint zer, GError **error)
{
  g_return_if_fail(error == NULL || *error == NULL);

  if (zer == ZIP_ER_OK)
    return;

  zip_error_t *ze = g_malloc (sizeof(zip_error_t));
  zip_error_init_with_code (ze, zer);

  j2c_zip_set_error (ze, error);

  zip_error_fini (ze);
}

/****
  DEFINE TYPE J2C_ZIP
 ****/

struct _J2cZip
{
  GObject parent;

  zip_t *zip;
  GData *open_zip_files;
  GMutex *mutex;
};

G_DEFINE_TYPE (J2cZip, j2c_zip, G_TYPE_OBJECT)

static void
j2c_zip_dispose (GObject *object)
{
  J2cZip *self = J2C_ZIP (object);

  if (self->zip)
    j2c_zip_close (self, NULL);

  if (self->open_zip_files)
    {
      g_datalist_clear (&self->open_zip_files);
      self->open_zip_files = NULL;
    }

  if (self->zip)
    j2c_zip_close (self, NULL);

  if (self->mutex)
    {
      g_mutex_clear (self->mutex);
      g_free (self->mutex);
      self->mutex = NULL;
    }

  G_OBJECT_CLASS (j2c_zip_parent_class)->dispose (object);
}

static void
j2c_zip_class_init (J2cZipClass *klass)
{
  G_OBJECT_CLASS (klass)->dispose = j2c_zip_dispose;
}

static void
j2c_zip_init (J2cZip *self)
{
  self->zip = NULL;
  g_datalist_init (&self->open_zip_files);
  self->mutex = g_malloc (sizeof (GMutex));
  g_mutex_init (self->mutex);
}

/****
  DEFINE TYPE J2C_ZIP_FILE
 ****/

struct _J2cZipFile
{
  GObject parent;

  GQuark key;
  GWeakRef *zip_ref;
};

G_DEFINE_TYPE (J2cZipFile, j2c_zip_file, G_TYPE_OBJECT)

static void
j2c_zip_file_dispose (GObject *object)
{
  J2cZipFile *self = J2C_ZIP_FILE (object);

  if (self->zip_ref)
    {
      g_weak_ref_clear (self->zip_ref);
      g_free (self->zip_ref);
      self->zip_ref = NULL;
    }

  G_OBJECT_CLASS (j2c_zip_file_parent_class)->dispose (object);
}

static void
j2c_zip_file_class_init (J2cZipFileClass *klass)
{
  G_OBJECT_CLASS (klass)->dispose = j2c_zip_file_dispose;
}

static void
j2c_zip_file_init (J2cZipFile *self)
{
  self->zip_ref = g_malloc (sizeof (GWeakRef));
  g_weak_ref_init (self->zip_ref, NULL);
}

static void
j2c_zip_file_destroy_zip_file_t (gpointer data)
{
  zip_fclose ((zip_file_t *)data);
}


/****
  PUBLIC METHODS
 ****/

J2cZip *
j2c_zip_new (GFile *file, GError **error)
{
  g_return_val_if_fail (file != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  gchar *path = g_file_get_path (file);
  gint zer = ZIP_ER_OK;

  zip_t *zip = zip_open (path, ZIP_RDONLY, &zer);
  g_free (path);
  if (!zip)
    {
      j2c_zip_set_error_from_code (zer, error);
      return NULL;
    }

  J2cZip *ret = g_object_new (J2C_TYPE_ZIP, NULL);
  ret->zip = zip;

  return ret;
}

gchar *
j2c_zip_get_name (J2cZip *self, guint64 index, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (self != NULL, NULL);

  if (!self->zip)
    {
      g_set_error (error,
		   J2C_ZIP_ERROR,
		   J2C_ZIP_ZIPCLOSED_ERROR,
		   "This zip file is closed");
      return NULL;
    }

  const gchar *name = zip_get_name (self->zip, index, ZIP_FL_ENC_GUESS);
  if (!name)
    {
      zip_error_t *zer = zip_get_error (self->zip);
      j2c_zip_set_error (zer, error);
      return NULL;
    }

  return g_strdup (name);
}

gboolean
j2c_zip_close (J2cZip *self, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  g_return_val_if_fail (self != NULL, FALSE);
  
  gboolean ret = TRUE;

  g_mutex_lock (self->mutex);

  if (self->open_zip_files)
    {
      g_datalist_clear (&self->open_zip_files);
      self->open_zip_files = NULL;
    }

  if (self->zip)
    {
      if (0 != zip_close (self->zip))
	{
	  zip_error_t *zer = zip_get_error (self->zip);
	  j2c_zip_set_error (zer, error);
	  ret = FALSE;
	}
      else
	{
	  self->zip = NULL;
	}
    }

  g_mutex_unlock (self->mutex);

  return ret;
}

guint64
j2c_zip_num_entries (J2cZip *self, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, 0);
  g_return_val_if_fail (self != NULL, 0);
  g_return_val_if_fail (self->zip != NULL, 0);
 
  g_mutex_lock (self->mutex);
  guint64 ret = zip_get_num_entries (self->zip, ZIP_FL_UNCHANGED);
  g_mutex_unlock (self->mutex);
  return ret;
}

J2cZipFile *
j2c_zip_file_open (J2cZip *zip, guint64 index, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  g_return_val_if_fail (zip != NULL, NULL);

  J2cZipFile *ret = NULL;
  g_mutex_lock (zip->mutex);

  if (!zip->zip)
    {
      g_set_error (error,
		   J2C_ZIP_ERROR,
		   J2C_ZIP_ZIPCLOSED_ERROR,
		   "This zip file is closed");
    }


  zip_file_t *file = zip_fopen_index (zip->zip, index, ZIP_FL_UNCHANGED);
  if (!file)
    {
      zip_error_t *zer = zip_get_error (zip->zip);
      j2c_zip_set_error (zer, error);
      goto end;
    }

  GError *tmp_error = NULL;
  gchar *name = j2c_zip_get_name (zip, index, &tmp_error);
  if (tmp_error)
    {
      g_propagate_error (error, tmp_error);
      goto end;
    }

  ret = g_object_new (J2C_TYPE_ZIP_FILE, NULL);
  ret->key = g_quark_from_string (name);
  g_free (name);
  g_weak_ref_set (ret->zip_ref, zip);

  g_datalist_id_set_data_full (&zip->open_zip_files,
			       ret->key,
			       file,
                               j2c_zip_file_destroy_zip_file_t);

end:
  g_mutex_unlock (zip->mutex);
  return ret;
}

gssize
j2c_zip_file_read (J2cZipFile *self, void *buffer, gsize size, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, 0);
  g_return_val_if_fail (self != NULL, 0);
  g_return_val_if_fail (buffer != NULL, 0);

  J2cZip *zip = g_weak_ref_get (self->zip_ref);
  if (!zip)
    {
      g_set_error (error,
		   J2C_ZIP_ERROR,
		   J2C_ZIP_ZIPCLOSED_ERROR,
		   "This zip file is closed");
      return 0;
    }

  g_mutex_lock (zip->mutex);
  zip_file_t *file = g_datalist_id_get_data (&zip->open_zip_files, self->key);

  if (!file)
    {
      g_set_error (error,
		   J2C_ZIP_ERROR,
		   J2C_ZIP_ZIPCLOSED_ERROR,
		   "This zip file is closed");
      return 0;
    }

  gssize ret = zip_fread (file, buffer, size);
  if (ret != size)
    {
      zip_error_t *zer = zip_file_get_error (file);
      j2c_zip_set_error (zer, error);
    }
  g_mutex_unlock (zip->mutex);

  return ret;
}

gboolean
j2c_zip_file_close (J2cZipFile *self, GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  g_return_val_if_fail (self != NULL, FALSE);

  J2cZip *zip = g_weak_ref_get (self->zip_ref);
  if (!zip)
    {
      g_set_error (error,
		   J2C_ZIP_ERROR,
		   J2C_ZIP_ZIPCLOSED_ERROR,
		   "This zip file is closed");
      return 0;
    }

  
  g_mutex_lock (zip->mutex);
  zip_file_t *file = g_datalist_id_get_data (&zip->open_zip_files, self->key);
  gint zer = zip_fclose (file);
  if (zer != ZIP_ER_OK)
    j2c_zip_set_error_from_code (zer, error);

  g_datalist_id_remove_no_notify (&zip->open_zip_files, self->key);
  g_mutex_unlock (zip->mutex);
}
