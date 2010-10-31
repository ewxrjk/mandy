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

void MandelbrotJob::work() {
  // Compute the pixel limits
  const int lx = params.x + params.w, ly = params.y + params.h;
  // Iterate over rows
  for(int py = params.y; py < ly; ++py) {
    // Starting point for this row's results
    int *res = params.dest->data + py * params.dest->w + params.x;
    // Complex-plane location of this row
    const double cy = params.ybottom + (params.dest->h - 1 - py) * params.xsize / params.dest->w;
    // Iterate over columns
    for(int px = params.x; px < lx; ++px) {
      // Complex-plane location of this column
      const double cx = params.xleft + px * params.xsize / params.dest->w;
      // let c = cx + icy
      // let z = zx + izy
      //
      // then z^2 + c = zx^2 - zy^2 + cx + i(2zxzy+cy)
      int iterations = 0;
      double zx = 0, zy = 0, zx2, zy2;
      while(((zx2 = zx * zx) + (zy2 = zy * zy) < 4.0)
	    && iterations < params.maxiters) {
	zy = 2 * zx * zy  + cy;
	zx = zx2 - zy2 + cx;
	++iterations;
      }
      *res++ = iterations;
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
