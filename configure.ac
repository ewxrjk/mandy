# Copyright © 2010, 2015 Richard Kettlewell.
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
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([lib/MandelbrotJob.cc])
AM_CONFIG_HEADER([config.h])
AC_CANONICAL_HOST
AC_LANG([C++])
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AS
AC_PROG_RANLIB
AM_PROG_AR
AC_SET_MAKE
PKG_CHECK_MODULES([gtkmm], [gtkmm-2.4])
PKG_CHECK_MODULES([gdkpixbuf], [gdk-pixbuf-2.0])
AC_CHECK_LIB([fcgi],[FCGX_Init],
             [AC_SUBST([FCGI_LIBS],[-lfcgi])
              AC_SUBST([MFCGI],[mandy.fcgi])])
AC_CHECK_HEADERS([unistd.h inttypes.h getopt.h])
AC_CHECK_FUNCS([sysconf strtold])
AC_CHECK_TYPES([__int128])
AC_DEFINE([_GNU_SOURCE], [1], [use GNU extensions])
if test "x$GXX" = xyes; then
  CXX="$CXX -Wall -W -Werror -Wpointer-arith -Wwrite-strings"
fi
if test "x$GCC" = xyes; then
  CC="$CC -Wall -W -Werror -Wpointer-arith -Wwrite-strings"
fi

# G++ 5.x and Clang now both default to the C++ABI (even if you ask
# for -std=c++03), but the libraries haven't caught up, so by default
# ask for the old ABI.
AC_ARG_WITH([cxx11],[AS_HELP_STRING([--with-cxx11],[enable C++11 ABI])],
            [],[with_cxx11=no])
if test "$with_cxx11" = no; then
  CPPFLAGS="${CPPFLAGS} -D_GLIBCXX_USE_CXX11_ABI=0"
fi

AC_DEFINE_UNQUOTED([ARITH_DEFAULT],[arith_double],[default precision])

#
# Assembler support
#
AC_ARG_WITH([assembler],
            [AS_HELP_STRING([--without-assembler],
			    [suppress assembler implementations])],
            [],
            [with_assembler=yes])
if test "x$with_assembler" != xno; then
  AC_MSG_CHECKING([for assembler support])
  case $host_cpu in
  x86_64 )
    AC_DEFINE([HAVE_ASM], [1], [use assembly fixed point support])
    AC_DEFINE([HAVE_ASM_AMD64], [1], [use amd64 assembler])
    AC_MSG_RESULT([x86-64])
    ;;
  * )
    AC_MSG_RESULT([none])
    ;;
  esac
fi

#
# Choose a threading API
# If you have pthreads there is no particular advantage to using glib
# threads.  The intent is improve support for non-POSIX platforms.
#
AC_MSG_CHECKING([what thread API to use])
threading=pthreads
AC_ARG_WITH([gthreads],
	    [AS_HELP_STRING([--with-gthreads],
			    [use glib thread support])],
	    [threading=glib])
AC_MSG_RESULT([${threading}])
case ${threading} in
pthreads )
  AC_DEFINE([USE_PTHREADS],[1],[define to use pthreads])
  ;;
glib )
  AC_DEFINE([USE_GTHREADS],[1],[define to use glib threads])
  PKG_CHECK_MODULES([gthread], [gthread-2.0])
  ;;
esac

AH_BOTTOM([#ifdef __GNUC__
# define attribute(x) __attribute__(x)
#else
# define attribute(x)
#endif])

AC_CONFIG_FILES([Makefile lib/Makefile images/Makefile web/Makefile gtkui/Makefile])
AC_OUTPUT
