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

#if __amd64__
# include <x86intrin.h>
# define CYCLES() __rdtsc()
#endif

#if __aarch64__ && __clang__
# include <arm_acle.h>
#define CYCLES() __arm_rsr64("CNTVCT_EL0")
#endif

#if __aarch64__ && ! defined CYCLES
static inline unsigned long long cycles(void) {
  unsigned long long r;
  __asm__ volatile("mrs %0,CNTVCT_EL0" : "=r"(r));
  return r;
}
#define CYCLES() cycles()
#endif

#if SIMD
#include "simdarith.h"
#endif
#include <ctime>

int main(int argc, char **argv) {
  int maxiter = 20000;
  if(argc > 1)
    maxiter = atoi(argv[1]);
  double zxvalues[SIMD_MAX] = {0}, zyvalues[SIMD_MAX] = {0}, cxvalues[SIMD_MAX] = {0}, cyvalues[SIMD_MAX] = {0},
         r2values[SIMD_MAX] = {0};
  int iterations[SIMD_MAX];
  unsigned long long start = CYCLES();
  simd_iterate(zxvalues, zyvalues, cxvalues, cyvalues, maxiter, iterations, r2values, 0);
  __asm__ volatile("" : : "m"(iterations[0]), "m"(r2values[0]), "m"(iterations[2]), "m"(r2values[2]));
  unsigned long long finish = CYCLES();
  unsigned long long cycles = finish - start;
  printf("iterations %d/%d cycles %llu cycles/iteration %f\n",
         iterations[0],
         maxiter,
         cycles,
         (double)cycles / iterations[0]);
  return 0;
}
