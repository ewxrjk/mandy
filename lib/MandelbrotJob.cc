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
#include "simdarith.h"
#include "PixelStream.h"

void MandelbrotJob::work() {
  arith_type a;
  switch(arith) {
#if SIMD2
  case arith_simd2: simd(); return;
#endif
#if SIMD4
  case arith_simd4: simd(); return;
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

#if SIMD2 || SIMD4
void MandelbrotJob::simd() {
  int px[4], py[4];

  PixelStreamRectangle pixels(x, y, w, h);
  while(pixels.morepixels(4, px, py))
    plot(px, py);
}

void MandelbrotJob::plot(int *px, int *py) {
  const double zxvalues[4] = {0, 0, 0, 0};
  const double zyvalues[4] = {0, 0, 0, 0};
  double cxvalues[4];
  double cyvalues[4];
  for(int i = 0; i < 4; i++) {
    cxvalues[i] = (xleft + arith_t(px[i]) * xsize / dest->width()).toDouble();
    cyvalues[i] =
        (ybottom + arith_t(dest->height() - 1 - py[i]) * xsize / dest->width())
            .toDouble();
  }
  double r2values[4];
  int iterations[4];
  simd_iterate(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations,
               r2values);
  for(int i = 0; i < 4; i++)
    dest->pixel(px[i], py[i]) =
        transform_iterations(iterations[i], r2values[i], maxiters);
}

inline void MandelbrotJob::simd_iterate(const double *zxvalues,
                                        const double *zyvalues,
                                        const double *cxvalues,
                                        const double *cyvalues, int maxiters,
                                        int *iterations, double *r2values) {
  switch(arith) {
#if SIMD2
  case arith_simd2:
    simd_iterate2(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations,
                  r2values);
    simd_iterate2(zxvalues + 2, zyvalues + 2, cxvalues + 2, cyvalues + 2,
                  maxiters, iterations + 2, r2values + 2);
    break;
#endif
#if SIMD4
  case arith_simd4:
    simd_iterate4(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations,
                  r2values);
    break;
#endif
  default: throw std::logic_error("unhandled arith_type");
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
