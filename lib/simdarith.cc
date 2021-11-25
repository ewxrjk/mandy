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
#include "simdarith.h"
#include <stdint.h>
#include <cstring>

typedef double vector2 __attribute__((vector_size(16)));
typedef long long ivector2 __attribute__((vector_size(16)));

void simd_iterate2(double *zvalues, const double *cvalues, int maxiters,
                   int *iters) {
  const vector2 Cx = {cvalues[0], cvalues[2]};
  const vector2 Cy = {cvalues[1], cvalues[3]};
  vector2 Zx = {zvalues[0], zvalues[2]};
  vector2 Zy = {zvalues[1], zvalues[3]};
  ivector2 escape_iters = {0, 0}, escaped_already = {0, 0};
  int iterations = 0;
  while(iterations < maxiters) {
    vector2 Zx2 = Zx * Zx;
    vector2 Zy2 = Zy * Zy;
    vector2 r = Zx2 + Zy2;
    ivector2 escaped = r >= 64.0;
    ivector2 escaped_this_time = escaped & ~escaped_already;
    ivector2 iters_vector = {iterations, iterations};
    escape_iters |= iters_vector & escaped_this_time;
    escaped_already |= escaped;
#if __amd64__
    // xmm0=escaped
    // xmm7=(all 1s)
    //   vptest xmm0,xmm7
    //   jnc (continue loop)
    // sets CF <=> ~xmm0 & xmm7 = all 0s
    //         <=> ~xmm0 = all 0s
    //         <=> xmm0 all 1s
    // so escapes the loop if xmm0 is all 1s, which is what we want
    const ivector2 all_escaped = {-1, -1};
    if(__builtin_ia32_ptestc128(escaped, all_escaped))
      break;
#else
    // amd64: vmovq, vpextrq, test. (&& produces multiple test/branches.)
    // memcmp or casting to __int128 force escaped out to memory.
    if(escaped[0] & escaped[1])
      break;
#endif
    vector2 Zxnew = Zx2 - Zy2 + Cx;
    vector2 Zynew = 2 * Zx * Zy + Cy;
    Zx = escaped ? Zx : Zxnew;
    Zy = escaped ? Zy : Zynew;
    iterations++;
  }
  ivector2 maxiters_vector = {maxiters, maxiters};
  escape_iters |= maxiters_vector & ~escaped_already;
  zvalues[0] = Zx[0];
  zvalues[1] = Zy[0];
  zvalues[2] = Zx[1];
  zvalues[3] = Zy[1];
  iters[0] = escape_iters[0];
  iters[1] = escape_iters[1];
}

typedef double vector4 __attribute__((vector_size(32)));
typedef long long ivector4 __attribute__((vector_size(32)));

void simd_iterate4(double *zvalues, const double *cvalues, int maxiters,
                   int *iters) {
  const vector4 Cx = {cvalues[0], cvalues[2], cvalues[4], cvalues[6]};
  const vector4 Cy = {cvalues[1], cvalues[3], cvalues[5], cvalues[7]};
  vector4 Zx = {zvalues[0], zvalues[2], zvalues[4], zvalues[6]};
  vector4 Zy = {zvalues[1], zvalues[3], zvalues[5], zvalues[7]};
  ivector4 escape_iters = {0, 0, 0, 0}, escaped_already = {0, 0, 0, 0};
  int iterations = 0;
  while(iterations < maxiters) {
    vector4 Zx2 = Zx * Zx;
    vector4 Zy2 = Zy * Zy;
    vector4 r = Zx2 + Zy2;
    ivector4 escaped = r >= 64.0;
    ivector4 escaped_this_time = escaped & ~escaped_already;
    ivector4 iters_vector = {iterations, iterations, iterations, iterations};
    escape_iters |= iters_vector & escaped_this_time;
    escaped_already |= escaped;
#if __amd64__
    const ivector4 all_escaped = {-1, -1, -1, -1};
    if(__builtin_ia32_ptestc256(escaped, all_escaped))
      break;
#else
    if(escaped[0] & escaped[1] & escaped[2] & escaped[3])
      break;
#endif
    vector4 Zxnew = Zx2 - Zy2 + Cx;
    vector4 Zynew = 2 * Zx * Zy + Cy;
    Zx = escaped ? Zx : Zxnew;
    Zy = escaped ? Zy : Zynew;
    iterations++;
    // TODO stop if everything has escaped
  }
  ivector4 maxiters_vector = {maxiters, maxiters, maxiters, maxiters};
  escape_iters |= maxiters_vector & ~escaped_already;
  zvalues[0] = Zx[0];
  zvalues[1] = Zy[0];
  zvalues[2] = Zx[1];
  zvalues[3] = Zy[1];
  zvalues[4] = Zx[2];
  zvalues[5] = Zy[2];
  zvalues[6] = Zx[3];
  zvalues[7] = Zy[3];
  iters[0] = escape_iters[0];
  iters[1] = escape_iters[1];
  iters[2] = escape_iters[2];
  iters[3] = escape_iters[3];
}
