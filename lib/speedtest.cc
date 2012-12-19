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
#include "mandy.h"
#include "arith.h"
#include <ctime>

int main(int argc, char **argv) {
  int repeats = 40000, maxiter = 20000;
  if(argc > 1)
    repeats = atoi(argv[1]);
  if(argc > 2)
    maxiter = atoi(argv[2]);
  for(int t = 0; t < arith_limit; ++t) {
    arith_t zx = 0;
    arith_t zy = 0;
    arith_t cx = 0.125;
    arith_t cy = 0.125;
    clock_t begin = clock();
    for(int n = 0; n < repeats; ++n)
      iterate(zx, zy, cx, cy, maxiter, arith_type(t));
    clock_t end = clock();
    double seconds = (end - begin) / (double)CLOCKS_PER_SEC;
    double iterations = (double)repeats * maxiter;
    double ips = iterations / seconds;
    printf("%12s %6gs; %g iterations; %10g iterations/second\n", 
	   arith_names[t], seconds, iterations, ips);
  }
  return 0;
}
