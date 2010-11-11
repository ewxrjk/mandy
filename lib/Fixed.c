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
#include <config.h>
#include "Fixed.h"
#include <stdio.h>
#include <math.h>

#if !(HAVE_ASM && NFIXED == 4)
void Fixed_add(struct Fixed *r, const struct Fixed *a, const struct Fixed *b) {
  uint64_t s = 0;
  int n;

  for(n = 0; n < NFIXED; ++n) {
    s = s + a->word[n] + b->word[n];
    r->word[n] = s;
    s >>= 32;
  }
}
#endif

#if !(HAVE_ASM && NFIXED == 4)
void Fixed_sub(struct Fixed *r, const struct Fixed *a, const struct Fixed *b) {
  uint64_t s = 1;
  int n;

  for(n = 0; n < NFIXED; ++n) {
    s = s + a->word[n] + (b->word[n] ^ 0xFFFFFFFF);
    r->word[n] = s;
    s >>= 32;
  }
}
#endif

#if !(HAVE_ASM && NFIXED == 4)
int Fixed_neg(struct Fixed *r, const struct Fixed *a) {
  uint64_t s = 1;
  int n;
  uint32_t sign = a->word[NFIXED - 1] & 0x80000000;

  for(n = 0; n < NFIXED; ++n) {
    s = s + (a->word[n] ^ 0xFFFFFFFF);
    r->word[n] = s;
    s >>= 32;
  }
  if(sign && (r->word[NFIXED - 1] & 0x80000000))
    return 1;
  else
    return 0;
}
#endif

#if !(HAVE_ASM && NFIXED == 4)
static int Fixed_mul_unsigned(struct Fixed *r, const struct Fixed *a, const struct Fixed *b) {
  int n, m, i;
  /* Clear result accumulator */
  int overflow = 0;
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
      if(p)
	overflow = 1;
    }
  }
  if(result[NFIXED - 1] > 0x80000000) {
    uint64_t s = 1;
    for(n = NFIXED; n < NFIXED; ++n) {
      s = result[n] + s;
      result[n] = s;
      s >>= 32;
    }
    if(s)
      overflow = 1;
  }
  for(n = 0; n < NFIXED; ++n)
    r->word[n] = result[NFIXED + n];
  return overflow;
}

int Fixed_mul(struct Fixed *r, const struct Fixed *a, const struct Fixed *b) {
  struct Fixed aa, bb;
  int sign = 0, overflow = 0;
  /* Sort out sign */
  if(Fixed_lt0(a)) {
    overflow |= Fixed_neg(&aa, a);
    a = &aa;
    sign = !sign;
  }
  if(Fixed_lt0(b)) {
    overflow |= Fixed_neg(&bb, b);
    b = &bb;
    sign = !sign;
  }
  overflow |= Fixed_mul_unsigned(r, a, b);
  if(sign)
    overflow |= Fixed_neg(r, r);
  return overflow;
}
#endif

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
  uint32_t u = n.word[NFIXED - 1];
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

int Fixed_lt(const struct Fixed *a, const struct Fixed *b) {
  int n;
  if(a->word[NFIXED - 1] != b->word[NFIXED - 1])
    return (int32_t)a->word[NFIXED - 1] < (int32_t)b->word[NFIXED - 1];
  for(n = NFIXED - 2; n >= 0; --n)
    if(a->word[n] != b->word[n])
      return a->word[n] < b->word[n];
  return 0;
}

static int Fixed_lt_unsigned(const struct Fixed *a, const struct Fixed *b) {
  int n;
  for(n = NFIXED - 1; n >= 0; --n)
    if(a->word[n] != b->word[n])
      return a->word[n] < b->word[n];
  return 0;
}

static inline int Fixed_gt_unsigned(const struct Fixed *a, const struct Fixed *b) {
  return Fixed_lt_unsigned(b, a);
}

static inline int Fixed_le_unsigned(const struct Fixed *a, const struct Fixed *b) {
  return !Fixed_gt_unsigned(a, b);
}

static inline int Fixed_ge_unsigned(const struct Fixed *a, const struct Fixed *b) {
  return !Fixed_lt_unsigned(a, b);
}

int Fixed_eq(const struct Fixed *a, const struct Fixed *b) {
  int n;
  for(n = 0; n < NFIXED; ++n)
    if(a->word[n] != b->word[n])
      return 0;
  return 1;
}

