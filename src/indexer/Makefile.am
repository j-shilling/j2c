SUBDIRS = class-file

noinst_LIBRARIES = libindexer.a

libindexer_a_SOURCES = 	index.c \
						indexed-file.c \
						class-file.c \
						constant-pool.c \
						resource-file.c
						
libindexer_a_CFLAGS = $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS) $(ZIP_CFLAGS) \
						-I$(top_srcdir)/src/include \
						-I$(srcdir)/include
