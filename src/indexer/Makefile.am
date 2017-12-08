SUBDIRS = class-file

noinst_LIBRARIES = libindexer.a

libindexer_a_SOURCES = 	index.c \
						indexed-file.c \
						jar-file.c \
						readable.c \
						resource-file.c \
						zip-input-stream.c
						
libindexer_a_CFLAGS = $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS) $(ZIP_CFLAGS) \
						-I$(top_srcdir)/src/include \
						-I$(srcdir)/include