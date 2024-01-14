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
#include "Fixed64.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

static int errors;

static void printFixed(Fixed64 f, const char *expect) {
  char buffer[128];
  printf("%02x.%06x%08x", (unsigned char)(f >> 56), (unsigned)(f >> 32) & 0x00FFFFFF, (unsigned)f);
  printf(" =%s", Fixed64_2str(buffer, sizeof buffer, f, 10));
  if(strcmp(buffer, expect)) {
    printf("<EXPECTED %s>", expect);
    ++errors;
  }
}
const struct {
  Fixed64 a, b, expect;
} Fixed64_mul_cases[] = {
    {(int64_t)5 << 56, (int64_t)7 << 56, (int64_t)35 << 56},
    {(int64_t)8 << 56, (int64_t)8 << 56, (int64_t)64 << 56},
    // Noninteger result
    {(int64_t)1 << 56, (int64_t)1 << 55, (int64_t)1 << 55},
    // Signs
    {-((int64_t)10 << 56), -((int64_t)5 << 56), (int64_t)50 << 56},
    {(int64_t)10 << 56, -((int64_t)5 << 56), -((int64_t)50 << 56)},
    {-((int64_t)10 << 56), (int64_t)5 << 56, -((int64_t)50 << 56)},
    // Very small
    {(int64_t)1 << 24, (int64_t)1 << 24, 0},
    // Underflow rounding
    {(int64_t)1 << 55, 1, 1},
};

static void Fixed64_mul_generic_test(void) {
  for(size_t i = 0; i < sizeof Fixed64_mul_cases / sizeof *Fixed64_mul_cases; i++) {
    Fixed64 c = Fixed64_mul_generic(Fixed64_mul_cases[i].a, Fixed64_mul_cases[i].b);
    printf("Fixed64_mul_generic %#lx * %#lx = %#lx (expected %#lx)%s\n",
           Fixed64_mul_cases[i].a,
           Fixed64_mul_cases[i].b,
           c,
           Fixed64_mul_cases[i].expect,
           c == Fixed64_mul_cases[i].expect ? "" : " (MISMATCH)");
  }
}

static void Fixed64_mul_test(void) {
  for(size_t i = 0; i < sizeof Fixed64_mul_cases / sizeof *Fixed64_mul_cases; i++) {
    Fixed64 c = Fixed64_mul(Fixed64_mul_cases[i].a, Fixed64_mul_cases[i].b);
    printf("Fixed64_mul %#lx * %#lx = %#lx (expected %#lx)%s\n",
           Fixed64_mul_cases[i].a,
           Fixed64_mul_cases[i].b,
           c,
           Fixed64_mul_cases[i].expect,
           c == Fixed64_mul_cases[i].expect ? "" : " (MISMATCH)");
  }
}

