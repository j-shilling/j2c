#SUBDIRS = util object

TESTS = check_j2c
check_PROGRAMS = check_j2c

check_j2c_SOURCES = check_j2c.c \
		    $(srcdir)/object/check_reference_table.c \
		    $(srcdir)/object/check_object.c \
		    $(srcdir)/util/check_array_list.c \
		    $(srcdir)/util/check_util.c

check_j2c_CFLAGS = @CHECK_CFLAGS@ \
					-I$(srcdir)/util \
					-I$(srcdir)/object \
					-I$(srcdir)/../src/object \
					-I$(srcdir)/../src/util
					
check_j2c_LDADD = @CHECK_LIBS@ \
					$(srcdir)/../src/object/libobject.a \
					$(srcdir)/../src/util/libutil.a \
					$(srcdir)/util/libcheckutil.a \
					$(srcdir)/object/libcheckobject.a
