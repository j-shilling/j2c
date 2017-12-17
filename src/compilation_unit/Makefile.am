noinst_LIBRARIES = libcompilationunit.a

libcompilationunit_a_SOURCES = 	compilation-unit.c \
				compilation-unit-class.c \
				constant-pool.c
						
libcompilationunit_a_CFLAGS = $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS) \
						-I$(top_srcdir)/src/include \
						-I$(srcdir)/include
