#ifndef INDEX_H_
#define INDEX_H_

#include <j2c/indexed-file.h>
#include <j2c/logger.h>

typedef struct _J2cIndex J2cIndex;

J2cIndex *j2c_index_new (void);
void      j2c_index_delete (J2cIndex *self);
void      j2c_index_insert (J2cIndex *self, J2cIndexedFile *item);
void      j2c_index_insert_file (J2cIndex *self, J2cReadable *file);
void      j2c_index_log_contents (J2cIndex *self, J2cLoggerLevel level);

#endif /* INDEX_H_ */
