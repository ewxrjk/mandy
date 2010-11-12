#include <config.h>
#include "Fixed.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void printFixed(const struct Fixed *f) {
  char buffer[128];
  int n;
  printf("%08x.%08x", f->word[NFIXED-1], f->word[NFIXED-2]);
  for(n = NFIXED-3; n >= 0; --n)
    printf(" %08x", f->word[n]);
  printf(" =%s", Fixed_2str(buffer, sizeof buffer, f, 10));
}

int main() {
  struct Fixed a, b, c, d;

  // Divide by 2 across word boundary...
  Fixed_int2(&a, 1);
  Fixed_divu(&a, &a, 2);
  printf("1/2:     "); printFixed(&a); putchar('\n');
  // ...and within one word
  Fixed_divu(&a, &a, 4);
  printf("0.5/4:   "); printFixed(&a); putchar('\n');

  // Add across word boundary
  Fixed_int2(&b, 1);
  Fixed_add(&c, &a, &b);
  printf("1+0.125: "); printFixed(&c); putchar('\n');

  // Add with carry
  Fixed_int2(&a, 1);
  Fixed_divu(&a, &a, 2);
  b = a;
  Fixed_add(&c, &a, &b);
  printf("0.5+0.5: "); printFixed(&c); putchar('\n');

  // Negation
  Fixed_int2(&a, 10);
  Fixed_neg(&b, &a);
  Fixed_neg(&c, &b);
  printf("-10:     "); printFixed(&b); putchar('\n');
  printf("-(-10):  "); printFixed(&b); putchar('\n');

  // Subtraction
  Fixed_int2(&a, 1);
  Fixed_int2(&b, 10);
  Fixed_sub(&c, &b, &a);
  printf("10-1:    "); printFixed(&c); putchar('\n');
  Fixed_sub(&c, &a, &b);
  printf("1-10:    "); printFixed(&c); putchar('\n');

  Fixed_int2(&a, 1);
  Fixed_divu(&a, &a, 16);
  Fixed_int2(&b, 1);
  Fixed_divu(&b, &b, 256);
  Fixed_sub(&c, &a, &b);
  printf("-15/16:  "); printFixed(&c); putchar('\n');

  Fixed_int2(&a, 0);
  b = a;
  a.word[NFIXED-1] = 0x4321;
  a.word[NFIXED-2] = 0x5678abcd;
  a.word[NFIXED-3] = 0xef012345;
  b.word[NFIXED-1] = 0x1234;
  b.word[NFIXED-2] = 0xabcd5678;
  b.word[NFIXED-3] = 0x12345678;
  printf("a:       "); printFixed(&a); putchar('\n');
  printf("b:       "); printFixed(&b); putchar('\n');
  Fixed_sub(&c, &a, &b);
  printf("a-b:     "); printFixed(&c); putchar('\n');
  Fixed_sub(&c, &b, &a);
  printf("b-a:     "); printFixed(&c); putchar('\n');

  // Multiply
  Fixed_int2(&a, 5);
  Fixed_int2(&b, 7);
  Fixed_mul(&c, &a, &b);
  printf("5*7:     "); printFixed(&c); putchar('\n');

  // Multiply by 0.5
  Fixed_int2(&a, 101);
  Fixed_int2(&b, 1);
  Fixed_divu(&b, &b, 2);
  Fixed_mul(&c, &a, &b);
  printf("101*0.5  "); printFixed(&c); putchar('\n');

  // Multiply with nontrivial sign
  Fixed_int2(&a, -10);
  Fixed_int2(&b, -5);
  Fixed_mul(&c, &a, &b);
  printf("-10*-5:  "); printFixed(&c); putchar('\n');
  Fixed_int2(&a, -10);
  Fixed_int2(&b, 5);
  Fixed_mul(&c, &a, &b);
  printf("-10*5:   "); printFixed(&c); putchar('\n');

  // Multiply regression
  memset(&a, 0, sizeof a);
  memset(&b, 0, sizeof b);
  a.word[NFIXED-1] = 0x00000000;
  a.word[NFIXED-2] = 0x95f61998;
  a.word[NFIXED-3] = 0x0c433000;
  a.word[NFIXED-4] = 0x00000000;	/* about 0.5857864... */
  b.word[NFIXED-1] = 0xffffffff;
  b.word[NFIXED-2] = 0xfaaaaaaa;
  b.word[NFIXED-3] = 0xaaaaaaaa;
  b.word[NFIXED-4] = 0xaaaaaaaa;	/* about -0.02083333... */
  printf("a:       "); printFixed(&a); putchar('\n');
  printf("b:       "); printFixed(&b); putchar('\n');
  Fixed_mul(&c, &a, &b);
  printf("a*b:     "); printFixed(&c); putchar('\n');
  printf("  ...should be about -0.0122038841\n");
  printf("a:       "); printFixed(&a); putchar('\n');
  printf("b:       "); printFixed(&b); putchar('\n');

  // Very small numbers
  Fixed_int2(&a, 1);
  Fixed_divu(&a, &a, 65536);
  Fixed_divu(&a, &a, 65536);
  printf("2⁻³²:    "); printFixed(&a); putchar('\n');
  b = a;
  Fixed_mul(&c, &a, &b);
  printf("(2⁻³²)²: "); printFixed(&c); putchar('\n');

  // Even smaller than that
  Fixed_mul(&d, &a, &c);
  printf("(2⁻³²)³: "); printFixed(&d); putchar('\n');

  // Underflow rounding
  Fixed_int2(&a, 1);
  Fixed_divu(&a, &a, 2);
  Fixed_mul(&c, &a, &d);
  printf("½(2⁻³²)³:"); printFixed(&d); putchar('\n');

  // Division
  Fixed_int2(&a, 1);
  Fixed_int2(&b, 16);
  Fixed_div(&c, &a, &b);
  printf("1/16:    "); printFixed(&c); putchar('\n');

  Fixed_int2(&a, 1);
  Fixed_int2(&b, 3);
  Fixed_div(&c, &a, &b);
  printf("1/3:     "); printFixed(&c); putchar('\n');

  Fixed_div(&c, &c, &b);
  printf("1/3/3:   "); printFixed(&c); putchar('\n');

  // Square roots
  Fixed_int2(&a, 2);
  Fixed_sqrt(&a, &a);
  printf("√2:      "); printFixed(&a); putchar('\n');
  Fixed_mul(&b, &a, &a);
  printf("(√2)²:   "); printFixed(&b); putchar('\n');

  Fixed_int2(&a, 1);
  Fixed_divu(&a, &a, 2);
  Fixed_sqrt(&a, &a);
  printf("√½:      "); printFixed(&a); putchar('\n');
  Fixed_mul(&b, &a, &a);
  printf("(√½)²:   "); printFixed(&b); putchar('\n');

  // Conversion to/from double
  Fixed_double2(&a, 1.0);
  printf("1.0:     "); printFixed(&a); putchar('\n');
  printf("and back: %g\n", Fixed_2double(&a));
  Fixed_double2(&a, 0.5);
  printf("0.5:     "); printFixed(&a); putchar('\n');
  printf("and back: %g\n", Fixed_2double(&a));
  Fixed_double2(&a, M_PI);
  printf("π:       "); printFixed(&a); putchar('\n');
  printf("and back: %g\n", Fixed_2double(&a));

  return 0;
}
