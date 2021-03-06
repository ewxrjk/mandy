/* Copyright © 2010, 2012 Richard Kettlewell.
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
#include "Fixed128.h"
#include <stdio.h>
#include <math.h>

#if !HAVE_ASM_128
void Fixed128_add(struct Fixed128 *r, const struct Fixed128 *a, const struct Fixed128 *b) {
#if HAVE___INT128
  unsigned __int128 aa = *(unsigned __int128 *)a;
  unsigned __int128 bb = *(unsigned __int128 *)b;
  *(unsigned __int128 *)r = aa + bb;
#else
  uint64_t s = 0;
  int n;

  for(n = 0; n < NFIXED128; ++n) {
    s = s + a->word[n] + b->word[n];
    r->word[n] = (uint32_t)s;
    s >>= 32;
  }
#endif
}

void Fixed128_sub(struct Fixed128 *r, const struct Fixed128 *a, const struct Fixed128 *b) {
#if HAVE___INT128
  unsigned __int128 aa = *(unsigned __int128 *)a;
  unsigned __int128 bb = *(unsigned __int128 *)b;
  *(unsigned __int128 *)r = aa - bb;
#else
  uint64_t s = 1;
  int n;

  for(n = 0; n < NFIXED128; ++n) {
    s = s + a->word[n] + (b->word[n] ^ 0xFFFFFFFF);
    r->word[n] = (uint32_t)s;
    s >>= 32;
  }
#endif
}

int Fixed128_neg(struct Fixed128 *r, const struct Fixed128 *a) {
  uint32_t sign = a->word[NFIXED128 - 1] & 0x80000000;
#if HAVE___INT128
  *(unsigned __int128 *)r = -*(unsigned __int128 *)a;
#else
  uint64_t s = 1;
  int n;

  for(n = 0; n < NFIXED128; ++n) {
    s = s + (a->word[n] ^ 0xFFFFFFFF);
    r->word[n] = (uint32_t)s;
    s >>= 32;
  }
#endif
  if(sign && (r->word[NFIXED128 - 1] & 0x80000000))
    return 1;
  else
    return 0;
}

static int Fixed128_mul_unsigned(struct Fixed128 *r, const struct Fixed128 *a, const struct Fixed128 *b) {
  int n, m, i;
  /* Clear result accumulator */
  int overflow = 0;
  uint32_t result[2 * NFIXED128];
  for(n = 0; n < NFIXED128 * 2; ++n)
    result[n] = 0;
  for(n = 0; n < NFIXED128; ++n) {
    if(!a->word[NFIXED128 - 1 - n])
      continue;
    for(m = 0; m < NFIXED128; ++m) {
      uint64_t p = (uint64_t)a->word[NFIXED128 - 1 - n] * b->word[NFIXED128 - 1 - m];
      /*
      printf("  mul %d %d[%d]: %08x * %08x -> %016llx\n", n, m,
             2 * NFIXED128 - 1 - (n + m),
	     a->word[NFIXED128 - 1 - n], b->word[NFIXED128 - 1 - m],
	     p);
      */
      for(i = 2 * NFIXED128 - 1 - (n + m); p && i < 2 * NFIXED128; ++i) {
	uint64_t s = result[i] + p;
	/*printf("    mul %d -> %08lx + %016llx = %016llx\n", i, result[i], p, s);*/
	result[i] = (uint32_t)s;
	p = s >> 32;
      }
      if(p) {
        /*printf("    mul leftover product: %016llx  *************************************\n", p);*/
	overflow = 1;
      }
    }
  }
  /*for(n = 2 * NFIXED128-1; n >= 0; --n) {
    printf("%08lx%c", result[n], n == 2 * NFIXED128-1 ? '.' : ' ');
  }
  printf("\n");*/
  if(result[NFIXED128 - 1] > 0x80000000) {
    uint64_t s = 1;
    for(n = NFIXED128; n < 2 * NFIXED128; ++n) {
      s = result[n] + s;
      result[n] = (uint32_t)s;
      s >>= 32;
    }
    if(s) {
      overflow = 1;
      /*printf("    mul leftover carry: %016llx  *******************************\n", s);*/
    }
  }
  for(n = 0; n < NFIXED128; ++n)
    r->word[n] = result[NFIXED128 + n];
  return overflow;
}

int Fixed128_mul(struct Fixed128 *r, const struct Fixed128 *a, const struct Fixed128 *b) {
  struct Fixed128 aa, bb;
  int sign = 0, overflow = 0;
  /* Sort out sign */
  if(Fixed128_lt0(a)) {
    overflow |= Fixed128_neg(&aa, a);
    a = &aa;
    sign = !sign;
  }
  if(Fixed128_lt0(b)) {
    overflow |= Fixed128_neg(&bb, b);
    b = &bb;
    sign = !sign;
  }
  overflow |= Fixed128_mul_unsigned(r, a, b);
  if(sign)
    overflow |= Fixed128_neg(r, r);
  return overflow;
}
#endif

