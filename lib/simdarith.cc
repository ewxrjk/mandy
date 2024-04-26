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
#include "mandy.h"
#include "simdarith.h"

#if __amd64__ || __i386__
#include <x86intrin.h>
#endif

#if __ARM_NEON
#include <arm_neon.h>
#endif

/* Things we need to define:
 *
 * ivector                  Vector of 64-bit integers, representing counts or booleans
 *                          Booleans are all-0 for false and all-1 for true
 * vector                   Vector of 64-bit floats
 * NONZERO(ivector v)       Return nonzero if _all_ elements of v are true
 * COND_UPDATEV(vector d, vector s, ivector m)
 *                          For any true element of m, copy s to d; leave the rest of d unchanged
 * VALUES(v)                Elements of v; i.e. "v[0], v[1], ..."
 * ASSIGN(d, s)             Assign array-like things; i.e. "d[0]=s[0]; d[1]=s[1]; ..."
 */

#if SIMD == 2

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
#define COND_UPDATEI(dest, source, mask)                                                                               \
  ((dest) = (ivector)_mm_blendv_pd((vector)(dest), (vector)(source), (vector)(mask)))
#endif

#if __ARM_NEON
#define COND_UPDATEV(dest, source, mask)                                                                               \
  ((dest) = (vector)vbslq_f16((uint16x8_t)(mask), (float16x8_t)(source), (float16x8_t)(dest))) // Generates BSL or BIT
#endif

#ifndef NONZERO
#define NONZERO(v) (v[0] & v[1])
#endif

#define VALUES(v) v[0], v[1]
#define ASSIGN(d, s)                                                                                                   \
  do {                                                                                                                 \
    d[0] = s[0];                                                                                                       \
    d[1] = s[1];                                                                                                       \
  } while(0)

#endif

#if SIMD == 4

#if __AVX__
#define NONZERO(v) _mm256_testc_si256(v, ivector{SIMD_REP(-1)})
//#define NONZERO(v) !_mm256_testz_si256(ivector{REP(0)}, v) // Slower
#define COND_UPDATEV(dest, source, mask) ((dest) = _mm256_blendv_pd((dest), (source), (vector)(mask)))
#define COND_UPDATEI(dest, source, mask)                                                                               \
  ((dest) = (ivector)_mm256_blendv_pd((vector)(dest), (vector)(source), (vector)(mask)))
#endif

#ifndef NONZERO
#define NONZERO(v) (v[0] & v[1] & v[2] & v[3])
#endif

#define VALUES(v) v[0], v[1], v[2], v[3]
#define ASSIGN(d, s)                                                                                                   \
  do {                                                                                                                 \
    d[0] = s[0];                                                                                                       \
    d[1] = s[1];                                                                                                       \
    d[2] = s[2];                                                                                                       \
    d[3] = s[3];                                                                                                       \
  } while(0)
#endif

#ifndef COND_UPDATEI
#define COND_UPDATEI(dest, source, mask) ((dest) |= ((source) & (mask)))
#endif

#ifndef COND_UPDATEV
#define COND_UPDATEV(dest, source, mask) ((dest) = (vector)((ivector)(dest) | ((ivector)(source) & (mask))))
#endif

typedef double vector __attribute__((vector_size(8 * SIMD)));
typedef long long ivector __attribute__((vector_size(8 * SIMD)));

static inline bool escape_check(ivector &escaped_already,
                                ivector &escape_iters,
                                ivector escaped,
                                int64_t iterations,
                                vector r2,
                                vector &escape_r2) {
  ivector escaped_this_time = escaped & ~escaped_already;
  ivector iters_vector = {SIMD_REP(iterations)};
  escape_iters |= iters_vector & escaped_this_time;
  // COND_UPDATEI(escape_iters, iters_vector, escaped_this_time); // no - slightly slower
  escaped_already |= escaped;
  COND_UPDATEV(escape_r2, r2, escaped_this_time);
  return NONZERO(escaped_already);
}

static inline void simd_iterate_core(const double *zxvalues,
                                     const double *zyvalues,
                                     const double *cxvalues,
                                     const double *cyvalues,
                                     int64_t maxiters,
                                     int *iters,
                                     double *r2values,
                                     int mandelbrot) {
  const vector Cx = {VALUES(cxvalues)};
  const vector Cy = {VALUES(cyvalues)};
  vector Zx = {VALUES(zxvalues)};
  vector Zy = {VALUES(zyvalues)};
  vector r2 = {SIMD_REP(0)};
  vector escape_r2 = {0};
  ivector escape_iters = {SIMD_REP(0)};
  ivector escaped_already = {SIMD_REP(0)};
  int64_t iterations = 0;

  if(mandelbrot) {
    const vector cxq = (Cx - 0.25);
    const vector cy2 = Cy * Cy;
    const vector q = cxq * cxq + cy2;
    const ivector escaped = (4.0 * q * (q + cxq) < cy2) || (Cx * Cx + 2.0 * Cx + 1.0 + cy2 < 1.0 / 16.0);
    escape_check(escaped_already, escape_iters, escaped, maxiters, r2, escape_r2);
  }

  while(iterations < maxiters) {
    const vector Zx2 = Zx * Zx;
    const vector Zy2 = Zy * Zy;
    r2 = Zx2 + Zy2;
    const ivector escaped = r2 >= 64.0; // -1 for points that escaped this time, or in the past; else 0
    if(escape_check(escaped_already, escape_iters, escaped, iterations, r2, escape_r2))
      break;
    const vector Zxnew = Zx2 - Zy2 + Cx;
    const vector Zynew = 2 * Zx * Zy + Cy;
    Zx = Zxnew;
    Zy = Zynew;
    iterations++;
  }
  const ivector maxiters_vector = {SIMD_REP(maxiters)};
  escape_iters |= maxiters_vector & ~escaped_already;
  ASSIGN(r2values, escape_r2);
  ASSIGN(iters, escape_iters);
}

void simd_iterate(const double *zxvalues,
                  const double *zyvalues,
                  const double *cxvalues,
                  const double *cyvalues,
                  int maxiters,
                  int *iterations,
                  double *r2values,
                  int mandelbrot) {
  simd_iterate_core(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations, r2values, mandelbrot);
}
