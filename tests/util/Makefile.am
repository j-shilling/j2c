lib_LIBRARIES = libcheckutil.a

libcheckutil_a_SOURCES =  $(srcdir)/check_util.c \
							$(srcdir)/check_array_list.c

libcheckutil_a_CFLAGS = @CHECK_CFLAGS@ \
						-I$(srcdir)/../../src/object \
						-I$(srcdir)/../../src/util
						
libcheckutil_a_LIBADD =  $(srcdir)/../../src/object/libobject.a \
						 $(srcdir)/../../src/util/libutil.a