#include <config.h>
#include "Fixed64.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

void printFixed(Fixed64 f) {
  char buffer[128];
  printf("%02x.%06x%08x",
	 (unsigned)(f >> 56),
	 (unsigned)(f >> 32) & 0x00FFFFFF,
	 (unsigned)f);
  printf(" =%s", Fixed64_2str(buffer, sizeof buffer, f, 10));
}

int main() {
  Fixed64 a, b, c;

  a = Fixed64_int2(1);
  a /= 2;
  printf("1/2:     "); printFixed(a); putchar('\n');
  a /= 4;
  printf("0.5/4:   "); printFixed(a); putchar('\n');

  b = Fixed64_int2(1);
  c = a + b;
  printf("1+0.125: "); printFixed(c); putchar('\n');

  // Add with carry
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

#if 0				/* TODO */
  Fixed64_int2(a, 0);
  b = a;
  a.word[NFIXED-1] = 0x4321;
  a.word[NFIXED-2] = 0x5678abcd;
  a.word[NFIXED-3] = 0xef012345;
  b.word[NFIXED-1] = 0x1234;
  b.word[NFIXED-2] = 0xabcd5678;
  b.word[NFIXED-3] = 0x12345678;
  printf("a:       "); printFixed(a); putchar('\n');
  printf("b:       "); printFixed(b); putchar('\n');
  Fixed64_sub(&c, a, &b);
  printf("a-b:     "); printFixed(c); putchar('\n');
  Fixed64_sub(&c, &b, a);
  printf("b-a:     "); printFixed(c); putchar('\n');
#endif

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

#if 0				/* TODO */
  // Multiply regression
  memset(a, 0, sizeof a);
  memset(&b, 0, sizeof b);
  a.word[NFIXED-1] = 0x00000000;
  a.word[NFIXED-2] = 0x95f61998;
  a.word[NFIXED-3] = 0x0c433000;
  a.word[NFIXED-4] = 0x00000000;	/* about 0.5857864... */
  b.word[NFIXED-1] = 0xffffffff;
  b.word[NFIXED-2] = 0xfaaaaaaa;
  b.word[NFIXED-3] = 0xaaaaaaaa;
  b.word[NFIXED-4] = 0xaaaaaaaa;	/* about -0.02083333... */
  printf("a:       "); printFixed(a); putchar('\n');
  printf("b:       "); printFixed(b); putchar('\n');
  Fixed64_mul(&c, a, &b);
  printf("a*b:     "); printFixed(c); putchar('\n');
  printf("  ...should be about -0.0122038841\n");
#endif

  // Very small numbers
  a = Fixed64_int2(1) / 65536 / 65536;
  printf("2⁻³²:    "); printFixed(a); putchar('\n');
  c = Fixed64_mul(a, a);
  printf("(2⁻³²)²: "); printFixed(c); putchar('\n');

  // Underflow rounding
  c = Fixed64_mul(Fixed64_int2(1)/2, 1);
  printf("½(2⁻⁵⁶): "); printFixed(c); putchar('\n');

#if 0				/* TODO */
  // Division
  Fixed64_int2(a, 1);
  Fixed64_int2(&b, 16);
  Fixed64_div(&c, a, &b);
  printf("1/16:    "); printFixed(c); putchar('\n');

  Fixed64_int2(a, 1);
  Fixed64_int2(&b, 3);
  Fixed64_div(&c, a, &b);
  printf("1/3:     "); printFixed(c); putchar('\n');

  Fixed64_div(&c, &c, &b);
  printf("1/3/3:   "); printFixed(c); putchar('\n');

  // Square roots
  Fixed64_int2(a, 2);
  Fixed64_sqrt(a, a);
  printf("√2:      "); printFixed(a); putchar('\n');
  Fixed64_mul(&b, a, a);
  printf("(√2)²:   "); printFixed(b); putchar('\n');

  Fixed64_int2(a, 1);
  Fixed64_divu(a, a, 2);
  Fixed64_sqrt(a, a);
  printf("√½:      "); printFixed(a); putchar('\n');
  Fixed64_mul(&b, a, a);
  printf("(√½)²:   "); printFixed(b); putchar('\n');
#endif

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

#if 0
#if HAVE_ASM
  // Mandelbrot computation
  Fixed64_int2(a, 0);
  Fixed64_int2(&b, 0);
  memset(&c, 0, sizeof c);
  c.word[NFIXED-1] = 0;
  c.word[NFIXED-2] = 0xa6aaaaaa;
  c.word[NFIXED-3] = 0xaaaaaaaa;
  c.word[NFIXED-4] = 0xaaaaaaab;
  printf("cx:      "); printFixed(c); putchar('\n');
  memset(&d, 0, sizeof d);
  d.word[NFIXED-1] = 0xffffffff;
  d.word[NFIXED-2] = 0xfd555555;
  d.word[NFIXED-3] = 0x55555555;
  d.word[NFIXED-4] = 0x55555555;
  printf("cy:      "); printFixed(d); putchar('\n');
  int count = Fixed64_iterate(a, &b, &c, &d, 255);
  printf("iterate: %d\n", count);
  assert(count >= 0);

  Fixed64_int2(&c, -1);
  Fixed64_int2(&d, -1);
  printf("cx:      "); printFixed(c); putchar('\n');
  printf("cy:      "); printFixed(d); putchar('\n');
  count = Fixed64_iterate(a, &b, &c, &d, 255);
  printf("iterate: %d\n", count);
#endif
#endif

  return 0;
}
