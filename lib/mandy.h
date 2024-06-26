/* Copyright © Richard Kettlewell.
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

#include <config.h>
#include <inttypes.h>

#ifndef ATOMIC_TYPE
#define ATOMIC_TYPE int
#endif

// Select SIMD implementations
#if __amd64__
#if __AVX__
#define SIMD 4
#else
#define SIMD 2
#endif
#endif

#if __ARM_NEON
#define SIMD 2
#endif

#if SIMD == 2
#define SIMD_REP(n) n, n
#endif
#if SIMD == 4
#define SIMD_REP(n) n, n, n, n
#endif

#if SIMD
#define ARITH_DEFAULT arith_simd
#else
#define ARITH_DEFAULT arith_double
#endif

#if __GNUC__ && !defined ATOMIC_INC
#define ATOMIC_INC(x) __sync_add_and_fetch(&(x), 1)
#define ATOMIC_DEC(x) __sync_sub_and_fetch(&(x), 1)
#define ATOMIC_SET(x) __sync_or_and_fetch(&(x), 1)
#define ATOMIC_GET(x) __sync_fetch_and_or(&(x), 0)
#endif

#include "Fixed128.h"

typedef double count_t;

void fatal(int errno_value, const char *fmt, ...) attribute((format(printf, 2, 3))) attribute((noreturn));

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
