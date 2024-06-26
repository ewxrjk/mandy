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
  bool sisd_calculate(int px, int py) override;
#if SIMD
  bool simd_calculate(int px[SIMD], int py[SIMD]) override;
#endif

  bool fastpath(arith_t cx, arith_t cy, int &iterations, double &r2) override;
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
