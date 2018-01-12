#ifndef INDEX_H_
#define INDEX_H_

#include <glib-object.h>

#include <j2c/indexed-file.h>
#include <j2c/readable.h>
#include <j2c/logger.h>

G_BEGIN_DECLS

#define J2C_TYPE_INDEX        j2c_index_get_type()
G_DECLARE_FINAL_TYPE(J2cIndex, j2c_index, J2C, INDEX, GObject)

J2cIndex *j2c_index_new (void);
void      j2c_index_insert (J2cIndex *self, J2cIndexedFile *item, gboolean const target);
void      j2c_index_insert_file (J2cIndex *self, J2cReadable *file, gboolean const target);
J2cIndexedFile *j2c_index_get_main (J2cIndex *self);

G_END_DECLS

#endif /* INDEX_H_ */
