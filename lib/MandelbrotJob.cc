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

struct comparator {
  int cx, cy;
  comparator(int cx_, int cy_): cx(cx_), cy(cy_) {}
  int operator()(FractalJob *a, FractalJob *b) {
    int adx = a->params.x - cx, ady = a->params.y - cy;
    int ar2 = adx * adx + ady * ady;
    int bdx = b->params.x - cx, bdy = b->params.y - cy;
    int br2 = bdx * bdx + bdy * bdy;
    return ar2 < br2;
  }
};

IterBuffer *FractalJob::recompute(double cx, double cy, double r, 
				  int maxiters, int w, int h,
				  void (*completion_callback)(Job *, void *),
				  void *completion_data,
				  int xpos, int ypos,
				  const FractalJobFactory *factory) {
  // Discard stale work
  Job::cancel();
  IterBuffer *dest = new IterBuffer(w, h);
  // Set everything to 'unknown'
  memset(dest->data, 0xFF, dest->w * dest->h * sizeof(int));
  // Chunks need to be large enough that the overhead of jobs doesn't
  // add up to much but small enough that stale jobs don't hog the CPU
  // much.
  const int chunk = 32;
  std::vector<FractalJob *> jobs;
  for(int px = 0; px < dest->w; px += chunk) {
    const int pw = std::min(chunk, dest->w - px);
    for(int py = 0; py < dest->h; py += chunk) {
      const int ph = std::min(chunk, dest->h - py);
      FractalJob *j = factory->create();
      j->params.set(dest, cx, cy, r, maxiters, px, py, pw, ph);
      jobs.push_back(j);
    }
  }
  comparator c(xpos, ypos);
  std::sort(jobs.begin(), jobs.end(), c);
  for(size_t n = 0; n < jobs.size(); ++n)
    jobs[n]->submit(completion_callback, completion_data);
  return dest;
}

FractalJob *MandelbrotJobFactory::create() const {
  return new MandelbrotJob();
}
