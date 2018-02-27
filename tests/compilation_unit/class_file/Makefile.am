TESTS = constant_pool_test
check_PROGRAMS = constant_pool_test

BUILT_SOURCES = constant_pool_data
CLEANFILES = constant_pool_data

constant_pool_data: Makefile
	./constantpooldatagen.py

clean-local:
	-rm -rf constant_pool_data
	-rm -rf rt.jar

constant_pool_test_SOURCES = constant-pool-test.c
constant_pool_test_CFLAGS = $(GLIB_CFLAGS) $(GIO_CLFAGS) $(GOBJECT_CFLAGS) \
                    -I$(top_srcdir)/src/include \
                    -I$(top_srcdir)/src/compilation_unit/include \
                    -I$(top_srcdir)/src/compilation_unit/class_file/include
constant_pool_test_LDADD = $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS) \
                     $(top_srcdir)/src/compilation_unit/libcompilationunit.a \
		     $(top_srcdir)/src/compilation_unit/class_file/libclassfile.a
