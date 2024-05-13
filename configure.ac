# Copyright © Richard Kettlewell.
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
AC_INIT([mandy],[0.0.WIP],[rjk@greenend.org.uk])
AC_CONFIG_AUX_DIR([config.aux])
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_SRCDIR([lib/MandelbrotJob.cc])
AM_CONFIG_HEADER([config.h])
AC_CANONICAL_HOST
AC_LANG([C++])
AC_PROG_CXX
AM_PROG_AS
AC_PROG_RANLIB
AM_PROG_AR
AC_PROG_MAKE_SET
PKG_CHECK_MODULES([gtkmm], [gtkmm-2.4])
PKG_CHECK_MODULES([gdkpixbuf], [gdk-pixbuf-2.0])
AC_CHECK_LIB([fcgi],[FCGX_Init],
             [AC_SUBST([FCGI_LIBS],[-lfcgi])
              AC_SUBST([MFCGI],[mandy.fcgi])])
AC_CHECK_FUNCS([sysconf])
AC_DEFINE([_GNU_SOURCE], [1], [use GNU extensions])
AX_CHECK_COMPILE_FLAG([-march=native],[CXXFLAGS="-march=native $CXXFLAGS"])
if test "x$GXX" = xyes; then
  CXXFLAGS="-Wall -W -Werror -Wpointer-arith -Wwrite-strings $CXXFLAGS"
  CXXFLAGS="-std=c++11 -Wno-c++14-extensions -Wno-deprecated-declarations $CXXFLAGS"
  compiler="$($CXX --version)"
  case "$compiler" in
  *clang* )
    ;;
  * )
    CXXFLAGS="-Wno-deprecated-copy $CXXFLAGS"
    ;;
  esac
fi
CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_EXTRA}"
AC_PROG_CC
AC_LANG([C])
AX_CHECK_COMPILE_FLAG([-march=native],[CFLAGS="-march=native $CFLAGS"])
if test "x$GCC" = xyes; then
  CFLAGS="-Wall -W -Werror -Wpointer-arith -Wwrite-strings $CFLAGS"
fi
CFLAGS="${CFLAGS} ${CFLAGS_EXTRA}"

# sigc++ is broken, but the compiler can be persuaded to ignore it.
# https://bugzilla.redhat.com/show_bug.cgi?id=1576299
# Annoyingly, Clang and GCC use different options for this.
AC_CACHE_CHECK([whether sigc++ is broken],
  [rjk_cv_sigcpp_broken],
  [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <sigc++/sigc++.h>]])],
     [rjk_cv_sigcpp_broken=no],
     [rjk_cv_sigcpp_broken=yes])
])
if test $rjk_cv_sigcpp_broken = yes; then
  # https://stackoverflow.com/questions/30752966/how-to-set-compiler-specific-flags-with-autotools
  if `$CXX -v 2>&1 | grep 'gcc version' >/dev/null 2>&1`; then
    CXXFLAGS="${CXXFLAGS} -Wno-cast-function-type"
  else
    CXXFLAGS="${CXXFLAGS} -Wno-bad-function-cast"
  fi
fi

AH_BOTTOM([#ifdef __GNUC__
# define attribute(x) __attribute__(x)
#else
# define attribute(x)
#endif

// Select assembler implementation
#if __amd64__
# define HAVE_ASM_FIXED128_ITERATE 1
# define HAVE_ASM_FIXED256_MUL 1
# define HAVE_ASM_FIXED64_ITERATE 1
#endif

#if __aarch64__
# define HAVE_ASM_FIXED256_MUL 1
#endif

#define R2LIMIT 4 // Escape value
])

AC_CONFIG_FILES([Makefile lib/Makefile images/Makefile web/Makefile gtkui/Makefile])
AC_OUTPUT
