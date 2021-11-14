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
#ifndef FRACTALJOB_H
#define FRACTALJOB_H

#include "Job.h"
#include "arith.h"

class FractalJobFactory;

class FractalJob: public Job {
public:
  IterBuffer *dest;       // buffer to store results in
  arith_t xleft, ybottom; // complex-plane location
  arith_t xsize;          // complex-plane size
  int maxiters;           // maximum iterations
  int x, y;               // pixel location
  int w, h;               // pixel dimensions
  arith_type arith;       // arithmetic type to use

  FractalJob(): dest(NULL) {}
  ~FractalJob() {
    if(dest)
      dest->release();
  }

  void set(IterBuffer *dest_, arith_t xcenter_, arith_t ycenter_,
           arith_t radius_, int maxiters_, int x_, int y_, int w_, int h_,
           arith_type arith_) {
    dest = dest_;
    xleft =
        xcenter_ - (dest->w > dest->h ? radius_ * dest->w / dest->h : radius_);
    ybottom =
        ycenter_ - (dest->w > dest->h ? radius_ : radius_ * dest->h / dest->w);
    xsize = (dest->w > dest->h ? radius_ * 2 * dest->w / dest->h : radius_ * 2);
    maxiters = maxiters_;
    x = x_;
    y = y_;
    w = w_;
    h = h_;
    arith = arith_;
    dest->acquire();
  }

  // Create a new IterBuffer and start to asynchronously populate it.  It will
  // be returned with one ref owned by the caller (and many by the background
  // jobs).  Uncomputed locations are set to -1.
  static IterBuffer *recompute(arith_t cx, arith_t cy, arith_t r, int maxiters,
                               int w, int h, arith_type arith,
                               void (*completion_callback)(Job *, void *),
                               void *completion_data, int xpos, int ypos,
                               const FractalJobFactory *factory);
};

class FractalJobFactory {
public:
  virtual FractalJob *create() const = 0;
};

#endif /* FRACTALJOB_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
