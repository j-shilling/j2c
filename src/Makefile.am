SUBDIRS = logger \
			indexer

bin_PROGRAMS=j2c

j2c_SOURCES= main.c

j2c_CFLAGS = $(CFLAGS) $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS) \
				-I$(srcdir)/include
				
j2c_LDADD = $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS) $(ZIP_LIBS) \
			$(srcdir)/logger/liblogger.a
