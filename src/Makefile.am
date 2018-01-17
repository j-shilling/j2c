SUBDIRS = logger \
          jre \
          util \
	  reader \
          indexer \
	  compilation_unit \
	  dependencies

bin_PROGRAMS=j2c

j2c_SOURCES= main.c

j2c_CFLAGS = $(CFLAGS) $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS) \
				-I$(srcdir)/include
				
j2c_LDADD = -lm $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS) $(ZIP_LIBS) \
			$(srcdir)/jre/libjre.a \
			$(srcdir)/logger/liblogger.a \
			$(srcdir)/reader/libreader.a \
			$(srcdir)/indexer/libindexer.a \
			$(srcdir)/compilation_unit/libcompilationunit.a \
			$(srcdir)/compilation_unit/class_file/libclassfile.a \
			$(srcdir)/compilation_unit/libcompilationunit.a \
			$(srcdir)/util/libutil.a \
			$(srcdir)/dependencies/libdependencies.a
