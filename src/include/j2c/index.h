#ifndef INDEX_H_
#define INDEX_H_

#include <gio/gio.h>
#include <j2c/indexed-file.h>

typedef struct _J2cIndex J2cIndex;

J2cIndex *j2c_index_new (void);
void      j2c_index_delete (J2cIndex *self);
void      j2c_index_insert (J2cIndex *self, J2cIndexedFile *item);
void      j2c_index_insert_file (J2cIndex *self, GFile *file);

#endif /* INDEX_H_ */
