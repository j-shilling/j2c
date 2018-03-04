TESTS = constant_pool_test
check_PROGRAMS = constant_pool_test

BUILT_SOURCES = datagen.jar test_data
DISTCLEANFILES = datagen.jar

datagen.jar: Makefile
	cd ./datagen; mvn package

test_data: Makefile
	java -jar datagen.jar ${srcdir}

distclean-local:
	-rm -rf test_data
	-rm -rf rt

constant_pool_test_SOURCES = constant-pool-test.c
constant_pool_test_CFLAGS = $(GLIB_CFLAGS) $(GIO_CLFAGS) $(GOBJECT_CFLAGS) \
                    -I$(top_srcdir)/src/include \
                    -I$(top_srcdir)/src/compilation_unit/include \
                    -I$(top_srcdir)/src/compilation_unit/class_file/include
constant_pool_test_LDADD = $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS) -lm \
                     $(top_srcdir)/src/compilation_unit/libcompilationunit.a \
		     $(top_srcdir)/src/compilation_unit/class_file/libclassfile.a \
                     $(top_srcdir)/src/util/libutil.a
