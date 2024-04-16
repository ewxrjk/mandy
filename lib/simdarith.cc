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
#include "simdarith.h"

void simd_iterate(const double *zxvalues,
                  const double *zyvalues,
                  const double *cxvalues,
                  const double *cyvalues,
                  int maxiters,
                  int *iterations,
                  double *r2values,
                  int mandelbrot) {
#if SIMD4
  simd_iterate4(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations, r2values, mandelbrot);
#elif SIMD2
  simd_iterate2(zxvalues, zyvalues, cxvalues, cyvalues, maxiters, iterations, r2values, mandelbrot);
  simd_iterate2(
      zxvalues + 2, zyvalues + 2, cxvalues + 2, cyvalues + 2, maxiters, iterations + 2, r2values + 2, mandelbrot);
#endif
}
