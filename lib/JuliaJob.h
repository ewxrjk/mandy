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
#ifndef JULIAJOB_H
#define JULIAJOB_H

#include "FractalJob.h"

class JuliaJob: public FractalJob {
  arith_t cx, cy;

public:
  JuliaJob(arith_t cx_, arith_t cy_): cx(cx_), cy(cy_) {}

  bool sisd_calculate(int px, int py) override;
#if SIMD
  bool simd_calculate(int px[4], int py[4]) override;
#endif
};

class JuliaJobFactory: public FractalJobFactory {
public:
  JuliaJobFactory(): cx(0), cy(0) {}
  arith_t cx, cy;
  FractalJob *create() const;
};

#endif /* JULIAJOB_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
