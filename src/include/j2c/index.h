#ifndef INDEX_H_
#define INDEX_H_

#include <glib.h>

#include <j2c/indexed-file.h>
#include <j2c/readable.h>
#include <j2c/compilation-unit.h>

void j2c_index_init (void);

J2cIndexedFile *j2c_index_get_main (void);
J2cCompilationUnit *j2c_index_get_compilation_unit (gchar *java_name);

#endif /* INDEX_H_ */
