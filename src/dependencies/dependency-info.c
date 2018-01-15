#include <j2c/dependency-info.h>
#include <j2c/logger.h>
#include <j2c/descriptors.h>
#include <j2c/index.h>

#include <j2c/type-dependency.h>
#include <j2c/method-dependency.h>
#include <j2c/field-dependency.h>

#include <stdlib.h>

static struct
{
  GThreadPool *pool;
  GTree *tree;
  GMutex mutex;
} deps = { .pool = NULL, .tree = NULL };

typedef struct
{
  J2cMethod *method;
  gchar **param_types;
} J2cMethodCall;

typedef struct
{
  gchar tag;
  union
    {
      J2cMethodCall method_call;
      gchar *type_descriptor;
    };
} J2cAddDependencyInfo;

static J2cTypeDependency *
j2c_dependency_info_add_type (gchar *java_name)
{
  g_return_val_if_fail (java_name != NULL, NULL);
  g_return_val_if_fail (*java_name != '\0', NULL);

  if (g_tree_lookup (deps.tree, java_name))
    {
      j2c_logger_warning ("Cannot add type to the dependency tree more than once.");
      return g_tree_lookup (deps.tree, java_name);
    }

  J2cTypeDependency *value = j2c_type_dependency_new (java_name);
  J2cCompilationUnit *unit = j2c_index_get_compilation_unit (java_name);
  if (NULL == unit)
    {
      j2c_logger_fatal ("Cannot find information on type %s", java_name);
      exit (EXIT_FAILURE);
    }

  /* Add superclass */
  gchar *superclass = j2c_compilation_unit_superclass_name (unit);
  if (superclass)
    {
      J2cTypeDependency *super_dep = g_tree_lookup (deps.tree, superclass);
      if (!super_dep)
	super_dep = j2c_dependency_info_add_type (superclass);

      j2c_type_dependency_set_parent (value, super_dep);
      g_object_unref (super_dep);
      j2c_logger_fine ("%s is extended by %s", superclass, java_name);
    }

  /* Add interfaces */
  gchar **interfaces = j2c_compilation_unit_interfaces (unit);
  for (gchar **cur; cur && *cur; cur ++)
    {
      J2cTypeDependency *iface = g_tree_lookup (deps.tree, *cur);
      if (!iface)
	iface = j2c_dependency_info_add_type (*cur);
      j2c_type_dependency_add_interface (value, iface);
      j2c_logger_fine ("%s is extended by %s", *cur, java_name);
    }
  g_strfreev (interfaces);

  gchar *key = g_strdup (java_name);
  g_tree_insert (deps.tree, key, value);

  return value;
}

static J2cMethodDependency *
j2c_dependency_info_add_method (gchar *type, gchar *name, gchar *descriptor)
{
  g_return_val_if_fail (type != NULL, NULL);
  g_return_val_if_fail (name != NULL, NULL);
  g_return_val_if_fail (descriptor != NULL, NULL);

  J2cTypeDependency *type_dep = g_tree_lookup (deps.tree, type);
  if (!type_dep)
    type_dep = j2c_dependency_info_add_type (type);

  J2cMethodDependency *value = j2c_method_dependency_new (name, descriptor);
  j2c_type_dependency_add_method (type_dep, value);

  /* Find overrided method */
  J2cCompilationUnit *unit = j2c_index_get_compilation_unit (type);
  if (NULL == unit)
    {
      j2c_logger_fatal ("Cannot find information on type %s", type);
      exit (EXIT_FAILURE);
    }

  J2cMethod *supermethod = NULL;
  gchar *superclass = j2c_compilation_unit_superclass_name (unit);
  while (superclass)
    {
      J2cCompilationUnit *super = j2c_index_get_compilation_unit (superclass);
      supermethod = j2c_compilation_unit_get_method (super, name, descriptor);
      if (supermethod)
	break;

      g_free (superclass);
      superclass = j2c_compilation_unit_superclass_name (super);
      g_object_unref (super);
    }

  gchar **ifaces = j2c_compilation_unit_interfaces (unit);
  gint i = 0;
  while (!supermethod && ifaces && ifaces[i])
    {
      J2cCompilationUnit *iface = j2c_index_get_compilation_unit (ifaces[i]);
      supermethod = j2c_compilation_unit_get_method (iface, name, descriptor);
      if (supermethod)
	break;

      i ++;

      gchar **more_ifaces = j2c_compilation_unit_interfaces (iface);
      if (!more_ifaces)
	continue;

      guint size = g_strv_length (ifaces) + g_strv_length (more_ifaces);
      gchar **tmp = g_malloc0 (sizeof (gchar *) * (size + 1));
      gint index = 0;

      for (gchar **cur = ifaces; cur && *cur; cur ++)
	{
	  tmp[index] = *cur;
	  index++;
	}

      for (gchar **cur = more_ifaces; cur && *cur; cur ++)
	{
	  tmp[index] = *cur;
	  index++;
	}

      g_free (ifaces);
      g_free (more_ifaces);
      ifaces = tmp;
    }
  g_object_unref (unit);
  if (ifaces)
    g_strfreev (ifaces);

  if (supermethod)
    {
      gchar *type_name = j2c_method_get_defining_type (supermethod);
      g_object_unref (supermethod);

      J2cTypeDependency *super_type = g_tree_lookup (deps.tree, type_name);
      if (!super_type)
	super_type = j2c_dependency_info_add_type (type_name);

      J2cMethodDependency *dep = j2c_type_dependency_get_method (super_type, name, descriptor);
      if (!dep)
	dep = j2c_dependency_info_add_method (type_name, name, descriptor);

      j2c_method_dependency_add_overrider (dep, value);
      j2c_logger_fine ("%s::%s %s is overrided in %s", type_name, name, descriptor, type);
      g_free (type_name);
    }

  return value;
}

