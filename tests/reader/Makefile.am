TESTS = zip_test \
        zip_input_stream_test \
	jar_file_test

check_PROGRAMS = zip_test \
                 zip_input_stream_test \
		 jar_file_test

zip_test_SOURCES = zip-test.c
zip_test_CFLAGS = $(GLIB_CFLAGS) $(GIO_CLFAGS) $(GOBJECT_CFLAGS) $(ZIP_CFLAGS) \
                  -I$(top_srcdir)/src/include \
		  -I$(top_srcdir)/src/reader/include
zip_test_LDADD = $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS) $(ZIP_LIBS) \
                 $(top_srcdir)/src/jre/libjre.a \
                 $(top_srcdir)/src/reader/libreader.a


zip_input_stream_test_SOURCES = zip-input-stream-test.c
zip_input_stream_test_CFLAGS = $(GLIB_CFLAGS) $(GIO_CLFAGS) $(GOBJECT_CFLAGS) $(ZIP_CFLAGS) \
                  -I$(top_srcdir)/src/include \
		  -I$(top_srcdir)/src/reader/include
zip_input_stream_test_LDADD = $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS) $(ZIP_LIBS) \
                 $(top_srcdir)/src/jre/libjre.a \
                 $(top_srcdir)/src/reader/libreader.a

jar_file_test_SOURCES = jar-file-test.c
jar_file_test_CFLAGS = $(GLIB_CFLAGS) $(GIO_CLFAGS) $(GOBJECT_CFLAGS) $(ZIP_CFLAGS) \
                  -I$(top_srcdir)/src/include \
		  -I$(top_srcdir)/src/reader/include
jar_file_test_LDADD = $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS) $(ZIP_LIBS) \
                 $(top_srcdir)/src/jre/libjre.a \
                 $(top_srcdir)/src/reader/libreader.a	 
