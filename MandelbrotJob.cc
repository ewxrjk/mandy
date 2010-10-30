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

MandelbrotJob::MandelbrotJob(int x_, int y_,
			     int w_, int h_,
			     double cx_, double cy_,
			     double cr_,
			     int maxiters_,
			     IterBuffer *dest_):
  dest(dest_),
  x(x_), y(y_),
  w(w_), h(h_),
  xcentre(cx_), ycentre(cy_), radius(cr_),
  maxiters(maxiters_) {
  dest->acquire();
}

void MandelbrotJob::work() {
  // Compute the full size of the rectangle
  const double xleft = xcentre - (dest->w > dest->h
				  ? radius * w / h
				  : radius);
  const double ybottom = ycentre - (dest->w > dest->h
				    ? radius
				    : radius * h / w);
  const double xsize = (dest->w > dest->h
			? radius * 2 / h
			: radius * 2);
  // Compute the pixel limits
  const int lx = x + w, ly = y + h;
  // Iterate over rows
  for(int py = y; py < ly; ++py) {
    // Starting point for this row's results
    int *res = dest->data + y * dest->w + x;
    // Complex-plane location of this row
    const double cy = ybottom + py * xsize / dest->w;
    // Iterate over columns
    for(int px = x; px < lx; ++px) {
      // Complex-plane location of this column
      const double cx = xleft + px * xsize / dest->w;
      // let c = cx + icy
      // let z = zx + izy
      //
      // then z^2 + c = zx^2 - zy^2 + cx + i(2zxzy+cy)
      int iterations = 0;
      double zx = 0, zy = 0, zx2, zy2;
      while(((zx2 = zx * zx) + (zy2 = zy * zy) < 4.0)
	    && iterations < maxiters) {
	zy = 2 * zx * zy  + cy;
	zx = zx2 - zy2 + cx;
	++iterations;
      }
      *res++ = iterations;
    }
  }
}

IterBuffer *MandelbrotJob::recompute(double cx, double cy, double r, 
				     int maxiters, int w, int h,
				     void (*completion_callback)(Job *)) {
  IterBuffer *dest = new IterBuffer(w, h);
  // Set everything to 'unknown'
  memset(dest->data, 0xFF, dest->w * dest->h * sizeof(int));
  // Chunks need to be large enough that the overhead of jobs doesn't
  // add up to much but small enough that stale jobs don't hog the CPU
  // much.
  const int chunk = 64;
  for(int px = 0; px < dest->w; px += chunk) {
    const int pw = std::min(chunk, dest->w - px);
    for(int py = 0; py < dest->h; py += chunk) {
      const int ph = std::min(chunk, dest->h - py);
      (new MandelbrotJob(px, py, pw, ph, cx, cy, r, maxiters, dest))
	->submit(completion_callback);
    }
  }
  return dest;
}

MandelbrotJob::~MandelbrotJob() {
  dest->release();
}
