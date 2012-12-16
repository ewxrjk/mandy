/* Copyright © 2010 Richard Kettlewell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MANDY_H
#define MANDY_H

/* Portability guck */

#if HAVE_CONFIG_H
#include <config.h>
#endif
#if HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#if _WIN32
#include <windows.h>
#undef max
#undef min
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
#define inline __inline
#define M_PI 3.14159265358979323846
#define M_SQRT2 1.41421356237309504880
#define M_SQRT1_2 0.70710678118654752440
#define USE_GTHREADS 1
#define ARITH_TYPE double
#define ITER_TYPE double
#define NFIXED 4
#define ATOMIC_TYPE long
#define ATOMIC_INC(x) InterlockedIncrement(&(x))
#define ATOMIC_DEC(x) InterlockedDecrement(&(x))
#define strcasecmp _stricmp
#define log2(x) (log(x) * 1.44269504088896340737)
#define VERSION "0.0.WIP"
#define PATHSEP ';'
#define EXEEXT ".exe"
#define DIRSEP "\\"
#endif

#ifndef PATHSEP
# define PATHSEP ':'
#endif
#ifndef EXEEXT
# define EXEEXT ""
#endif
#ifndef DIRSEP
# define DIRSEP "/"
#endif

#if !HAVE_STRTOLD
# define strtold(s,e) (long double)strtod(s, e)
#endif

#ifndef ATOMIC_TYPE
#define ATOMIC_TYPE int
#endif

#if __GNUC__ && !defined ATOMIC_INC
#define ATOMIC_INC(x) __sync_add_and_fetch(&(x), 1)
#define ATOMIC_DEC(x) __sync_sub_and_fetch(&(x), 1)
#endif

#include "Fixed128.h"

typedef double count_t;

void fatal(int errno_value, const char *fmt, ...)
  attribute((format(printf,2,3)))
  attribute((noreturn));

#endif /* MANDY_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
