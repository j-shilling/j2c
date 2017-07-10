SUBDIRS = object util

AM_CFLAGS = -I$(srcdir)/object -I${srcdir}/util

bin_PROGRAMS=j2c
j2c_SOURCES= main.c

j2c_LDADD = object/libobject.a \
	    util/libutil.a
