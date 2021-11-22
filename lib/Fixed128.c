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

#define UINT128_MAX                                                            \
  ((uint128_t)0xFFFFFFFFFFFFFFFF + ((uint128_t)0xFFFFFFFFFFFFFFFF << 64))
#define UNDERFLOW_MASK (UINT128_MAX >> 32)

// add b to *r, setting *overflow if the result is too large
static inline void add_with_overflow(int *overflow, uint128_t *r, uint128_t b) {
  if(*r > UINT128_MAX - b)
    *overflow |= 1;
  *r += b;
}

static int Fixed128_mul_unsigned(union Fixed128 *r, const union Fixed128 *a,
                                 const union Fixed128 *b) {
  // Compute the four partial products. Recall representation is
  // little-endian.
  uint128_t hh = (uint128_t)a->u64[1] * (uint128_t)b->u64[1];
  uint128_t hl = (uint128_t)a->u64[1] * (uint128_t)b->u64[0];
  uint128_t lh = (uint128_t)a->u64[0] * (uint128_t)b->u64[1];
  uint128_t ll = (uint128_t)a->u64[0] * (uint128_t)b->u64[0];
  // Overall we have a 256-bit product.
  //
  // The top 32 bits are always too big; overflow is set
  // if we impinge on them.
  //
  // The bottom 96 bits will accumulate in u.
  int overflow = hh > ((uint128_t)1 << 96) - 1;
  uint128_t result = hh << 32;
  add_with_overflow(&overflow, &result, hl >> 32);
  uint128_t u = (hl << 64) & UNDERFLOW_MASK;
  add_with_overflow(&overflow, &result, lh >> 32);
  u += (lh << 64) & UNDERFLOW_MASK;
  add_with_overflow(&overflow, &result, ll >> 96);
  u += ll & UNDERFLOW_MASK;
  // If the top bit of the underflow is nonzero, round up.
  if(u & ((uint128_t)1 << 95))
    u += ((uint128_t)1 << 96);
  add_with_overflow(&overflow, &result, u >> 96);
  r->u128 = result;
  return overflow;
}

int Fixed128_mul(union Fixed128 *r, const union Fixed128 *a,
                 const union Fixed128 *b) {
  union Fixed128 aa = {0}, bb = {0};
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

void Fixed128_divu(union Fixed128 *r, const union Fixed128 *a, unsigned u) {
  uint64_t quot, rem = 0, d;
  int n;

  // Sort out the sign
  if(Fixed128_lt0(a)) {
    union Fixed128 aa;
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

char *Fixed128_2str(char buffer[], unsigned bufsize, const union Fixed128 *a,
                    int base) {
#define ADDCHAR(C)                                                             \
  do {                                                                         \
    if(i < bufsize - 1)                                                        \
      buffer[i++] = (C);                                                       \
  } while(0)

  static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

  size_t i = 0;
  union Fixed128 n = *a;
  union Fixed128 radix;
  uint32_t u;
  char ipart[130];      // Buffer for integer part
  int j = sizeof ipart; // Start at the least significant digit and work back

  Fixed128_int2(&radix, base);
  // Sort out the sign
  if(Fixed128_lt0(&n)) {
    Fixed128_neg(&n, &n);
    ADDCHAR('-');
  }
  // Work out the integer part
  u = n.word[NFIXED128 - 1];
  do {
    ipart[--j] = digits[u % base];
    u /= base;
  } while(u);
  // Add it to the buffer
  while(j < (int)sizeof ipart)
    ADDCHAR(ipart[j++]);
  // No more integer part
  n.word[NFIXED128 - 1] = 0;
  if(!Fixed128_eq0(&n)) {
    // Non-zero fractional part
    ADDCHAR('.');
    do {
      // Multiply by the radix so that the next digit comes into
      // the integer part.
      Fixed128_mul(&n, &n, &radix);
      ADDCHAR(digits[n.word[NFIXED128 - 1]]);
      n.word[NFIXED128 - 1] = 0;
    } while(!Fixed128_eq0(&n));
  }
  buffer[i] = 0;
  return buffer;
}

static void Fixed128_div_unsigned(union Fixed128 *r, const union Fixed128 *a,
                                  const union Fixed128 *b) {
  union Fixed128 rem = *a, sub = *b, quot;
  int bit;
  bit = 0;
  while(Fixed128_lt(&sub, &rem)) {
    Fixed128_shl_unsigned(&sub);
    ++bit;
  }
  Fixed128_int2(&quot, 0);
  while(bit >= -NFRACBITS) {
    union Fixed128 diff;
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

void Fixed128_div(union Fixed128 *r, const union Fixed128 *a,
                  const union Fixed128 *b) {
  union Fixed128 aa = {0}, bb = {0};
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

void Fixed128_sqrt(union Fixed128 *r, const union Fixed128 *a) {
  // Slow and naive bit-by-bit algorithm
  union Fixed128 result, product;
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
          break; /* Exact answer! */
                 /* Keep that bit */
                 // printf("   YES\n");
      } else {
        result.word[n] ^= bit;
        // printf("   no.\n");
      }
    }
  }
  *r = result;
  // TODO we always round down, we need another bit
}

void Fixed128_double2(union Fixed128 *r, double n) {
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

double Fixed128_2double(const union Fixed128 *a) {
  if(Fixed128_lt0(a)) {
    union Fixed128 b;
    Fixed128_neg(&b, a);
    return -Fixed128_2double(&b);
  }
  return (a->word[NFIXED128 - 4] / 79228162514264337593543950336.0
          + a->word[NFIXED128 - 3] / 18446744073709551616.0
          + a->word[NFIXED128 - 2] / 4294967296.0 + a->word[NFIXED128 - 1]);
}

long double Fixed128_2longdouble(const union Fixed128 *a) {
  if(Fixed128_lt0(a)) {
    union Fixed128 b;
    Fixed128_neg(&b, a);
    return -Fixed128_2longdouble(&b);
  }
  return (a->word[NFIXED128 - 4] / 79228162514264337593543950336.0L
          + a->word[NFIXED128 - 3] / 18446744073709551616.0L
          + a->word[NFIXED128 - 2] / 4294967296.0L + a->word[NFIXED128 - 1]);
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