void Fixed128_divu(struct Fixed128 *r, const struct Fixed128 *a, unsigned u) {
  uint64_t quot, rem = 0, d;
  int n;

  if(Fixed128_lt0(a)) {
    struct Fixed128 aa;
    Fixed128_neg(&aa, a);
    Fixed128_divu(r, &aa, u);
    Fixed128_neg(r, r);
    return;
  }
  for(n = NFIXED128 - 1; n >= 0; --n) {
    d = a->word[n] + (rem << 32);
    quot = d / u;
    rem = d % u;
    r->word[n] = (uint32_t)quot;
  }
}

void Fixed128_int2(struct Fixed128 *r, int i) {
  int n;
  r->word[NFIXED128 - 1] = (uint32_t)i;
  for(n = NFIXED128 - 2; n >= 0; --n)
    r->word[n] = 0;
}

int Fixed128_eq0(const struct Fixed128 *a) {
  int n;

  for(n = 0; n < NFIXED128; ++n)
    if(a->word[n])
      return 0;
  return 1;
}

char *Fixed128_2str(char buffer[], unsigned bufsize, const struct Fixed128 *a,
                 int base) {
#define ADDCHAR(C) do {				\
  if(i < bufsize - 1)				\
    buffer[i++] = (C);				\
} while(0)

  static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

  size_t i = 0;
  struct Fixed128 n = *a;
  struct Fixed128 radix;
  uint32_t u;
  char ipart[130];
  int j = sizeof ipart;

  Fixed128_int2(&radix, base);
  if(Fixed128_lt0(&n)) {
    Fixed128_neg(&n, &n);
    ADDCHAR('-');
  }
  u = n.word[NFIXED128 - 1];
  do {
    ipart[--j] = digits[u % base];
    u /= base;
  } while(u);
  while(j < (int)sizeof ipart)
    ADDCHAR(ipart[j++]);
  n.word[NFIXED128 - 1] = 0;
  if(!Fixed128_eq0(&n)) {
    ADDCHAR('.');
    do {
      Fixed128_mul(&n, &n, &radix);
      ADDCHAR(digits[n.word[NFIXED128 - 1]]);
      n.word[NFIXED128 - 1] = 0;
    } while(!Fixed128_eq0(&n));
  }
  buffer[i] = 0;
  return buffer;
}

int Fixed128_lt(const struct Fixed128 *a, const struct Fixed128 *b) {
  int n;
  if(a->word[NFIXED128 - 1] != b->word[NFIXED128 - 1])
    return (int32_t)a->word[NFIXED128 - 1] < (int32_t)b->word[NFIXED128 - 1];
  for(n = NFIXED128 - 2; n >= 0; --n)
    if(a->word[n] != b->word[n])
      return a->word[n] < b->word[n];
  return 0;
}

static int Fixed128_lt_unsigned(const struct Fixed128 *a, const struct Fixed128 *b) {
  int n;
  for(n = NFIXED128 - 1; n >= 0; --n)
    if(a->word[n] != b->word[n])
      return a->word[n] < b->word[n];
  return 0;
}

static inline int Fixed128_gt_unsigned(const struct Fixed128 *a, const struct Fixed128 *b) {
  return Fixed128_lt_unsigned(b, a);
}

static inline int Fixed128_le_unsigned(const struct Fixed128 *a, const struct Fixed128 *b) {
  return !Fixed128_gt_unsigned(a, b);
}

int Fixed128_eq(const struct Fixed128 *a, const struct Fixed128 *b) {
  int n;
  for(n = 0; n < NFIXED128; ++n)
    if(a->word[n] != b->word[n])
      return 0;
  return 1;
}

#if !HAVE_ASM_128
void Fixed128_shl_unsigned(struct Fixed128 *a) {
  int n;
  for(n = NFIXED128 - 1; n > 0; --n)
    a->word[n] = (a->word[n] << 1) + !!(a->word[n-1] & 0x80000000);
  a->word[0] <<= 1;
}
#endif

#if !HAVE_ASM_128
void Fixed128_shr_unsigned(struct Fixed128 *a) {
    int n;
    for(n = 0; n < NFIXED128 - 1; ++n)
      a->word[n] = (a->word[n] >> 1) + ((a->word[n+1] & 1) ? 0x80000000 : 0);
    a->word[NFIXED128 - 1] >>= 1;
}
#endif

