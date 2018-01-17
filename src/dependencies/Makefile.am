noinst_LIBRARIES = libdependencies.a

libdependencies_a_SOURCES = dependency-info.c \
                            type-dependency.c \
			    method-dependency.c \
			    field-dependency.c
						
libdependencies_a_CFLAGS = $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS) $(ZIP_CFLAGS) \
						-I$(top_srcdir)/src/include \
						-I$(srcdir)/include