#if !(HAVE_ASM && NFIXED == 4)
void Fixed_shl_unsigned(struct Fixed *a) {
  int n;
  for(n = NFIXED - 1; n > 0; --n)
    a->word[n] = (a->word[n] << 1) + !!(a->word[n-1] & 0x80000000);
  a->word[0] <<= 1;
}
#endif

#if !(HAVE_ASM && NFIXED == 4)
void Fixed_shr_unsigned(struct Fixed *a) {
    int n;
    for(n = 0; n < NFIXED - 1; ++n)
      a->word[n] = (a->word[n] >> 1) + ((a->word[n+1] & 1) ? 0x80000000 : 0);
    a->word[NFIXED - 1] >>= 1;
}
#endif

void Fixed_setbit(struct Fixed *a, int bit) {
  if(bit >= 0)
    a->word[NFIXED-1] |= 1 << bit;
  else {
    // bits -1..-32 are te first word; -33..-64 the second; etc.
    int word = NFIXED - 2 - -(bit+1) / 32;
    bit = bit & 31;
    a->word[word] |= 1 << bit;
  }
}

static void Fixed_div_unsigned(struct Fixed *r, const struct Fixed *a, const struct Fixed *b) {
  struct Fixed rem = *a, sub = *b, quot;
  int bit;
  bit = 0;
  while(Fixed_lt(&sub, &rem)) {
    Fixed_shl_unsigned(&sub);
    ++bit;
  }
  Fixed_int2(&quot, 0);
  while(bit >= -NFRACBITS) {
    struct Fixed diff;
    Fixed_sub(&diff, &rem, &sub);
    if(Fixed_ge0(&diff)) {
      rem = diff;
      Fixed_setbit(&quot, bit);
      if(Fixed_eq0(&diff))
        break;
    }
    Fixed_shr_unsigned(&sub);
    --bit;
  }
  *r = quot;
  // TODO we always round down, we need another bit
}

void Fixed_div(struct Fixed *r, const struct Fixed *a, const struct Fixed *b) {
  struct Fixed aa, bb;
  int sign = 0;
  /* Sort out sign */
  if(Fixed_lt0(a)) {
    Fixed_neg(&aa, a);
    a = &aa;
    sign = !sign;
  }
  if(Fixed_lt0(b)) {
    Fixed_neg(&bb, b);
    b = &bb;
    sign = !sign;
  }
  Fixed_div_unsigned(r, a, b);
  if(sign)
    Fixed_neg(r, r);
}

void Fixed_sqrt(struct Fixed *r, const struct Fixed *a) {
  // Slow and naive bit-by-bit algorithm
  struct Fixed result, product;
  int n;
  uint32_t bit;
  Fixed_int2(&result, 0);
  for(n = NFIXED - 1; n >= 0; --n) {
    for(bit = 1 << 31; bit > 0; bit >>= 1) {
      result.word[n] |= bit;
      int overflow = Fixed_mul(&product, &result, &result);
      /*
      {
	char rbuf[256], pbuf[256], abuf[256], bbuf[256];
	Fixed_2str(rbuf, sizeof rbuf, &result, 16);
	Fixed_2str(pbuf, sizeof pbuf, &product, 16);
	Fixed_2str(abuf, sizeof abuf, a, 16);
	Fixed_2str(bbuf, sizeof bbuf, b, 16);
	printf("%d:%08x: 0x%s * 0x%s -> 0x%s%s <= 0x%s?\n",
	       n, bit, bbuf, rbuf, pbuf,
	       overflow ? " [OVERFLOW]" : "",
	       abuf);
      }
      */
      if(!overflow && Fixed_le_unsigned(&product, a)) {
	if(Fixed_eq(&product, a))
	  break;		/* Exact answer! */
	/* Keep that bit */
	//printf("   ...keep!\n");
      } else {
	result.word[n] ^= bit;
      }
    }
  }
  *r = result;
  // TODO we always round down, we need another bit
}

void Fixed_double2(struct Fixed *r, double n) {
  Fixed_int2(r, 0);
  if(n < 0) {
    Fixed_double2(r, -n);
    Fixed_neg(r, r);
    return;
  }
  int i = NFIXED - 1;
  while(n && i >= 0) {
    double ipart = floor(n);
    r->word[i] = ipart;
    n -= ipart;
    n *= 4294967296.0;
    --i;
  }
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
