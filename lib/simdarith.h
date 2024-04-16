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

#if __amd64__ || __i386__
# include <x86intrin.h>
#endif

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
#define NONZERO(v) _mm_testc_si128(v, ivector{REP(-1)})
#define COND_UPDATEV(dest, source, mask) ((dest) = _mm_blendv_pd((dest), (source), (vector)(mask)))
#define COND_UPDATEI(dest, source, mask) ((dest) = (ivector)_mm_blendv_pd((vector)(dest), (vector)(source), (vector)(mask)))
#else
#define NONZERO(v) v[0] & v[1]
#define COND_UPDATE(dest, source, mask) ((dest) |= ((source) & (mask)))
#endif
#define REP(v) v, v
#define VALUES(v) v[0], v[1]
#define ASSIGN(d, s)                                                                                                   \
  do {                                                                                                                 \
    d[0] = s[0];                                                                                                       \
    d[1] = s[1];                                                                                                       \
  } while(0)
#define vector simd2_vector
#define ivector simd2_ivector
#define escape_check simd2_escape_check

#include "simdimpl.h"

#undef NAME
#undef BYTES
#undef PTESTC
#undef REP
#undef VALUES
#undef NONZERO
#undef COND_UPDATEI
#undef COND_UPDATEV
#undef ASSIGN
#undef vector
#undef ivector
#undef escape_check
#endif

#if SIMD4
#define NAME simd_iterate4
#define BYTES 32
#if __AVX__
#define NONZERO(v) _mm256_testc_si256(v, ivector{REP(-1)})
#define COND_UPDATEV(dest, source, mask) ((dest) = _mm256_blendv_pd((dest), (source), (vector)(mask)))
#define COND_UPDATEI(dest, source, mask) ((dest) = (ivector)_mm256_blendv_pd((vector)(dest), (vector)(source), (vector)(mask)))
#else
#define NONZERO(v) v[0] & v[1] & v[2] & v[3]
#define COND_UPDATE(dest, source, mask) ((dest) |= ((source) & (mask)))
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
#define vector simd4_vector
#define ivector simd4_ivector
#define escape_check simd4_escape_check

#include "simdimpl.h"

#undef NAME
#undef BYTES
#undef PTESTC
#undef REP
#undef VALUES
#undef NONZERO
#undef COND_UPDATEI
#undef COND_UPDATEV
#undef ASSIGN
#undef vector
#undef ivector
#undef escape_check
#endif

static inline void simd_iterate(const double *zxvalues,
                                const double *zyvalues,
                                const double *cxvalues,
                                const double *cyvalues,
                                int maxiters,
                                int *iterations,
                                double *r2values,
                                int mandelbrot) {
#if SIMD4
  simd_iterate4(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations, r2values, mandelbrot);
#elif SIMD2
  simd_iterate2(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations, r2values, mandelbrot);
  simd_iterate2(zxvalues + 2, zyvalues + 2, cxvalues + 2, cyvalues + 2, maxiters, iterations + 2, r2values + 2, mandelbrot);
#endif
}

#endif /* SIMDARITH_H */
