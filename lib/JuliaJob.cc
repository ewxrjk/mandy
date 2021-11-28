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
#include "JuliaJob.h"
#include <algorithm>
#include <cstring>
#include "arith.h"
#include "simdarith.h"

void JuliaJob::work() {
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
  const int lx = x + w, ly = y + h;
  for(int py = y; py < ly; ++py) {
    count_t *res = &dest->pixel(x, py);
    arith_t izy =
        ybottom + arith_t(dest->height() - 1 - py) * xsize / dest->width();
    for(int px = x; px < lx; ++px) {
      arith_t izx = xleft + arith_t(px) * xsize / dest->width();
      count_t iterations = 0;
      arith_t zx = izx, zy = izy;
      iterations = iterate(zx, zy, cx, cy, maxiters, a);
      *res++ = iterations;
    }
  }
}

#if SIMD2 || SIMD4
void JuliaJob::simd() {
  const int lx = x + w, ly = y + h;
  double cxd = cx.toDouble(), cyd = cy.toDouble();
  const double cxvalues[8] = {cxd, cxd, cxd, cxd};
  const double cyvalues[8] = {cyd, cyd, cyd, cyd};
  for(int py = y; py < ly; ++py) {
    count_t *res = &dest->pixel(x, py);
    const double izy =
        (ybottom + arith_t(dest->height() - 1 - py) * xsize / dest->width())
            .toDouble();
    for(int px = x; px < lx; px += 4) {
      const double izx0 =
          (xleft + arith_t(px) * xsize / dest->width()).toDouble();
      const double izx1 =
          (xleft + arith_t(px + 1) * xsize / dest->width()).toDouble();
      const double izx2 =
          (xleft + arith_t(px + 2) * xsize / dest->width()).toDouble();
      const double izx3 =
          (xleft + arith_t(px + 3) * xsize / dest->width()).toDouble();
      const double zxvalues[8] = {izx0, izx1, izx2, izx3};
      const double zyvalues[8] = {izy, izy, izy, izy};
      double r2values[4];
      int iterations[4];
      switch(arith) {
#if SIMD2
      case arith_simd2:
        simd_iterate2(zxvalues, zyvalues, cxvalues, cyvalues, maxiters,
                      iterations, r2values);
        simd_iterate2(zxvalues + 2, zyvalues + 2, cxvalues + 2, cyvalues + 2,
                      maxiters, iterations + 2, r2values + 2);
        break;
#endif
#if SIMD4
      case arith_simd4:
        simd_iterate4(zxvalues, zyvalues, cxvalues, cyvalues, maxiters,
                      iterations, r2values);
        break;
#endif
      default: throw std::logic_error("unhandled arith_type");
      }
      *res++ = transform_iterations(iterations[0], r2values[0], maxiters);
      *res++ = transform_iterations(iterations[1], r2values[1], maxiters);
      *res++ = transform_iterations(iterations[2], r2values[2], maxiters);
      *res++ = transform_iterations(iterations[3], r2values[3], maxiters);
    }
  }
}
#endif

FractalJob *JuliaJobFactory::create() const {
  return new JuliaJob(cx, cy);
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
