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
#include "MandelbrotJob.h"
#include <algorithm>
#include <cstring>
#include "arith.h"

bool MandelbrotJob::fastpath(arith_t cx, arith_t cy, int &iterations, double &r2) {
  const arith_t cxq = (cx - 0.25);
  const arith_t cy2 = cy * cy;
  const arith_t q = cxq * cxq + cy2;

  bool fast = false;

  if(arith_t(4) * q * (q + cxq) < cy2) // Main cardioid
    fast = true;
  else if(cx * cx + arith_t(2) * cx + 1 + cy2 < arith_t(1) / arith_t(16)) // Period-2 bulb
    fast = true;

  if(fast) {
    iterations = maxiters;
    r2 = 0.0;
  }

  return fast;
}

bool MandelbrotJob::sisd_calculate(int px, int py) {
  // Complex-plane location of this point
  const arith_t cx = xleft + arith_t(px) * xsize / dest->width();
  const arith_t cy = ybottom + arith_t(dest->height() - 1 - py) * xsize / dest->width();
  // let c = cx + icy
  // let z = zx + izy
  //
  // then z^2 + c = zx^2 - zy^2 + cx + i(2zxzy+cy)
  int iterations = 0;
  double r2 = 0.0;
  if(!fastpath(cx, cy, iterations, r2)) {
    arith_t zx = 0, zy = 0;
    iterations = iterate(zx, zy, cx, cy, maxiters, arith, r2);
  }
  dest->pixel(px, py) = transform_iterations(iterations, r2, maxiters);
  return iterations != maxiters;
}

#if SIMD
bool MandelbrotJob::simd_calculate(int px[SIMD], int py[SIMD]) {
  const double zxvalues[SIMD] = {SIMD_REP(0)};
  const double zyvalues[SIMD] = {SIMD_REP(0)};
  double cxvalues[SIMD];
  double cyvalues[SIMD];
  for(int i = 0; i < SIMD; i++) {
    cxvalues[i] = (double)(xleft + arith_t(px[i]) * xsize / dest->width());
    cyvalues[i] = (double)(ybottom + arith_t(dest->height() - 1 - py[i]) * xsize / dest->width());
  }
  double r2values[SIMD];
  int iterations[SIMD];
  simd_iterate(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations, r2values, 1);
  bool escaped = false;
  for(int i = 0; i < SIMD; i++) {
    dest->pixel(px[i], py[i]) = transform_iterations(iterations[i], r2values[i], maxiters);
    escaped |= (iterations[i] != maxiters);
  }
  return escaped;
}
#endif

FractalJob *MandelbrotJobFactory::create() const {
  return new MandelbrotJob();
}

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
