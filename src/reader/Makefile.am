noinst_LIBRARIES = libreader.a

libreader_a_SOURCES = reader.c \
		      readable.c \
		      jar-file.c \
		      zip-input-stream.c
libreader_a_CFLAGS = $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS) \
		     	-I$(top_srcdir)/src/include \
			-I$(srcdir)/include
