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
#include "FractalJob.h"
#include <algorithm>
#include <cstring>
#include "PixelStream.h"

struct comparator {
  int cx, cy;
  comparator(int cx_, int cy_): cx(cx_), cy(cy_) {}
  int operator()(FractalJob *a, FractalJob *b) {
    int adx = a->x - cx, ady = a->y - cy;
    int ar2 = adx * adx + ady * ady;
    int bdx = b->x - cx, bdy = b->y - cy;
    int br2 = bdx * bdx + bdy * bdy;
    return ar2 < br2;
  }
};

IterBuffer *FractalJob::recompute(arith_t cx,
                                  arith_t cy,
                                  arith_t r,
                                  int maxiters,
                                  int w,
                                  int h,
                                  arith_type arith,
                                  void (*completion_callback)(Job *, void *),
                                  void *completion_data,
                                  int xpos,
                                  int ypos,
                                  const FractalJobFactory *factory) {
  IterBuffer *dest = new IterBuffer(w, h);
  // Set everything to 'unknown'#
  dest->clear();
  // Chunks need to be large enough that the overhead of jobs doesn't
  // add up to much but small enough that stale jobs don't hog the CPU
  // much.
  //
  // Chunk widths should normally be powers of 2, so that SIMD implementations
  // don't waste columns.
  const int chunk = 32;
  std::vector<FractalJob *> jobs;
  for(int px = 0; px < dest->width(); px += chunk) {
    const int pw = std::min(chunk, dest->width() - px);
    for(int py = 0; py < dest->height(); py += chunk) {
      const int ph = std::min(chunk, dest->height() - py);
      FractalJob *j = factory->create();
      j->set(dest, cx, cy, r, maxiters, px, py, pw, ph, arith);
      jobs.push_back(j);
    }
  }
  comparator c(xpos, ypos);
  std::sort(jobs.begin(), jobs.end(), c);
  for(size_t n = 0; n < jobs.size(); ++n)
    jobs[n]->submit(completion_callback, completion_data);
  return dest;
}

void FractalJob::work() {
  switch(arith) {
#if SIMD2
  case arith_simd2: simd_work(); break;
#endif
#if SIMD4
  case arith_simd4: simd_work(); break;
#endif
  default: sisd_work(); break;
  }
}

void FractalJob::sisd_work() {
  int px, py, d;
  bool escaped = false;
  if(w > 2 && h > 2) {
    PixelStreamEdge edge_pixels(x, y, w, h);
    while(edge_pixels.next(px, py))
      escaped |= sisd_calculate(px, py);
    d = 1;
  } else {
    escaped = true;
    d = 0;
  }
  PixelStreamRectangle fill_pixels(x + d, y + d, w - d, h - d);
  if(escaped) {
    while(fill_pixels.next(px, py))
      sisd_calculate(px, py);
  } else {
    while(fill_pixels.next(px, py))
      for(int i = 0; i < 4; i++)
        dest->pixel(px, py) = transform_iterations(maxiters, 0, maxiters);
  }
}

#if SIMD2 || SIMD4
void FractalJob::simd_work() {
  int px[4], py[4], d;
  bool escaped = false;
  if(w > 2 && h > 2) {
    PixelStreamEdge edge_pixels(x, y, w, h);
    while(edge_pixels.morepixels(4, px, py))
      escaped |= simd_calculate(px, py);
    d = 1;
  } else {
    escaped = true;
    d = 0;
  }
  PixelStreamRectangle fill_pixels(x + d, y + d, w - d, h - d);
  if(escaped) {
    while(fill_pixels.morepixels(4, px, py))
      simd_calculate(px, py);
  } else {
    while(fill_pixels.morepixels(4, px, py))
      for(int i = 0; i < 4; i++)
        dest->pixel(px[i], py[i]) = transform_iterations(maxiters, 0, maxiters);
  }
}
#endif

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
