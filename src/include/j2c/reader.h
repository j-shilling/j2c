#ifndef __READER_H__
#define __READER_H__

#include <glib.h>
#include <gio/gio.h>

typedef struct _J2cReadableList J2cReadableList;

J2cReadableList * j2c_readable_list_new (void);
GSList *j2c_readable_list_finish (J2cReadableList *list);
void j2c_readable_list_add (J2cReadableList *list, GFile *file);

#endif /* __READER_H__ */
