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
#ifndef FRACTALJOB_H
#define FRACTALJOB_H

#include "Job.h"
#include "arith.h"
#include "simdarith.h"

class FractalJobFactory;

class FractalJob: public Job {
public:
  IterBuffer *dest = nullptr; // buffer to store results in
  arith_t xleft, ybottom;     // complex-plane location
  arith_t xsize;              // complex-plane size
  int maxiters;               // maximum iterations
  int x, y;                   // pixel location
  int w, h;                   // pixel dimensions
  arith_type arith;           // arithmetic type to use

  FractalJob() {}
  ~FractalJob() {
    if(dest)
      dest->release();
  }

  void set(IterBuffer *dest_,
           arith_t xcenter_,
           arith_t ycenter_,
           arith_t radius_,
           int maxiters_,
           int x_,
           int y_,
           int w_,
           int h_,
           arith_type arith_) {
    dest = dest_;
    xleft = xcenter_ - (dest->width() > dest->height() ? radius_ * dest->width() / dest->height() : radius_);
    ybottom = ycenter_ - (dest->width() > dest->height() ? radius_ : radius_ * dest->height() / dest->width());
    xsize = (dest->width() > dest->height() ? radius_ * 2 * dest->width() / dest->height() : radius_ * 2);
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
  static IterBuffer *recompute(arith_t cx,
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
                               const FractalJobFactory *factory);

  // Attempt to fast-path a point
  // Return true if it can be optimized
  virtual bool fastpath(arith_t cx, arith_t cy, int &iterations, double &r2);

  // Calculate and plot px, py
  // Return true if it escapes
  virtual bool sisd_calculate(int px, int py) = 0;

#if SIMD
  // Calculate and plot the SIMD points px, py
  // Return true if any of them escape
  virtual bool simd_calculate(int px[SIMD], int py[SIMD]) = 0;
#endif

  // Do the computation (called in background thread)
  void work();
  void sisd_work();
#if SIMD
  void simd_work();
#endif
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
