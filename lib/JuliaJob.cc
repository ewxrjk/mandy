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
  case arith_simd2: simd_work(); return;
#endif
#if SIMD4
  case arith_simd4: simd_work(); return;
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
bool JuliaJob::simd_calculate(int px[4], int py[4]) {
  const double cxd = (double)cx, cyd = (double)cy;
  double zxvalues[4];
  double zyvalues[4];
  const double cxvalues[4] = {cxd, cxd, cxd, cxd};
  const double cyvalues[4] = {cyd, cyd, cyd, cyd};
  for(int i = 0; i < 4; i++) {
    zxvalues[i] = (double)(xleft + arith_t(px[i]) * xsize / dest->width());
    zyvalues[i] =
        (double)(ybottom
                 + arith_t(dest->height() - 1 - py[i]) * xsize / dest->width());
  }
  double r2values[4];
  int iterations[4];
  simd_iterate(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations,
               r2values);
  bool escaped = false;
  for(int i = 0; i < 4; i++) {
    dest->pixel(px[i], py[i]) =
        transform_iterations(iterations[i], r2values[i], maxiters);
    escaped |= (iterations[i] != maxiters);
  }
  return escaped;
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
