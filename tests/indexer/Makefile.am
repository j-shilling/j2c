TESTS = indexer_test
check_PROGRAMS = indexer_test

indexer_test_SOURCES = indexer-test.c
indexer_test_CFLAGS = $(GLIB_CFLAGS) $(GIO_CLFAGS) $(GOBJECT_CFLAGS) \
                    -I$(top_srcdir)/src/include
indexer_test_LDADD = -lm $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS) $(ZIP_LIBS)\
                     $(top_srcdir)/src/indexer/libindexer.a \
                     $(top_srcdir)/src/compilation_unit/libcompilationunit.a \
                     $(top_srcdir)/src/compilation_unit/class_file/libclassfile.a \
                     $(top_srcdir)/src/compilation_unit/libcompilationunit.a \
                     $(top_srcdir)/src/reader/libreader.a \
                     $(top_srcdir)/src/util/libutil.a \
                     $(top_srcdir)/src/options/liboptions.a \
                     $(top_srcdir)/src/jre/libjre.a \
                     $(top_srcdir)/src/logger/liblogger.a

