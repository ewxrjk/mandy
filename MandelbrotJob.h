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
#ifndef MANDELBROTJOB_H
#define MANDELBROTJOB_H

#include "Job.h"

class MandelbrotJob: public Job {
  IterBuffer *dest;                     // buffer to store results in
  int x, y;                             // pixel location in buffer
  int w, h;                             // pixel size in buffer
  double xcentre, ycentre;              // complex-plane image location
  double radius;                        // complex-plane radius
  int maxiters;
public:
  ~MandelbrotJob();

  // Construct a job which fills in a rectangle
  MandelbrotJob(int x, int y,           // pixel location to draw at
                int w, int h,           // pixel size to draw
                double cx, double cy,   // centre of image
                double r,               // radius of biggest circle in image
                int maxiters,           // max iteration count
                IterBuffer *dest);

  // Do the computation (called in background thread)
  void work();

  // Create a new IterBuffer and start to asynchronously populate it.  It will
  // be returned with one ref owned by the caller (and many by the background
  // jobs).  Uncomputed locations are set to -1.
  IterBuffer *recompute(double cx, double cy, double r, 
                        int maxiters, int w, int h,
                        void (*completion_callback)(Job *));
};

#endif /* MANDELBROTJOB_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
