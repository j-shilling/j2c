noinst_LIBRARIES = libcompilationunit.a

libcompilationunit_a_SOURCES = 	modified-utf8.c \
                                compilation-unit.c \
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
						
libcompilationunit_a_CFLAGS = $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS) \
						-I$(top_srcdir)/src/include \
						-I$(srcdir)/include
