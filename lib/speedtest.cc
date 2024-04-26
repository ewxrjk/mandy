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
  #include "arith.h"
  #if SIMD
  #include "simdarith.h"
  #endif
  #include <ctime>

  int main(int argc, char **argv) {
    int repeats = 40000, maxiter = 20000;
    if(argc > 1)
      repeats = atoi(argv[1]);
    if(argc > 2)
      maxiter = atoi(argv[2]);
    arith_t values[18], zx, zy, cx, cy;
    for(int i = 0; i < 18; i++)
      values[i] = i * 0.125 - 1.0;
    for(int t = 0; t < arith_limit; ++t) {
  #if !SIMD
      if(t == arith_simd)
        continue; // not supported by iterate()
  #endif
      clock_t begin = clock();
      int zxi = 0, zyi = 0, cxi = 0, cyi = 0;
      for(int n = 0; n < repeats; ++n) {
        zx = values[zxi];
        zy = values[zyi];
        cx = values[cxi];
        cy = values[cyi];
        if(++zxi >= 18) {
          zxi = 0;
          if(++zyi >= 18) {
            zyi = 0;
            if(++cxi >= 18) {
              cxi = 0;
              if(++cyi >= 18)
                cyi = 0;
            }
          }
        }
        if(t == arith_simd) {
          double zxvalues[SIMD], zyvalues[SIMD], cxvalues[SIMD], cyvalues[SIMD], r2values[SIMD];
          int iterations[SIMD];
          for(int j = 0; j < SIMD; j++) {
            zxvalues[j] = (double)zx;
            zyvalues[j] = (double)zy;
            cxvalues[j] = (double)cx;
            cyvalues[j] = (double)cy;
          }
          simd_iterate(zxvalues, zyvalues, cxvalues, cyvalues, maxiter, iterations, r2values, 0);
        } else {
          double r2;
          iterate(zx, zy, cx, cy, maxiter, arith_type(t), r2);
        }
      }
      clock_t end = clock();
      double seconds = (end - begin) / (double)CLOCKS_PER_SEC;
      double iterations = (double)repeats * maxiter;
      if(t==arith_simd)
        iterations *= SIMD;
      double ips = iterations / seconds;
      printf("%12s %3.2fs; %10.0f iterations; %12.0f iterations/second\n", arith_names[t], seconds, iterations, ips);
    }
    return 0;
  }
