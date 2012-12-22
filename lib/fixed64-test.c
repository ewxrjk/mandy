#include "mandy.h"
#include "Fixed64.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

static int errors;

static void printFixed(Fixed64 f, const char *expect) {
  char buffer[128];
  printf("%02x.%06x%08x",
	 (unsigned char)(f >> 56),
	 (unsigned)(f >> 32) & 0x00FFFFFF,
	 (unsigned)f);
  printf(" =%s", Fixed64_2str(buffer, sizeof buffer, f, 10));
  if(strcmp(buffer, expect)) {
    printf("<EXPECTED %s>", expect);
    ++errors;
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
  printf("1/2:     "); printFixed(a, "0.5"); putchar('\n');
  a /= 4;
  printf("0.5/4:   "); printFixed(a, "0.125"); putchar('\n');

  b = Fixed64_int2(1);
  c = a + b;
  printf("1+0.125: "); printFixed(c, "1.125"); putchar('\n');

  a = Fixed64_int2(1);
  a /= 2;
  b = a;
  c = a + b;
  printf("0.5+0.5: "); printFixed(c, "1"); putchar('\n');

  // Negation
  a = Fixed64_int2(10);
  b = -a;
  c = -b;
  printf("-10:     "); printFixed(b, "-10"); putchar('\n');
  printf("-(-10):  "); printFixed(c, "10"); putchar('\n');

  // Subtraction
  a = Fixed64_int2(1);
  b = Fixed64_int2(10);
  c = b - a;
  printf("10-1:    "); printFixed(c, "9"); putchar('\n');
  c = a - b;
  printf("1-10:    "); printFixed(c, "-9"); putchar('\n');

  a = Fixed64_int2(1) / 16;
  b = Fixed64_int2(1) / 256;
  c = a - b;
  printf("15/256:  "); printFixed(c, "0.05859375"); putchar('\n');

  // Multiply
  c = Fixed64_mul(Fixed64_int2(5), Fixed64_int2(7));
  printf("5*7:     "); printFixed(c, "35"); putchar('\n');

  // Multiply by 0.5
  c = Fixed64_mul(Fixed64_int2(101), Fixed64_int2(1)/2);
  printf("101*0.5  "); printFixed(c, "50.5"); putchar('\n');

  // Multiply with nontrivial sign
  c = Fixed64_mul(Fixed64_int2(-10), Fixed64_int2(-5));
  printf("-10*-5:  "); printFixed(c, "50"); putchar('\n');
  c = Fixed64_mul(Fixed64_int2(-10), Fixed64_int2(5));
  printf("-10*5:   "); printFixed(c, "-50"); putchar('\n');

  c = Fixed64_mul(Fixed64_int2(8), Fixed64_int2(8));
  printf("8*8:     "); printFixed(c, "64"); putchar('\n');
  
  // Very small numbers
  a = Fixed64_int2(1) / 65536 / 65536;
  printf("2⁻³²:    "); printFixed(a, "0.00000000023283064365386962890625"); putchar('\n');
  c = Fixed64_mul(a, a);
  printf("(2⁻³²)²: "); printFixed(c, "0"); putchar('\n'); /* underflows */

  // Underflow rounding
  c = Fixed64_mul(Fixed64_int2(1)/2, 1);
  printf("½(2⁻⁵⁶): "); printFixed(c, "0.00000000000000001387778780781445675529539585113525390625"); putchar('\n');

  // Division
  c = Fixed64_div(Fixed64_int2(1), Fixed64_int2(16));
  printf("1/16:    "); printFixed(c, "0.0625"); putchar('\n');

  c = Fixed64_div(Fixed64_int2(1), Fixed64_int2(3));
  printf("1/3:     "); printFixed(c, "0.33333333333333332870740406406184774823486804962158203125"); putchar('\n');

  c = Fixed64_div(c, Fixed64_int2(3));
  printf("1/3/3:   "); printFixed(c, "0.111111111111111104943205418749130330979824066162109375"); putchar('\n');

  // Square roots
  a = Fixed64_sqrt(Fixed64_int2(2));
  printf("√2:      "); printFixed(a, "1.41421356237309504833010720403763116337358951568603515625"); putchar('\n');
  a = Fixed64_mul(a, a);
  printf("(√2)²:   "); printFixed(a, "2"); putchar('\n');

  a = Fixed64_sqrt(Fixed64_int2(1) / 2);
  printf("√½:      "); printFixed(a, "0.707106781186547517226159698111587204039096832275390625"); putchar('\n');
  a = Fixed64_mul(a, a);
  printf("(√½)²:   "); printFixed(a, "0.49999999999999998612221219218554324470460414886474609375"); putchar('\n');

  // Conversion to/from double
  a = Fixed64_double2(1.0);
  printf("1.0:     "); printFixed(a, "1"); putchar('\n');
  x = Fixed64_2double(a);
  printf("and back: %g\n", x);
  if(x != 1) {
    printf("-- EXPECTED 1\n");
    ++errors;
  }
  a = Fixed64_double2(0.5);
  printf("0.5:     "); printFixed(a, "0.5"); putchar('\n');
  x = Fixed64_2double(a);
  printf("and back: %g\n", x);
  if(x != 0.5) {
    printf("-- EXPECTED 0.5\n");
    ++errors;
  }
  a = Fixed64_double2(M_PI);
  printf("π:       "); printFixed(a, "3.141592653589793115997963468544185161590576171875"); putchar('\n');
  x = Fixed64_2double(a);
  printf("and back: %g\n", x);
  if(x != M_PI) {
    printf("-- EXPECTED π\n");
    ++errors;
  }

  // Conversion from string
  Fixed64_str2(&a, "1.0", NULL);
  printf("1.0:     "); printFixed(a, "1"); putchar('\n');
  Fixed64_str2(&a, "1.5", NULL);
  printf("1.5:     "); printFixed(a, "1.5"); putchar('\n');
  Fixed64_str2(&a, "-1.1", NULL);
  printf("-1.1:    "); printFixed(a, "-1.1000000000000000055511151231257827021181583404541015625"); putchar('\n');
  Fixed64_str2(&a, "0.00000000000000001387778780781445675529539585113525390625", NULL);
  printf("2⁻⁵⁶:    "); printFixed(a, "0.00000000000000001387778780781445675529539585113525390625"); putchar('\n');
  Fixed64_str2(&a, "127.00000000000000001387778780781445675529539585113525390625", NULL);
  printf("127+2⁻⁵⁶:"); printFixed(a, "127.00000000000000001387778780781445675529539585113525390625"); putchar('\n');
  Fixed64_str2(&a, "-127.00000000000000001387778780781445675529539585113525390625", NULL);
  printf("-(127+2⁻⁵⁶):"); printFixed(a, "-127.00000000000000001387778780781445675529539585113525390625"); putchar('\n');

#if HAVE_ASM_64
  // Mandelbrot computation
  cx = 0x00a6aaaaaaaaaaab;
  cy = 0xfffd555555555555;

  printf("cx:      "); printFixed(cx, "0.65104166666666667129259593593815225176513195037841796875"); putchar('\n');
  printf("cy:      "); printFixed(cy, "-0.01041666666666667129259593593815225176513195037841796875"); putchar('\n');
  count = Fixed64_iterate(0, 0, cx, cy,
                          &r2, 255);
  printf("iterate: %d   r2: %.32g\n", count, r2);
  assert(count == 5);
  assert(r2 ==  255.08471462316811);

  cx = Fixed64_int2(-1);
  cy = Fixed64_int2(-1);
  printf("cx:      "); printFixed(cx, "-1"); putchar('\n');
  printf("cy:      "); printFixed(cy, "-1"); putchar('\n');
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
