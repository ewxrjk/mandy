#include "mandy.h"
#include "Fixed128.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

void printFixed128(const struct Fixed128 *f) {
  char buffer[128];
  int n;
  printf("%08x.%08x", f->word[NFIXED128-1], f->word[NFIXED128-2]);
  for(n = NFIXED128-3; n >= 0; --n)
    printf(" %08x", f->word[n]);
  printf(" =%s", Fixed128_2str(buffer, sizeof buffer, f, 10));
}

int main() {
  struct Fixed128 a, b, c, d;

  // Divide by 2 across word boundary...
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  printf("1/2:     "); printFixed128(&a); putchar('\n');
  // ...and within one word
  Fixed128_divu(&a, &a, 4);
  printf("0.5/4:   "); printFixed128(&a); putchar('\n');

  // Add across word boundary
  Fixed128_int2(&b, 1);
  Fixed128_add(&c, &a, &b);
  printf("1+0.125: "); printFixed128(&c); putchar('\n');

  // Add with carry
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  b = a;
  Fixed128_add(&c, &a, &b);
  printf("0.5+0.5: "); printFixed128(&c); putchar('\n');

  // Negation
  Fixed128_int2(&a, 10);
  Fixed128_neg(&b, &a);
  Fixed128_neg(&c, &b);
  printf("-10:     "); printFixed128(&b); putchar('\n');
  printf("-(-10):  "); printFixed128(&b); putchar('\n');

  // Subtraction
  Fixed128_int2(&a, 1);
  Fixed128_int2(&b, 10);
  Fixed128_sub(&c, &b, &a);
  printf("10-1:    "); printFixed128(&c); putchar('\n');
  Fixed128_sub(&c, &a, &b);
  printf("1-10:    "); printFixed128(&c); putchar('\n');

  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 16);
  Fixed128_int2(&b, 1);
  Fixed128_divu(&b, &b, 256);
  Fixed128_sub(&c, &a, &b);
  printf("15/256:  "); printFixed128(&c); putchar('\n');

  Fixed128_int2(&a, 0);
  b = a;
  a.word[NFIXED128-1] = 0x4321;
  a.word[NFIXED128-2] = 0x5678abcd;
  a.word[NFIXED128-3] = 0xef012345;
  b.word[NFIXED128-1] = 0x1234;
  b.word[NFIXED128-2] = 0xabcd5678;
  b.word[NFIXED128-3] = 0x12345678;
  printf("a:       "); printFixed128(&a); putchar('\n');
  printf("b:       "); printFixed128(&b); putchar('\n');
  Fixed128_sub(&c, &a, &b);
  printf("a-b:     "); printFixed128(&c); putchar('\n');
  Fixed128_sub(&c, &b, &a);
  printf("b-a:     "); printFixed128(&c); putchar('\n');

  // Multiply
  Fixed128_int2(&a, 5);
  Fixed128_int2(&b, 7);
  Fixed128_mul(&c, &a, &b);
  printf("5*7:     "); printFixed128(&c); putchar('\n');

  // Multiply by 0.5
  Fixed128_int2(&a, 101);
  Fixed128_int2(&b, 1);
  Fixed128_divu(&b, &b, 2);
  Fixed128_mul(&c, &a, &b);
  printf("101*0.5  "); printFixed128(&c); putchar('\n');

  // Multiply with nontrivial sign
  Fixed128_int2(&a, -10);
  Fixed128_int2(&b, -5);
  Fixed128_mul(&c, &a, &b);
  printf("-10*-5:  "); printFixed128(&c); putchar('\n');
  Fixed128_int2(&a, -10);
  Fixed128_int2(&b, 5);
  Fixed128_mul(&c, &a, &b);
  printf("-10*5:   "); printFixed128(&c); putchar('\n');

  // Multiply regression
  memset(&a, 0, sizeof a);
  memset(&b, 0, sizeof b);
  a.word[NFIXED128-1] = 0x00000000;
  a.word[NFIXED128-2] = 0x95f61998;
  a.word[NFIXED128-3] = 0x0c433000;
  a.word[NFIXED128-4] = 0x00000000;	/* about 0.5857864... */
  b.word[NFIXED128-1] = 0xffffffff;
  b.word[NFIXED128-2] = 0xfaaaaaaa;
  b.word[NFIXED128-3] = 0xaaaaaaaa;
  b.word[NFIXED128-4] = 0xaaaaaaaa;	/* about -0.02083333... */
  printf("a:       "); printFixed128(&a); putchar('\n');
  printf("b:       "); printFixed128(&b); putchar('\n');
  Fixed128_mul(&c, &a, &b);
  printf("a*b:     "); printFixed128(&c); putchar('\n');
  printf("  ...should be about -0.0122038841\n");

  // Very small numbers
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 65536);
  Fixed128_divu(&a, &a, 65536);
  printf("2⁻³²:    "); printFixed128(&a); putchar('\n');
  b = a;
  Fixed128_mul(&c, &a, &b);
  printf("(2⁻³²)²: "); printFixed128(&c); putchar('\n');

  // Even smaller than that
  Fixed128_mul(&d, &a, &c);
  printf("(2⁻³²)³: "); printFixed128(&d); putchar('\n');

  // Underflow rounding
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  Fixed128_mul(&c, &a, &d);
  printf("½(2⁻³²)³:"); printFixed128(&d); putchar('\n');

  // Division
  Fixed128_int2(&a, 1);
  Fixed128_int2(&b, 16);
  Fixed128_div(&c, &a, &b);
  printf("1/16:    "); printFixed128(&c); putchar('\n');

  Fixed128_int2(&a, 1);
  Fixed128_int2(&b, 3);
  Fixed128_div(&c, &a, &b);
  printf("1/3:     "); printFixed128(&c); putchar('\n');

  Fixed128_div(&c, &c, &b);
  printf("1/3/3:   "); printFixed128(&c); putchar('\n');

  // Square roots
  Fixed128_int2(&a, 2);
  Fixed128_sqrt(&a, &a);
  printf("√2:      "); printFixed128(&a); putchar('\n');
  Fixed128_mul(&b, &a, &a);
  printf("(√2)²:   "); printFixed128(&b); putchar('\n');

  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  Fixed128_sqrt(&a, &a);
  printf("√½:      "); printFixed128(&a); putchar('\n');
  Fixed128_mul(&b, &a, &a);
  printf("(√½)²:   "); printFixed128(&b); putchar('\n');

  // Conversion to/from double
  Fixed128_double2(&a, 1.0);
  printf("1.0:     "); printFixed128(&a); putchar('\n');
  printf("and back: %g\n", Fixed128_2double(&a));
  Fixed128_double2(&a, 0.5);
  printf("0.5:     "); printFixed128(&a); putchar('\n');
  printf("and back: %g\n", Fixed128_2double(&a));
  Fixed128_double2(&a, M_PI);
  printf("π:       "); printFixed128(&a); putchar('\n');
  printf("and back: %g\n", Fixed128_2double(&a));

  // Conversion from string
  Fixed128_str2(&a, "1.0", NULL);
  printf("1.0:     "); printFixed128(&a); putchar('\n');
  Fixed128_str2(&a, "1.5", NULL);
  printf("1.5:     "); printFixed128(&a); putchar('\n');
  Fixed128_str2(&a, "-1.1", NULL);
  printf("-1.1:    "); printFixed128(&a); putchar('\n');
  Fixed128_str2(&a, "0.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625", NULL);
  printf("2⁻⁹⁶:    "); printFixed128(&a); putchar('\n');
  Fixed128_str2(&a, "2147483647.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625", NULL);
  printf("2³¹-1+2⁻⁹⁶:"); printFixed128(&a); putchar('\n');
  Fixed128_str2(&a, "-2147483647.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625", NULL);
  printf("-(2³¹-1+2⁻⁹⁶):"); printFixed128(&a); putchar('\n');

