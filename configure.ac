# Copyright © 2010 Richard Kettlewell.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
AC_INIT([mandy], [0.0.WIP], [rjk@greenend.org.uk])
AC_CONFIG_AUX_DIR([config.aux])
AM_INIT_AUTOMAKE([mandy], [0.0.WIP])
AC_CONFIG_SRCDIR([lib/MandelbrotJob.cc])
AM_CONFIG_HEADER([config.h])
AC_CANONICAL_HOST
AC_LANG([C++])
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AS
AC_PROG_RANLIB
AC_SET_MAKE
PKG_CHECK_MODULES([gtkmm], [gtkmm-2.4])
AC_DEFINE([_GNU_SOURCE], [1], [use GNU extensions])
if test "x$GXX" = xyes; then
  CXX="$CXX -Wall -W -Werror -Wpointer-arith -Wwrite-strings"
fi
if test "x$GCC" = xyes; then
  CC="$CC -Wall -W -Werror -Wpointer-arith -Wwrite-strings"
fi
arithtype=double
AC_MSG_CHECKING([what type to use for arithmetic])
AC_ARG_WITH([fixed-point],
            [AS_HELP_STRING([--with-fixed-point],
			    [use 128-bit fixed point arithmetic])],
            [arithtype=fixed])
AC_ARG_WITH([short-fixed-point],
            [AS_HELP_STRING([--with-short-fixed-point],
			    [use 64-bit fixed point arithmetic])],
            [arithtype=fixed;itertype=fixed64])
AC_ARG_WITH([long-double],
            [AS_HELP_STRING([--with-long-double],
			    [use long double for arithmetic])],
            [arithtype="long double"])
AC_ARG_WITH([assembler],
            [AS_HELP_STRING([--without-assembler],
			    [suppress assembler implementations])],
            [],
            [with_assembler=yes])
itertype=${itertype:-${arithtype}}
AC_MSG_RESULT([${arithtype}])
if test "x$with_assembler" != xno; then
  case $host_cpu in
  x86_64 )
    AC_DEFINE([HAVE_ASM], [1], [use assembly fixed point support])
    AC_DEFINE([HAVE_ASM_AMD64], [1], [use amd64 assembler])
    ;;
  esac
fi
AC_DEFINE_UNQUOTED([ARITH_TYPE],[${arithtype}],[arithmetic type])
AC_DEFINE_UNQUOTED([ITER_TYPE],[${itertype}],[iteration type])
AC_DEFINE([NFIXED],[4], [number of words in a fixed point integer])
AC_CONFIG_FILES([Makefile lib/Makefile images/Makefile gtkui/Makefile])
AC_OUTPUT
