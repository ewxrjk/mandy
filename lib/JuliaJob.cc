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
#include "JuliaJob.h"
#include <algorithm>
#include <cstring>
#include "arith.h"

void JuliaJob::work() {
  // TODO SIMD support
  arith_type a;
  switch(arith) {
  case arith_simd2:
  case arith_simd4: a = arith_double; break;
  default: a = arith;
  }
  const int lx = x + w, ly = y + h;
  for(int py = y; py < ly; ++py) {
    count_t *res = dest->data + py * dest->w + x;
    arith_t izy = ybottom + arith_t(dest->h - 1 - py) * xsize / dest->w;
    for(int px = x; px < lx; ++px) {
      arith_t izx = xleft + arith_t(px) * xsize / dest->w;
      count_t iterations = 0;
      arith_t zx = izx, zy = izy;
      iterations = iterate(zx, zy, cx, cy, maxiters, a);
      *res++ = iterations;
    }
  }
}

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
