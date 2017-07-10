lib_LIBRARIES = libcheckobject.a

libcheckobject_a_SOURCES =  $(srcdir)/check_object.c \
							$(srcdir)/check_reference_table.c

libcheckobject_a_CFLAGS = @CHECK_CFLAGS@ \
						-I$(srcdir)/../../src/object
						
libcheckobject_a_LIBADD = $(srcdir)/../../src/object/libobject.a