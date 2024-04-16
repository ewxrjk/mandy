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

bool MandelbrotJob::sisd_calculate(int px, int py) {
  // Complex-plane location of this point
  const arith_t cx = xleft + arith_t(px) * xsize / dest->width();
  const arith_t cy = ybottom + arith_t(dest->height() - 1 - py) * xsize / dest->width();
  // let c = cx + icy
  // let z = zx + izy
  //
  // then z^2 + c = zx^2 - zy^2 + cx + i(2zxzy+cy)
  int iterations = 0;
  arith_t zx = 0, zy = 0;
  // Optimizations as described in WP
  const arith_t cxq = (cx - 0.25);
  const arith_t cy2 = cy * cy;
  const arith_t q = cxq * cxq + cy2;
  double r2 = 0.0;
  // TODO if the whole square is outside both regions, we could
  // skip these tests.
  if(arith_t(4) * q * (q + cxq) < cy2) { // Main cardioid
    iterations = maxiters;
  } else if(cx * cx + arith_t(2) * cx + 1 + cy2 < arith_t(1) / arith_t(16)) { // Period-2 bulb
    iterations = maxiters;
  } else
    iterations = iterate(zx, zy, cx, cy, maxiters, arith, r2);
  dest->pixel(px, py) = transform_iterations(iterations, r2, maxiters);
  return iterations != maxiters;
}

#if SIMD
bool MandelbrotJob::simd_calculate(int px[4], int py[4]) {
  const double zxvalues[4] = {0, 0, 0, 0};
  const double zyvalues[4] = {0, 0, 0, 0};
  double cxvalues[4];
  double cyvalues[4];
  for(int i = 0; i < 4; i++) {
    cxvalues[i] = (double)(xleft + arith_t(px[i]) * xsize / dest->width());
    cyvalues[i] = (double)(ybottom + arith_t(dest->height() - 1 - py[i]) * xsize / dest->width());
  }
  double r2values[4];
  int iterations[4];
  simd_iterate(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations, r2values);
  bool escaped = false;
  for(int i = 0; i < 4; i++) {
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
