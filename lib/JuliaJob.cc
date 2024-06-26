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

bool JuliaJob::sisd_calculate(int px, int py) {
  arith_t zx = xleft + arith_t(px) * xsize / dest->width();
  arith_t zy = ybottom + arith_t(dest->height() - 1 - py) * xsize / dest->width();
  double r2;
  int iterations = iterate(zx, zy, cx, cy, maxiters, arith, r2);
  dest->pixel(px, py) = transform_iterations(iterations, r2, maxiters);
  return iterations != maxiters;
}

#if SIMD
bool JuliaJob::simd_calculate(int px[SIMD], int py[SIMD]) {
  const double cxd = (double)cx, cyd = (double)cy;
  double zxvalues[SIMD];
  double zyvalues[SIMD];
  const double cxvalues[SIMD] = {SIMD_REP(cxd)};
  const double cyvalues[SIMD] = {SIMD_REP(cyd)};
  for(int i = 0; i < SIMD; i++) {
    zxvalues[i] = (double)(xleft + arith_t(px[i]) * xsize / dest->width());
    zyvalues[i] = (double)(ybottom + arith_t(dest->height() - 1 - py[i]) * xsize / dest->width());
  }
  double r2values[SIMD];
  int iterations[SIMD];
  simd_iterate(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations, r2values, 0);
  bool escaped = false;
  for(int i = 0; i < SIMD; i++) {
    dest->pixel(px[i], py[i]) = transform_iterations(iterations[i], r2values[i], maxiters);
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