void Fixed128_setbit(struct Fixed128 *a, int bit) {
  if(bit >= 0)
    a->word[NFIXED128-1] |= (uint32_t)1 << bit;
  else {
    // bits -1..-32 are the first word; -33..-64 the second; etc.
    int word = NFIXED128 - 2 - -(bit+1) / 32;
    bit = bit & 31;
    a->word[word] |= (uint32_t)1 << bit;
  }
}

static void Fixed128_div_unsigned(struct Fixed128 *r, const struct Fixed128 *a, const struct Fixed128 *b) {
  struct Fixed128 rem = *a, sub = *b, quot;
  int bit;
  bit = 0;
  while(Fixed128_lt(&sub, &rem)) {
    Fixed128_shl_unsigned(&sub);
    ++bit;
  }
  Fixed128_int2(&quot, 0);
  while(bit >= -NFRACBITS) {
    struct Fixed128 diff;
    Fixed128_sub(&diff, &rem, &sub);
    if(Fixed128_ge0(&diff)) {
      rem = diff;
      Fixed128_setbit(&quot, bit);
      if(Fixed128_eq0(&diff))
        break;
    }
    Fixed128_shr_unsigned(&sub);
    --bit;
  }
  *r = quot;
  // TODO we always round down, we need another bit
}

void Fixed128_div(struct Fixed128 *r, const struct Fixed128 *a, const struct Fixed128 *b) {
  struct Fixed128 aa, bb;
  int sign = 0;
  /* Sort out sign */
  if(Fixed128_lt0(a)) {
    Fixed128_neg(&aa, a);
    a = &aa;
    sign = !sign;
  }
  if(Fixed128_lt0(b)) {
    Fixed128_neg(&bb, b);
    b = &bb;
    sign = !sign;
  }
  Fixed128_div_unsigned(r, a, b);
  if(sign)
    Fixed128_neg(r, r);
}

void Fixed128_sqrt(struct Fixed128 *r, const struct Fixed128 *a) {
  // Slow and naive bit-by-bit algorithm
  struct Fixed128 result, product;
  int n, overflow;
  uint32_t bit;
  Fixed128_int2(&result, 0);
  for(n = NFIXED128 - 1; n >= 0; --n) {
    for(bit = (uint32_t)1 << 31; bit > 0; bit >>= 1) {
      result.word[n] |= bit;
      overflow = Fixed128_mul(&product, &result, &result);
      /*
      {
	char rbuf[256], pbuf[256], abuf[256];
	Fixed128_2str(rbuf, sizeof rbuf, &result, 16);
	Fixed128_2str(pbuf, sizeof pbuf, &product, 16);
	Fixed128_2str(abuf, sizeof abuf, a, 16);
	printf("%d:%08x: 0x%s * 0x%s -> 0x%s%s <= 0x%s?  ",
	       n, bit, rbuf, rbuf, pbuf,
	       overflow ? " [OVERFLOW]" : "",
	       abuf);
      }
      */
      if(!overflow && Fixed128_le_unsigned(&product, a)) {
	if(Fixed128_eq(&product, a))
	  break;		/* Exact answer! */
	/* Keep that bit */
	//printf("   YES\n");
      } else {
	result.word[n] ^= bit;
        //printf("   no.\n");
      }
    }
  }
  *r = result;
  // TODO we always round down, we need another bit
}

void Fixed128_double2(struct Fixed128 *r, double n) {
  int i = NFIXED128 - 1;
  Fixed128_int2(r, 0);
  if(n < 0) {
    Fixed128_double2(r, -n);
    Fixed128_neg(r, r);
    return;
  }
  while(n && i >= 0) {
    double ipart = floor(n);
    r->word[i] = (uint32_t)ipart;
    n -= ipart;
    n *= 4294967296.0;
    --i;
  }
}

double Fixed128_2double(const struct Fixed128 *a) {
  if(Fixed128_lt0(a)) {
    struct Fixed128 b;
    Fixed128_neg(&b, a);
    return -Fixed128_2double(&b);
  }
  return (a->word[NFIXED128 - 4] / 79228162514264337593543950336.0
          + a->word[NFIXED128 - 3] / 18446744073709551616.0
          + a->word[NFIXED128 - 2] / 4294967296.0
          + a->word[NFIXED128 - 1]);
}

long double Fixed128_2longdouble(const struct Fixed128 *a) {
  if(Fixed128_lt0(a)) {
    struct Fixed128 b;
    Fixed128_neg(&b, a);
    return -Fixed128_2longdouble(&b);
  }
  return (a->word[NFIXED128 - 4] / 79228162514264337593543950336.0L
          + a->word[NFIXED128 - 3] / 18446744073709551616.0L
          + a->word[NFIXED128 - 2] / 4294967296.0L
          + a->word[NFIXED128 - 1]);
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
