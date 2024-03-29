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
#ifndef SIMDARITH_H
#define SIMDARITH_H

#if SIMD2
#define NAME simd_iterate2
#define BYTES 16
#if __SSE4_1__
// xmm0=escaped
// xmm7=(all 1s)
//   vptest xmm0,xmm7
//   jnc (continue loop)
// sets CF <=> ~xmm0 & xmm7 = all 0s
//         <=> ~xmm0 = all 0s
//         <=> xmm0 all 1s
// so escapes the loop if xmm0 is all 1s, which is what we want
#define NONZERO(v) __builtin_ia32_ptestc128(v, ivector{REP(-1)})
#else
#define NONZERO(v) v[0] & v[1]
#endif
#define REP(v) v, v
#define VALUES(v) v[0], v[1]
#define ASSIGN(d, s)                                                                                                   \
  do {                                                                                                                 \
    d[0] = s[0];                                                                                                       \
    d[1] = s[1];                                                                                                       \
  } while(0)

#include "simdimpl.h"

#undef NAME
#undef BYTES
#undef PTESTC
#undef REP
#undef VALUES
#undef NONZERO
#undef ASSIGN
#endif

#if SIMD4
#define NAME simd_iterate4
#define BYTES 32
#if __AVX__
#define NONZERO(v) __builtin_ia32_ptestc256(v, ivector{REP(-1)})
#else
#define NONZERO(v) v[0] & v[1] & v[2] & v[3]
#endif
#define REP(v) v, v, v, v
#define VALUES(v) v[0], v[1], v[2], v[3]
#define ASSIGN(d, s)                                                                                                   \
  do {                                                                                                                 \
    d[0] = s[0];                                                                                                       \
    d[1] = s[1];                                                                                                       \
    d[2] = s[2];                                                                                                       \
    d[3] = s[3];                                                                                                       \
  } while(0)

#include "simdimpl.h"

#undef NAME
#undef BYTES
#undef PTESTC
#undef REP
#undef VALUES
#undef NONZERO
#undef ASSIGN
#endif

#endif /* SIMDARITH_H */
