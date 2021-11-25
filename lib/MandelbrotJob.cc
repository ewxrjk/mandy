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
#include "mandy.h"
#include "MandelbrotJob.h"
#include <algorithm>
#include <cstring>
#include "arith.h"
#include "simdarith.h"

void MandelbrotJob::work() {
  // TODO SIMD support
  arith_type a;
  switch(arith) {
  case arith_simd2: simd2(); return;
  case arith_simd4: a = arith_double; break;
  default: a = arith;
  }
  // Compute the pixel limits
  const int lx = x + w, ly = y + h;
  // Iterate over rows
  for(int py = y; py < ly; ++py) {
    // Starting point for this row's results
    count_t *res = dest->data + py * dest->w + x;
    // Complex-plane location of this row
    const arith_t cy = ybottom + arith_t(dest->h - 1 - py) * xsize / dest->w;
    // Iterate over columns
    for(int px = x; px < lx; ++px) {
      // Complex-plane location of this column
      const arith_t cx = xleft + arith_t(px) * xsize / dest->w;
      // let c = cx + icy
      // let z = zx + izy
      //
      // then z^2 + c = zx^2 - zy^2 + cx + i(2zxzy+cy)
      count_t iterations = 0;
      arith_t zx = 0, zy = 0;
      // Optimizations as described in WP
      const arith_t cxq = (cx - 0.25);
      const arith_t cy2 = cy * cy;
      const arith_t q = cxq * cxq + cy2;
      if(arith_t(4) * q * (q + cxq) < cy2) { // Main cardioid
        iterations = maxiters;
        goto done;
      }
      if(cx * cx + arith_t(2) * cx + 1 + cy2
         < arith_t(1) / arith_t(16)) { // Period-2 bulb
        iterations = maxiters;
        goto done;
      }
      // TODO if the whole square is outside both regions, we could
      // skip these tests.
      iterations = iterate(zx, zy, cx, cy, maxiters, a);
    done:
      *res++ = iterations;
    }
  }
}

void MandelbrotJob::simd2() {
  // Compute the pixel limits
  const int lx = x + w, ly = y + h;
  // Iterate over rows
  for(int py = y; py < ly; ++py) {
    // Starting point for this row's results
    count_t *res = dest->data + py * dest->w + x;
    // Complex-plane location of this row
    const double cy =
        (ybottom + arith_t(dest->h - 1 - py) * xsize / dest->w).toDouble();
    // Iterate over columns
    for(int px = x; px < lx; px += 2) {
      // Complex-plane location of this column
      const double cx0 = (xleft + arith_t(px) * xsize / dest->w).toDouble();
      const double cx1 = (xleft + arith_t(px + 1) * xsize / dest->w).toDouble();
      const double zvalues[4] = {0, 0, 0, 0};
      const double cvalues[4] = {cx0, cy, cx1, cy};
      double r2values[2];
      int iterations[2];
      simd_iterate2(zvalues, cvalues, maxiters, iterations, r2values);
      *res++ = transform_iterations(iterations[0], r2values[0], maxiters);
      if(px + 1 < lx)
        *res++ = transform_iterations(iterations[1], r2values[1], maxiters);
    }
  }
}

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
