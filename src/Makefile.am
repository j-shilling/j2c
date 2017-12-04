bin_PROGRAMS=j2c

j2c_SOURCES= main.c \
	     logger/logger.c \
	     indexer/class-file.c

j2c_CFLAGS = $(CFLAGS) $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS) -I./include -I./indexer/include
j2c_LDADD = $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS)
