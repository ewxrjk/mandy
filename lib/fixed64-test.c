#include <config.h>
#include "Fixed64.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

void printFixed(Fixed64 f) {
  char buffer[128];
  printf("%02x.%06x%08x",
	 (unsigned char)(f >> 56),
	 (unsigned)(f >> 32) & 0x00FFFFFF,
	 (unsigned)f);
  printf(" =%s", Fixed64_2str(buffer, sizeof buffer, f, 10));
}

int main() {
  Fixed64 a, b, c;

  // Addition
  a = Fixed64_int2(1);
  a /= 2;
  printf("1/2:     "); printFixed(a); putchar('\n');
  a /= 4;
  printf("0.5/4:   "); printFixed(a); putchar('\n');

  b = Fixed64_int2(1);
  c = a + b;
  printf("1+0.125: "); printFixed(c); putchar('\n');

  a = Fixed64_int2(1);
  a /= 2;
  b = a;
  c = a + b;
  printf("0.5+0.5: "); printFixed(c); putchar('\n');

  // Negation
  a = Fixed64_int2(10);
  b = -a;
  c = -b;
  printf("-10:     "); printFixed(b); putchar('\n');
  printf("-(-10):  "); printFixed(b); putchar('\n');

  // Subtraction
  a = Fixed64_int2(1);
  b = Fixed64_int2(10);
  c = b - a;
  printf("10-1:    "); printFixed(c); putchar('\n');
  c = a - b;
  printf("1-10:    "); printFixed(c); putchar('\n');

  a = Fixed64_int2(1) / 16;
  b = Fixed64_int2(1) / 256;
  c = a - b;
  printf("15/256:  "); printFixed(c); putchar('\n');

  // Multiply
  c = Fixed64_mul(Fixed64_int2(5), Fixed64_int2(7));
  printf("5*7:     "); printFixed(c); putchar('\n');

  // Multiply by 0.5
  c = Fixed64_mul(Fixed64_int2(101), Fixed64_int2(1)/2);
  printf("101*0.5  "); printFixed(c); putchar('\n');

  // Multiply with nontrivial sign
  c = Fixed64_mul(Fixed64_int2(-10), Fixed64_int2(-5));
  printf("-10*-5:  "); printFixed(c); putchar('\n');
  c = Fixed64_mul(Fixed64_int2(-10), Fixed64_int2(5));
  printf("-10*5:   "); printFixed(c); putchar('\n');

  c = Fixed64_mul(Fixed64_int2(8), Fixed64_int2(8));
  printf("8*8:     "); printFixed(c); putchar('\n');
  
  // Very small numbers
  a = Fixed64_int2(1) / 65536 / 65536;
  printf("2⁻³²:    "); printFixed(a); putchar('\n');
  c = Fixed64_mul(a, a);
  printf("(2⁻³²)²: "); printFixed(c); putchar('\n');

  // Underflow rounding
  c = Fixed64_mul(Fixed64_int2(1)/2, 1);
  printf("½(2⁻⁵⁶): "); printFixed(c); putchar('\n');

  // Division
  c = Fixed64_div(Fixed64_int2(1), Fixed64_int2(16));
  printf("1/16:    "); printFixed(c); putchar('\n');

  c = Fixed64_div(Fixed64_int2(1), Fixed64_int2(3));
  printf("1/3:     "); printFixed(c); putchar('\n');

  c = Fixed64_div(c, Fixed64_int2(3));
  printf("1/3/3:   "); printFixed(c); putchar('\n');

  // Square roots
  a = Fixed64_sqrt(Fixed64_int2(2));
  printf("√2:      "); printFixed(a); putchar('\n');
  a = Fixed64_mul(a, a);
  printf("(√2)²:   "); printFixed(a); putchar('\n');

  a = Fixed64_sqrt(Fixed64_int2(1) / 2);
  printf("√½:      "); printFixed(a); putchar('\n');
  a = Fixed64_mul(a, a);
  printf("(√½)²:   "); printFixed(a); putchar('\n');

  // Conversion to/from double
  a = Fixed64_double2(1.0);
  printf("1.0:     "); printFixed(a); putchar('\n');
  printf("and back: %g\n", Fixed64_2double(a));
  a = Fixed64_double2(0.5);
  printf("0.5:     "); printFixed(a); putchar('\n');
  printf("and back: %g\n", Fixed64_2double(a));
  a = Fixed64_double2(M_PI);
  printf("π:       "); printFixed(a); putchar('\n');
  printf("and back: %g\n", Fixed64_2double(a));

  // Conversion from string
  Fixed64_str2(&a, "1.0", NULL);
  printf("1.0:     "); printFixed(a); putchar('\n');
  Fixed64_str2(&a, "1.5", NULL);
  printf("1.5:     "); printFixed(a); putchar('\n');
  Fixed64_str2(&a, "-1.1", NULL);
  printf("-1.1:    "); printFixed(a); putchar('\n');
  Fixed64_str2(&a, "0.00000000000000001387778780781445675529539585113525390625", NULL);
  printf("2⁻⁵⁶:    "); printFixed(a); putchar('\n');
  Fixed64_str2(&a, "127.00000000000000001387778780781445675529539585113525390625", NULL);
  printf("127+2⁻⁵⁶:"); printFixed(a); putchar('\n');
  Fixed64_str2(&a, "-127.00000000000000001387778780781445675529539585113525390625", NULL);
  printf("-(127+2⁻⁵⁶):"); printFixed(a); putchar('\n');

#if HAVE_ASM
  // Mandelbrot computation
  double r2;
  Fixed64 cx = 0x00a6aaaaaaaaaaab;
  Fixed64 cy = 0xfffd555555555555;
  printf("cx:      "); printFixed(cx); putchar('\n');
  printf("cy:      "); printFixed(cy); putchar('\n');
  int count = Fixed64_iterate(0, 0, cx, cy,
			      &r2, 255);
  printf("iterate: %d   r2: %g\n", count, r2);
  assert(count == 5);

  cx = Fixed64_int2(-1);
  cy = Fixed64_int2(-1);
  printf("cx:      "); printFixed(cx); putchar('\n');
  printf("cy:      "); printFixed(cy); putchar('\n');
  count = Fixed64_iterate(0, 0, cx, cy, &r2, 255);
  printf("iterate: %d   r2: %g\n", count, r2);
  assert(count == 4);
#endif

  return 0;
}
