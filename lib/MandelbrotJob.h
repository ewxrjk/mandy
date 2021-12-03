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
#ifndef MANDELBROTJOB_H
#define MANDELBROTJOB_H

#include "FractalJob.h"

class MandelbrotJob: public FractalJob {
public:
  // Do the computation (called in background thread)
  void work();

  // SIMD implementations
  void simd();

  // SIMD plot some pixels
  void plot(int *px, int *py);

  inline void simd_iterate(const double *zxvalues, const double *zyvalues,
                           const double *cxvalues, const double *cyvalues,
                           int maxiters, int *iterations, double *r2values);
};

class MandelbrotJobFactory: public FractalJobFactory {
public:
  FractalJob *create() const;
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
