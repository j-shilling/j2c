#ifndef __READER_H__
#define __READER_H__

#include <glib.h>
#include <gio/gio.h>

typedef struct
{
  GSList *list;
  GMutex m;
} J2cReadableList;

J2cReadableList *j2c_readable_list_new (void);
void j2c_readable_list_destroy (J2cReadableList *self);
void j2c_readable_list_add (J2cReadableList *self, gchar const *const filename);

#endif /* __READER_H__ */
