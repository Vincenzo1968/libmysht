#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])

AC_CONFIG_MACRO_DIR([m4])

#AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_INIT([libmysht], [1.0.0], [vincenzolocicero68@gmail.com], [libmysht], [https://vlcfreecode.netsons.org/])

AC_CONFIG_SRCDIR([src/myScopeHashTable.c])

AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC         # Find a C compiler
# AM_PROG_CC_C_O

# AC_PROG_CXX      # Find a C++ compiler
# AC_PROG_LEX      # Find flex/lex
# AC_PROG_YACC     # Find bison/yacc

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC

AM_INIT_AUTOMAKE([1.16 -Wall -Werror])
AM_PROG_AR
LT_INIT

AC_CONFIG_FILES([Makefile
                 src/Makefile])

AC_OUTPUT