#if HAVE_ASM && NFIXED128 == 4
  // Mandelbrot computation
  Fixed128_int2(&a, 0);
  Fixed128_int2(&b, 0);
  memset(&c, 0, sizeof c);
  c.word[NFIXED128-1] = 0;
  c.word[NFIXED128-2] = 0xa6aaaaaa;
  c.word[NFIXED128-3] = 0xaaaaaaaa;
  c.word[NFIXED128-4] = 0xaaaaaaab;
  printf("cx:      "); printFixed128(&c); putchar('\n');
  memset(&d, 0, sizeof d);
  d.word[NFIXED128-1] = 0xffffffff;
  d.word[NFIXED128-2] = 0xfd555555;
  d.word[NFIXED128-3] = 0x55555555;
  d.word[NFIXED128-4] = 0x55555555;
  printf("cy:      "); printFixed128(&d); putchar('\n');
  int count = Fixed128_iterate(&a, &b, &c, &d, 255);
  printf("iterate: %d\n", count);
  assert(count == 5);

  Fixed128_int2(&a, 0);
  Fixed128_int2(&b, 0);
  Fixed128_int2(&c, -1);
  Fixed128_int2(&d, -1);
  printf("cx:      "); printFixed128(&c); putchar('\n');
  printf("cy:      "); printFixed128(&d); putchar('\n');
  count = Fixed128_iterate(&a, &b, &c, &d, 255);
  printf("iterate: %d\n", count);
  assert(count == 4);
#endif

  return 0;
}
