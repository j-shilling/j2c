#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([j2c],[0.1],[shilling.jake@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([src/include/config.h])

AC_ARG_VAR([JAVA_HOME], [Path to JRE directory])
AC_DEFINE_UNQUOTED([JAVA_HOME], ["$JAVA_HOME"], [Path to JRE directory])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR

# Checks for libraries.
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GIO], [gio-2.0])
PKG_CHECK_MODULES([GOBJECT], [gobject-2.0])
PKG_CHECK_MODULES([ZIP], [libzip])

AC_CONFIG_FILES([Makefile
                 src/Makefile
		 src/options/Makefile
		 src/jre/Makefile
		 src/util/Makefile
                 src/logger/Makefile
		 src/reader/Makefile
                 src/indexer/Makefile
		 src/dependencies/Makefile
		 src/compilation_unit/Makefile
		 src/compilation_unit/class_file/Makefile
		 tests/Makefile
		 tests/jre/Makefile
		 tests/reader/Makefile
		 tests/compilation_unit/Makefile
		 tests/compilation_unit/class_file/Makefile])
AC_OUTPUT
