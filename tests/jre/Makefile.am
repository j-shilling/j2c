TESTS = jre_test
check_PROGRAMS = jre_test

jre_test_SOURCES = jre-test.c
jre_test_CFLAGS = $(GLIB_CFLAGS) $(GIO_CLFAGS) $(GOBJECT_CFLAGS) \
                    -I$(top_srcdir)/src/include
jre_test_LDADD = $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS) \
                     $(top_srcdir)/src/jre/libjre.a
