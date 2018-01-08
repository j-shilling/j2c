noinst_LIBRARIES = libclassfile.a

libclassfile_a_SOURCES = 	modified-utf8.c \
				compilation-unit-class.c \
				constant-pool.c \
				attributes.c \
				attributes-class-file.c \
				attributes-field-info.c \
				attributes-method-info.c \
				attributes-code.c \
				attributes-class-field-or-method.c \
				attributes-any.c \
				method-or-field-info.c \
				opcodes.c \
				byte-code-method.c
						
libclassfile_a_CFLAGS = $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS) \
						-I$(top_srcdir)/src/include \
						-I$(srcdir)/../include \
						-I$(srcdir)/include
