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
#include "Fixed.h"
#include <stdio.h>

void Fixed_add(struct Fixed *r, const struct Fixed *a, const struct Fixed *b) {
  uint64_t s = 0;
  int n;

  for(n = 0; n < NFIXED; ++n) {
    s = s + a->word[n] + b->word[n];
    r->word[n] = s;
    s >>= 32;
  }
}

void Fixed_sub(struct Fixed *r, const struct Fixed *a, const struct Fixed *b) {
  uint64_t s = 0;
  int n;

  for(n = 0; n < NFIXED; ++n) {
    s = s + a->word[n] - b->word[n];
    r->word[n] = s;
    s >>= 32;
  }
}

void Fixed_neg(struct Fixed *r, const struct Fixed *a) {
  uint64_t s = 1;
  int n;

  for(n = 0; n < NFIXED; ++n) {
    s = s + (a->word[n] ^ 0xFFFFFFFF);
    r->word[n] = s;
    s >>= 32;
  }
}

void Fixed_mul(struct Fixed *r, const struct Fixed *a, const struct Fixed *b) {
  int n, m, i;
  /* Sort out sign */
  if(Fixed_lt0(a)) {
    if(Fixed_lt0(b)) {
      struct Fixed aa, bb;
      Fixed_neg(&aa, a);
      Fixed_neg(&bb, b);
      Fixed_mul(r, &aa, &bb);
      return;
    } else {
      struct Fixed aa;
      Fixed_neg(&aa, a);
      Fixed_mul(r, &aa, b);
      Fixed_neg(r, r);
      return;
    }
  } else {
    if(Fixed_lt0(b)) {
      struct Fixed bb;
      Fixed_neg(&bb, b);
      Fixed_mul(r, a, &bb);
      Fixed_neg(r, r);
      return;
    }
  }
  /* Clear result accumulator */
  uint32_t result[2 * NFIXED];
  for(n = 0; n < NFIXED * 2; ++n)
    result[n] = 0;
  for(n = 0; n < NFIXED; ++n) {
    for(m = 0; m < NFIXED; ++m) {
      uint64_t p = (uint64_t)a->word[NFIXED - 1 - n] * b->word[NFIXED - 1 - m];
      /*
      printf("%d %d: %08x * %08x -> %016llx\n", n, m,
	     a->word[NFIXED - 1 - n], b->word[NFIXED - 1 - m],
	     p);
      */
      for(i = 2 * NFIXED - 1 - (n + m); p && i < 2 * NFIXED; ++i) {
	uint64_t s = result[i] + p;
	//printf("  %d -> %016llx\n", i, s);
	result[i] = s;
	p = s >> 32;
      }
    }
  }
  if(result[NFIXED - 1] > 0x80000000) {
    uint64_t s = 1;
    for(n = NFIXED; n < NFIXED; ++n) {
      s = result[n] + s;
      result[n] = s;
      s >>= 32;
    }
  }
  for(n = 0; n < NFIXED; ++n)
    r->word[n] = result[NFIXED + n];
}

void Fixed_divu(struct Fixed *r, const struct Fixed *a, unsigned u) {
  uint64_t quot, rem = 0, d;
  int n;

  for(n = NFIXED - 1; n >= 0; --n) {
    d = a->word[n] + (rem << 32);
    quot = d / u;
    rem = d % u;
    r->word[n] = quot;
  }
}

void Fixed_int2(struct Fixed *r, int i) {
  int n;
  r->word[NFIXED - 1] = i;
  for(n = NFIXED - 2; n >= 0; --n)
    r->word[n] = 0;
}

int Fixed_eq0(const struct Fixed *a) {
  int n;

  for(n = 0; n < NFIXED; ++n)
    if(a->word[n])
      return 0;
  return 1;
}

void Fixed_2str(char buffer[], unsigned bufsize, const struct Fixed *a,
		int base) {
#define ADDCHAR(C) do {				\
  if(i < bufsize - 1)				\
    buffer[i++] = (C);				\
} while(0)

  static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";


  size_t i = 0;
  struct Fixed n = *a;
  struct Fixed radix;
  Fixed_int2(&radix, base);

  if(Fixed_lt0(&n)) {
    Fixed_neg(&n, &n);
    ADDCHAR('-');
  }

  char ipart[130];
  unsigned u = n.word[NFIXED - 1];
  int j = sizeof ipart;
  do {
    ipart[--j] = digits[u % base];
    u /= base;
  } while(u);
  while(j < (int)sizeof ipart)
    ADDCHAR(ipart[j++]);
  n.word[NFIXED - 1] = 0;
  if(!Fixed_eq0(&n)) {
    ADDCHAR('.');
    do {
      Fixed_mul(&n, &n, &radix);
      ADDCHAR(digits[n.word[NFIXED - 1]]);
      n.word[NFIXED - 1] = 0;
    } while(!Fixed_eq0(&n));
  }
  buffer[i] = 0;
}

/*
Local Variables:
mode:c
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
