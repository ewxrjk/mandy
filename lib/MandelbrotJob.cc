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
  arith_type a;
  switch(arith) {
#if SIMD2
  case arith_simd2: simd2(); return;
#endif
#if SIMD4
  case arith_simd4: simd4(); return;
#endif
  default: a = arith;
  }
  // Compute the pixel limits
  const int lx = x + w, ly = y + h;
  // Iterate over rows
  for(int py = y; py < ly; ++py) {
    // Starting point for this row's results
    count_t *res = &dest->pixel(x, py);
    // Complex-plane location of this row
    const arith_t cy =
        ybottom + arith_t(dest->height() - 1 - py) * xsize / dest->width();
    // Iterate over columns
    for(int px = x; px < lx; ++px) {
      // Complex-plane location of this column
      const arith_t cx = xleft + arith_t(px) * xsize / dest->width();
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

#if SIMD2
void MandelbrotJob::simd2() {
  // Compute the pixel limits
  const int lx = x + w, ly = y + h;
  // Iterate over rows
  for(int py = y; py < ly; ++py) {
    // Starting point for this row's results
    count_t *res = &dest->pixel(x, py);
    // Complex-plane location of this row
    const double cy =
        (ybottom + arith_t(dest->height() - 1 - py) * xsize / dest->width())
            .toDouble();
    // Iterate over columns
    for(int px = x; px < lx; px += 2) {
      // Complex-plane location of this column
      const double cx0 =
          (xleft + arith_t(px) * xsize / dest->width()).toDouble();
      const double cx1 =
          (xleft + arith_t(px + 1) * xsize / dest->width()).toDouble();
      const double zvalues[4] = {0, 0, 0, 0};
      const double cvalues[4] = {cx0, cx1, cy, cy};
      double r2values[2];
      int iterations[2];
      simd_iterate2(zvalues, cvalues, maxiters, iterations, r2values);
      *res++ = transform_iterations(iterations[0], r2values[0], maxiters);
      *res++ = transform_iterations(iterations[1], r2values[1], maxiters);
    }
  }
}
#endif

#if SIMD4
void MandelbrotJob::simd4() {
  // Compute the pixel limits
  const int lx = x + w, ly = y + h;
  // Iterate over rows
  for(int py = y; py < ly; ++py) {
    // Starting point for this row's results
    count_t *res = &dest->pixel(x, py);
    // Complex-plane location of this row
    const double cy =
        (ybottom + arith_t(dest->height() - 1 - py) * xsize / dest->width())
            .toDouble();
    // Iterate over columns
    for(int px = x; px < lx; px += 4) {
      // Complex-plane location of this column
      const double cx0 =
          (xleft + arith_t(px) * xsize / dest->width()).toDouble();
      const double cx1 =
          (xleft + arith_t(px + 1) * xsize / dest->width()).toDouble();
      const double cx2 =
          (xleft + arith_t(px + 2) * xsize / dest->width()).toDouble();
      const double cx3 =
          (xleft + arith_t(px + 3) * xsize / dest->width()).toDouble();
      const double zvalues[8] = {0, 0, 0, 0, 0, 0, 0, 0};
      const double cvalues[8] = {cx0, cx1, cx2, cx3, cy, cy, cy, cy};
      double r2values[4];
      int iterations[4];
      simd_iterate4(zvalues, cvalues, maxiters, iterations, r2values);
      for(int i = 0; i < 4; i++)
        *res++ = transform_iterations(iterations[i], r2values[i], maxiters);
    }
  }
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
