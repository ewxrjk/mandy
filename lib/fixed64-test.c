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

static const struct {
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
    // From the square root tests
    {0x016a09e667f3bcc9, 0x016a09e667f3bcc9, (int64_t)2 << 56},
    {0x00b504f333f9de65, 0x00b504f333f9de65, 0x0080000000000001},
};

static void Fixed64_mul_test(void) {
  for(size_t i = 0; i < sizeof Fixed64_mul_cases / sizeof *Fixed64_mul_cases; i++) {
    Fixed64 c = Fixed64_mul(Fixed64_mul_cases[i].a, Fixed64_mul_cases[i].b);
    printf("Fixed64_mul 0x%016lx * 0x%016lx = 0x%016lx (expected 0x%016lx)%s\n",
           Fixed64_mul_cases[i].a,
           Fixed64_mul_cases[i].b,
           c,
           Fixed64_mul_cases[i].expect,
           c == Fixed64_mul_cases[i].expect ? "" : " (MISMATCH)");
    errors += c != Fixed64_mul_cases[i].expect;
  }
}

static const struct {
  Fixed64 a, b, expect;
} Fixed64_div_cases[] = {
    // 1/1 = 1
    {(int64_t)1 << 56, (int64_t)1 << 56, (int64_t)1 << 56},
    // 1/0.5 = 2
    {(int64_t)1 << 56, (int64_t)1 << 55, (int64_t)2 << 56},
    // 1/16 = 0.0625
    {(int64_t)1 << 56, (int64_t)16 << 56, (int64_t)1 << 52},
    // 1/3 = 0x00.55555555555555(5555555555555555...)
    {(int64_t)1 << 56, (int64_t)3 << 56, 0x0055555555555555},
    // 1/3/3 = 0x00.1c71c71c71c71c(71c71c71c71c71c7...)
    {0x0055555555555555, (int64_t)3 << 56, 0x001c71c71c71c71c},
    // 2/3 = 0x00.aaaaaaaaaaaaaa(aaaaaaaaaaaaaaaa...)
    {(int64_t)2 << 56, (int64_t)3 << 56, 0x00aaaaaaaaaaaaab},
    // Signs
    {-((int64_t)1 << 56), (int64_t)16 << 56, -((int64_t)1 << 52)},
    {(int64_t)1 << 56, -((int64_t)16 << 56), -((int64_t)1 << 52)},
};

static void Fixed64_div_test(void) {
  for(size_t i = 0; i < sizeof Fixed64_div_cases / sizeof *Fixed64_div_cases; i++) {
    Fixed64 c = Fixed64_div(Fixed64_div_cases[i].a, Fixed64_div_cases[i].b);
    printf("Fixed64_div 0x%016lx / 0x%016lx = 0x%016lx (expected 0x%016lx)%s\n",
           Fixed64_div_cases[i].a,
           Fixed64_div_cases[i].b,
           c,
           Fixed64_div_cases[i].expect,
           c == Fixed64_div_cases[i].expect ? "" : " (MISMATCH)");
    errors += c != Fixed64_div_cases[i].expect;
  }
}

static const struct {
  Fixed64 a, expect;
} Fixed64_sqrt_cases[] = {
    // √4 = 2
    {(int64_t)4 << 56, (int64_t)2 << 56},
    // √9 = 3
    {(int64_t)9 << 56, (int64_t)3 << 56},
    // √16 = 4
    {(int64_t)16 << 56, (int64_t)4 << 56},
    // √2 = 0x01.6a09e667f3bcc9(08b2fb1366ea957d3e...)
    {(int64_t)2 << 56, 0x016a09e667f3bcc9},
    // √½ = 0x00.b504f333f9de64(84597d89b3754abe9f...)
    {(int64_t)1 << 55, 0x00b504f333f9de65}, // rounded up!
};

static void Fixed64_sqrt_test(void) {
  for(size_t i = 0; i < sizeof Fixed64_sqrt_cases / sizeof *Fixed64_sqrt_cases; i++) {
    Fixed64 c = Fixed64_sqrt(Fixed64_sqrt_cases[i].a);
    printf("Fixed64_sqrt 0x%016lx = 0x%016lx (expected 0x%016lx)%s\n",
           Fixed64_sqrt_cases[i].a,
           c,
           Fixed64_sqrt_cases[i].expect,
           c == Fixed64_sqrt_cases[i].expect ? "" : " (MISMATCH)");
    errors += c != Fixed64_sqrt_cases[i].expect;
  }
}

int main() {
  Fixed64 a, b, c;
  double x;
#if HAVE_ASM_FIXED64_ITERATE
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

  Fixed64_mul_test();
  Fixed64_div_test();
  Fixed64_sqrt_test();

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

#if HAVE_ASM_FIXED64_ITERATE
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
