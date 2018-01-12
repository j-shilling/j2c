#include <j2c/dependency-info.h>
#include <j2c/logger.h>

struct _J2cDependencyInfo
{
  GObject parent;

  GTree *tree;
  GMutex mutex;
};

G_DEFINE_TYPE(J2cDependencyInfo, j2c_dependency_info, G_TYPE_OBJECT)

typedef struct
{
  gchar *java_name;
} J2cDependency;

static gint
j2c_dependency_info_key_compare (gconstpointer a, gconstpointer b, gpointer user_data)
{
  gchar *key1 = *(gchar **)a;
  gchar *key2 = *(gchar **)b;

  return g_strcmp0 (key1, key2);
}

static void
j2c_dependency_info_key_destory (gpointer data)
{
  if (data)
    g_free (data);
}

static void
j2c_dependency_info_data_destroy (gpointer data)
{
  J2cDependency *self = data;

  if (self)
    {
      if (self->java_name)
        g_free (self->java_name);
      g_free (self);
    }
}

static void
j2c_dependency_info_dispose (GObject *object)
{
  J2cDependencyInfo *self = J2C_DEPENDENCY_INFO (object);

  g_tree_destroy (self->tree);
  g_mutex_clear(&self->mutex);

  G_OBJECT_CLASS (j2c_dependency_info_parent_class)->dispose (object);
}

static void
j2c_dependency_info_class_init (J2cDependencyInfoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = j2c_dependency_info_dispose;
}

static void
j2c_dependency_info_init (J2cDependencyInfo *self)
{
  self->tree = g_tree_new_full (j2c_dependency_info_key_compare,
                                NULL,
                                j2c_dependency_info_key_destory,
                                j2c_dependency_info_data_destroy);
  g_mutex_init (&self->mutex);
}

static gboolean
j2c_dependency_info_log_node (gpointer key, gpointer value, gpointer data)
{
  gchar *java_name = key;
  J2cDependency *dep = value;
  J2cLoggerLevel level = *((J2cLoggerLevel *) data);

  j2c_logger_log(level, "Required type %s", java_name);
}

void
j2c_dependency_info_log_deps (J2cDependencyInfo *self, J2cLoggerLevel level)
{
  g_return_if_fail (self != NULL);
  g_mutex_lock (&self->mutex);
  g_tree_foreach (self->tree, j2c_dependency_info_log_node, &level);
  g_mutex_unlock (&self->mutex);
}

void
j2c_dependency_info_requires_type (J2cDependencyInfo *self, gchar *java_name)
{
  g_return_if_fail (self != NULL);
  g_return_if_fail (java_name != NULL);
  g_return_if_fail (java_name[0] != '\0');

  g_mutex_lock (&self->mutex);
  if (g_tree_lookup (self->tree, java_name) == NULL)
    {
      gchar *key = g_strdup (java_name);
      J2cDependency *data = g_malloc (sizeof (J2cDependency));
      data->java_name = g_strdup (java_name);

      g_tree_insert (self->tree, key, data);
    }
  g_mutex_unlock (&self->mutex);
}