static void
j2c_dependency_info_add_dependency (gpointer data, gpointer user_data)
{
  J2cAddDependencyInfo *info = data;

  switch (info->tag)
    {
      case 'm':
	  {
	    J2cMethod *method = info->method_call.method;
	    gchar **param_types = info->method_call.param_types;

	    gchar *type = j2c_method_get_defining_type (method);
	    gchar *name = j2c_method_get_java_name (method);
	    gchar *descriptor = j2c_method_get_descriptor (method);

	    g_mutex_lock (&deps.mutex);

	    J2cTypeDependency *dep = g_tree_lookup (deps.tree, type);
	    if (!dep)
	      dep = j2c_dependency_info_add_type (type);

	    J2cMethodDependency *method_dep = j2c_type_dependency_get_method (dep, name, descriptor);
	    if (!method_dep)
	      method_dep = j2c_dependency_info_add_method (type, name, descriptor);

	    g_free (type);
	    g_free (name);
	    g_free (descriptor);

	    j2c_method_dependency_set_called (method_dep);

	    g_mutex_unlock (&deps.mutex);

	    if (param_types)
	      g_strfreev (param_types);
	  }
	break;
      case 'r':
	  {
	    gchar *type_descriptor = info->type_descriptor;
	    gchar *type = j2c_descriptor_get_reference_java_name (type_descriptor);
	    g_free (type_descriptor);

	    g_mutex_lock (&deps.mutex);
	    J2cTypeDependency *dep = g_tree_lookup (deps.tree, type);
	    if (!dep)
	      dep = j2c_dependency_info_add_type (type);
	    g_free (type);

	    j2c_type_dependency_set_referenced (dep);

	    g_mutex_unlock (&deps.mutex);
	  }
	break;
      default:
	break;
    }

  g_free (info);
}

static gint
j2c_dependency_info_key_compare (gconstpointer a, gconstpointer b, gpointer user_data)
{
  return g_strcmp0 ((const gchar *)a, (const gchar *)b);
}


static void
j2c_dependency_info_key_destroy (gpointer data)
{
  if (data)
    g_free (data);
}

static void
j2c_dependency_info_value_destroy (gpointer data)
{
  if (data)
    g_object_unref (data);
}

void
j2c_dependency_info_init (gint max_threads)
{
  GError *error = NULL;
  deps.pool = g_thread_pool_new (j2c_dependency_info_add_dependency,
				 NULL,
				 max_threads > 0 ? max_threads : -1,
				 TRUE,
				 &error);
  if (error)
    {
      j2c_logger_fatal ("%s", error->message);
      exit (EXIT_FAILURE);
    }

  deps.tree = g_tree_new_full (j2c_dependency_info_key_compare,
			       NULL,
			       j2c_dependency_info_key_destroy,
			       j2c_dependency_info_value_destroy);
  g_mutex_init (&deps.mutex);
}

void
j2c_dependency_info_add_method_call (J2cMethod *method, gchar **param_types)
{
  g_return_if_fail (deps.pool != NULL);
  g_return_if_fail (method != NULL);

  J2cAddDependencyInfo *info = g_malloc (sizeof (J2cAddDependencyInfo));
  info->tag = 'm';
  info->method_call.method = g_object_ref(method);
  info->method_call.param_types = g_strdupv(param_types);

  GError *error = NULL;
  g_thread_pool_push (deps.pool, info, &error);
  if (error)
    {
      j2c_logger_fatal ("%s", error->message);
      exit (EXIT_FAILURE);
    }
}

void
j2c_dependency_info_add_reference (gchar *type_descriptor)
{
  g_return_if_fail (deps.pool != NULL);
  g_return_if_fail (j2c_descriptor_is_reference_type (type_descriptor));

  J2cAddDependencyInfo *info = g_malloc (sizeof (J2cAddDependencyInfo));
  info->tag = 'r';
  info->type_descriptor = g_strdup (type_descriptor);

  GError *error = NULL;
  g_thread_pool_push (deps.pool, info, &error);
  if (error)
    {
      j2c_logger_fatal ("%s", error->message);
      exit (EXIT_FAILURE);
    }
}

void
j2c_dependency_info_lock (void)
{
  g_return_if_fail (deps.pool != NULL);

  g_thread_pool_free (deps.pool, FALSE, TRUE);
  deps.pool = NULL;
}
