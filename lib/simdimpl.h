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

typedef double vector __attribute__((vector_size(BYTES)));
typedef long long ivector __attribute__((vector_size(BYTES)));

static inline bool escape_check(ivector &escaped_already,
                                ivector &escape_iters,
                                ivector escaped,
                                int iterations) {
  ivector escaped_this_time = escaped & ~escaped_already;
  ivector iters_vector = {REP(iterations)};
  escape_iters |= iters_vector & escaped_this_time;
  escaped_already |= escaped;
  return NONZERO(escaped_already);
}

static inline void NAME(const double *zxvalues,
                        const double *zyvalues,
                        const double *cxvalues,
                        const double *cyvalues,
                        int maxiters,
                        int *iters,
                        double *r2values,
                        int mandelbrot) {
  const vector Cx = {VALUES(cxvalues)};
  const vector Cy = {VALUES(cyvalues)};
  vector Zx = {VALUES(zxvalues)};
  vector Zy = {VALUES(zyvalues)};
  vector r2 = {REP(0)};
  ivector escape_iters = {REP(0)}, escaped_already = {REP(0)};
  int iterations = 0;

  if(mandelbrot) {
    const vector cxq = (Cx - 0.25);
    const vector cy2 = Cy * Cy;
    const vector q = cxq * cxq + cy2;
    ivector escaped = (4.0 * q * (q + cxq) < cy2) || (Cx * Cx + 2.0 * Cx + 1.0 + cy2 < 1.0 / 16.0);
    escape_check(escaped_already, escape_iters, escaped, maxiters);
  }

  while(iterations < maxiters) {
    vector Zx2 = Zx * Zx;
    vector Zy2 = Zy * Zy;
    r2 = Zx2 + Zy2;
    ivector escaped = r2 >= 64.0;
    if(escape_check(escaped_already, escape_iters, escaped, iterations))
      break;
    vector Zxnew = Zx2 - Zy2 + Cx;
    vector Zynew = 2 * Zx * Zy + Cy;
    Zx = escaped ? Zx : Zxnew;
    Zy = escaped ? Zy : Zynew;
    iterations++;
  }
  ivector maxiters_vector = {REP(maxiters)};
  escape_iters |= maxiters_vector & ~escaped_already;
  ASSIGN(r2values, r2);
  ASSIGN(iters, escape_iters);
}

#undef vector
#undef ivector
#undef escape_check

#undef WRAP