int main() {
  Fixed64 a, b, c;
  double x;
#if HAVE_ASM_64
  double r2;
  int count;
  Fixed64 cx, cy;
#endif

  // Addition
  a = Fixed64_int2(1);
  a /= 2;
  printf("1/2:     ");
  printFixed(a, "0.5");
  putchar('\n');
  a /= 4;
  printf("0.5/4:   ");
  printFixed(a, "0.125");
  putchar('\n');

  b = Fixed64_int2(1);
  c = a + b;
  printf("1+0.125: ");
  printFixed(c, "1.125");
  putchar('\n');

  a = Fixed64_int2(1);
  a /= 2;
  b = a;
  c = a + b;
  printf("0.5+0.5: ");
  printFixed(c, "1");
  putchar('\n');

  // Negation
  a = Fixed64_int2(10);
  b = -a;
  c = -b;
  printf("-10:     ");
  printFixed(b, "-10");
  putchar('\n');
  printf("-(-10):  ");
  printFixed(c, "10");
  putchar('\n');

  // Subtraction
  a = Fixed64_int2(1);
  b = Fixed64_int2(10);
  c = b - a;
  printf("10-1:    ");
  printFixed(c, "9");
  putchar('\n');
  c = a - b;
  printf("1-10:    ");
  printFixed(c, "-9");
  putchar('\n');

  a = Fixed64_int2(1) / 16;
  b = Fixed64_int2(1) / 256;
  c = a - b;
  printf("15/256:  ");
  printFixed(c, "0.05859375");
  putchar('\n');

  Fixed64_mul_generic_test();
  Fixed64_mul_test();

  // Division
  c = Fixed64_div(Fixed64_int2(1), Fixed64_int2(16));
  printf("1/16:    ");
  printFixed(c, "0.0625");
  putchar('\n');

  c = Fixed64_div(Fixed64_int2(1), Fixed64_int2(3));
  printf("1/3:     ");
  printFixed(c, "0.33333333333333332870740406406184774823486804962158203125");
  putchar('\n');

  c = Fixed64_div(c, Fixed64_int2(3));
  printf("1/3/3:   ");
  printFixed(c, "0.111111111111111104943205418749130330979824066162109375");
  putchar('\n');

  // Square roots
  a = Fixed64_sqrt(Fixed64_int2(2));
  printf("√2:      ");
  printFixed(a, "1.41421356237309504833010720403763116337358951568603515625");
  putchar('\n');
  a = Fixed64_mul(a, a);
  printf("(√2)²:   ");
  printFixed(a, "2");
  putchar('\n');

  a = Fixed64_sqrt(Fixed64_int2(1) / 2);
  printf("√½:      ");
  printFixed(a, "0.707106781186547517226159698111587204039096832275390625");
  putchar('\n');
  a = Fixed64_mul(a, a);
  printf("(√½)²:   ");
  printFixed(a, "0.49999999999999998612221219218554324470460414886474609375");
  putchar('\n');

  // Conversion to/from double
  a = Fixed64_double2(1.0);
  printf("1.0:     ");
  printFixed(a, "1");
  putchar('\n');
  x = Fixed64_2double(a);
  printf("and back: %g\n", x);
  if(x != 1) {
    printf("-- EXPECTED 1\n");
    ++errors;
  }
  a = Fixed64_double2(0.5);
  printf("0.5:     ");
  printFixed(a, "0.5");
  putchar('\n');
  x = Fixed64_2double(a);
  printf("and back: %g\n", x);
  if(x != 0.5) {
    printf("-- EXPECTED 0.5\n");
    ++errors;
  }
  a = Fixed64_double2(M_PI);
  printf("π:       ");
  printFixed(a, "3.141592653589793115997963468544185161590576171875");
  putchar('\n');
  x = Fixed64_2double(a);
  printf("and back: %g\n", x);
  if(x != M_PI) {
    printf("-- EXPECTED π\n");
    ++errors;
  }

  // Conversion from string
  Fixed64_str2(&a, "1.0", NULL);
  printf("1.0:     ");
  printFixed(a, "1");
  putchar('\n');
  Fixed64_str2(&a, "1.5", NULL);
  printf("1.5:     ");
  printFixed(a, "1.5");
  putchar('\n');
  Fixed64_str2(&a, "-1.1", NULL);
  printf("-1.1:    ");
  printFixed(a, "-1.1000000000000000055511151231257827021181583404541015625");
  putchar('\n');
  Fixed64_str2(&a, "0.00000000000000001387778780781445675529539585113525390625", NULL);
  printf("2⁻⁵⁶:    ");
  printFixed(a, "0.00000000000000001387778780781445675529539585113525390625");
  putchar('\n');
  Fixed64_str2(&a, "127.00000000000000001387778780781445675529539585113525390625", NULL);
  printf("127+2⁻⁵⁶:");
  printFixed(a, "127.00000000000000001387778780781445675529539585113525390625");
  putchar('\n');
  Fixed64_str2(&a, "-127.00000000000000001387778780781445675529539585113525390625", NULL);
  printf("-(127+2⁻⁵⁶):");
  printFixed(a, "-127.00000000000000001387778780781445675529539585113525390625");
  putchar('\n');

#if HAVE_ASM_64
  // Mandelbrot computation
  cx = 0x00a6aaaaaaaaaaab;
  cy = (Fixed64)0xfffd555555555555;

  printf("cx:      ");
  printFixed(cx, "0.65104166666666667129259593593815225176513195037841796875");
  putchar('\n');
  printf("cy:      ");
  printFixed(cy, "-0.01041666666666667129259593593815225176513195037841796875");
  putchar('\n');
  count = Fixed64_iterate(0, 0, cx, cy, &r2, 255);
  printf("iterate: %d   r2: %.32g\n", count, r2);
  assert(count == 5);
  assert(r2 == 255.08471462316811);

  cx = Fixed64_int2(-1);
  cy = Fixed64_int2(-1);
  printf("cx:      ");
  printFixed(cx, "-1");
  putchar('\n');
  printf("cy:      ");
  printFixed(cy, "-1");
  putchar('\n');
  count = Fixed64_iterate(0, 0, cx, cy, &r2, 255);
  printf("iterate: %d   r2: %.32g\n", count, r2);
  if(count != 4) {
    printf("-- EXPECTED iterate 4\n");
    ++errors;
  }
  if(r2 != 106) {
    printf("-- EXPECTED r2 106\n");
    ++errors;
  }
  /*
   *count=0
   *  z=0 0
   *  r²=0
   *count=1
   *  z=-1 -1
   *  r²=2
   *count=2
   *  z=-1 1
   *  r²=2
   *count=3
   *  z=-1 -3
   *  r²=10
   *count=4
   *  z=-9 5
   *  r²=106
   *final count=4
   */

#endif

  printf("%d errors\n", errors);
  return !!errors;
}
