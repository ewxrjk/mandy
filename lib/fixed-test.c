#include <config.h>
#include "Fixed.h"
#include <stdio.h>

void printFixed(const struct Fixed *f) {
  char buffer[128];
  int n;
  printf("%08x.%08x", f->word[NFIXED-1], f->word[NFIXED-2]);
  for(n = NFIXED-3; n >= 0; --n)
    printf(" %08x", f->word[n]);
  printf(" ");
  Fixed_2str(buffer, sizeof buffer, f, 10);
  printf("=%s", buffer);
}

int main() {
  struct Fixed a, b, c;

  // Divide by 2 across word boundary...
  Fixed_int2(&a, 1);
  Fixed_divu(&a, &a, 2);
  printf("0.5:   "); printFixed(&a); putchar('\n');
  // ...and within one word
  Fixed_divu(&a, &a, 4);
  printf("0.125: "); printFixed(&a); putchar('\n');

  // Add across word boundary
  Fixed_int2(&b, 1);
  Fixed_add(&c, &a, &b);
  printf("1.125: "); printFixed(&c); putchar('\n');

  // Add with carry
  Fixed_int2(&a, 1);
  Fixed_divu(&a, &a, 2);
  b = a;
  Fixed_add(&c, &a, &b);
  printf("1:     "); printFixed(&c); putchar('\n');

  // Multiply
  Fixed_int2(&a, 5);
  Fixed_int2(&b, 7);
  Fixed_mul(&c, &a, &b);
  printf("35:    "); printFixed(&c); putchar('\n');

  // Multiply by 0.5
  Fixed_int2(&a, 101);
  Fixed_int2(&b, 1);
  Fixed_divu(&b, &b, 2);
  Fixed_mul(&c, &a, &b);
  printf("50.5:  "); printFixed(&c); putchar('\n');

  // Very small numbers
  Fixed_int2(&a, 1);
  Fixed_divu(&a, &a, 65536);
  Fixed_divu(&a, &a, 65536);
  printf("2^-32: "); printFixed(&a); putchar('\n');
  b = a;
  Fixed_mul(&c, &a, &b);
  printf("2^-64: "); printFixed(&c); putchar('\n');

  // Division
  Fixed_int2(&a, 1);
  Fixed_int2(&b, 16);
  Fixed_div(&c, &a, &b);
  printf("1/16:  "); printFixed(&c); putchar('\n');

  Fixed_int2(&a, 1);
  Fixed_int2(&b, 3);
  Fixed_div(&c, &a, &b);
  printf("1/3:   "); printFixed(&c); putchar('\n');

  Fixed_div(&c, &c, &b);
  printf("1/3/3: "); printFixed(&c); putchar('\n');

  // Square roots
  Fixed_int2(&a, 2);
  Fixed_sqrt(&a, &a);
  printf("√2:    "); printFixed(&a); putchar('\n');
  Fixed_mul(&b, &a, &a);
  printf("(√2)²: "); printFixed(&b); putchar('\n');

  Fixed_int2(&a, 1);
  Fixed_divu(&a, &a, 2);
  Fixed_sqrt(&a, &a);
  printf("√½:    "); printFixed(&a); putchar('\n');
  Fixed_mul(&b, &a, &a);
  printf("(√½)²: "); printFixed(&b); putchar('\n');

  return 0;
}