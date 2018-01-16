#ifndef __READER_H__
#define __READER_H__

#include <glib.h>
#include <gio/gio.h>

void j2c_readable_list_init (gint max_threads);
GSList *j2c_readable_list_finish (void);
void j2c_readable_list_add (GFile *file);

#endif /* __READER_H__ */
