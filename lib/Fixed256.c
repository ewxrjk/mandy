/* Copyright © Richard Kettlewell.
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
#include "Fixed64.h"
#include "Fixed128.h"
#include "Fixed256.h"
#include <stdio.h>
#include <math.h>

// Force-inline so that Fixed256_square doesn't have to do so much work
static __always_inline void Fixed256_mul_unsigned(union Fixed256 *r, const union Fixed256 *a, const union Fixed256 *b) {
  uint64_t c = 0;

#define A0 (uint32_t)a->u16[0]
#define A1 (uint32_t)a->u16[1]
#define A2 (uint32_t)a->u16[2]
#define A3 (uint32_t)a->u16[3]
#define A4 (uint32_t)a->u16[4]
#define A5 (uint32_t)a->u16[5]
#define A6 (uint32_t)a->u16[6]
#define A7 (uint32_t)a->u16[7]
#define A8 (uint32_t)a->u16[8]
#define A9 (uint32_t)a->u16[9]
#define A10 (uint32_t)a->u16[10]
#define A11 (uint32_t)a->u16[11]
#define A12 (uint32_t)a->u16[12]
#define A13 (uint32_t)a->u16[13]
#define A14 (uint32_t)a->u16[14]
#define A15 (uint32_t)a->u16[15]

#define B0 (uint32_t)b->u16[0]
#define B1 (uint32_t)b->u16[1]
#define B2 (uint32_t)b->u16[2]
#define B3 (uint32_t)b->u16[3]
#define B4 (uint32_t)b->u16[4]
#define B5 (uint32_t)b->u16[5]
#define B6 (uint32_t)b->u16[6]
#define B7 (uint32_t)b->u16[7]
#define B8 (uint32_t)b->u16[8]
#define B9 (uint32_t)b->u16[9]
#define B10 (uint32_t)b->u16[10]
#define B11 (uint32_t)b->u16[11]
#define B12 (uint32_t)b->u16[12]
#define B13 (uint32_t)b->u16[13]
#define B14 (uint32_t)b->u16[14]
#define B15 (uint32_t)b->u16[15]

  // Units are the top 64 bits, = top 4 16-bit words, so A12/B12
  // So word n gets a contribution from Ax,By where x+y=n+12
  // Start with the hypothetical word n=-2 for a rounding bit

  c = (uint64_t)A0 * B10 + A1 * B9 + A2 * B8 + A3 * B7 + A4 * B6 + A5 * B5 + A6 * B4 + A7 * B3 + A8 * B2 + A9 * B1 + A10 * B0;
  c = (c >> 16) + ((c>>15) & 1);

  c += (uint64_t)A0 * B11 + A1 * B10 + A2 * B9 + A3 * B8 + A4 * B7 + A5 * B6 + A6 * B5 + A7 * B4 + A8 * B3 + A9 * B2 + A10 * B1 + A11 * B0;
  c = (c >> 16) + ((c>>15) & 1);

  c += (uint64_t)A0 * B12 + A1 * B11 + A2 * B10 + A3 * B9 + A4 * B8 + A5 * B7 + A6 * B6 + A7 * B5 + A8 * B4 + A9 * B3 + A10 * B2 + A11 * B1 + A12 * B0;
  r->u16[0] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A0 * B13 + A1 * B12 + A2 * B11 + A3 * B10 + A4 * B9 + A5 * B8 + A6 * B7 + A7 * B6 + A8 * B5 + A9 * B4 + A10 * B3 + A11 * B2 + A12 * B1 + A13 * B0;
  r->u16[1] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A0 * B14 + A1 * B13 + A2 * B12 + A3 * B11 + A4 * B10 + A5 * B9 + A6 * B8 + A7 * B7 + A8 * B6 + A9 * B5 + A10 * B4 + A11 * B3 + A12 * B2 + A13 * B1 + A14 * B0;
  r->u16[2] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A0 * B15 + A1 * B14 + A2 * B13 + A3 * B12 + A4 * B11 + A5 * B10 + A6 * B9 + A7 * B8 + A8 * B7 + A9 * B6 + A10 * B5 + A11 * B4 + A12 * B3 + A13 * B2 + A14 * B1 + A15 * B0;
  r->u16[3] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A1 * B15 + A2 * B14 + A3 * B13 + A4 * B12 + A5 * B11 + A6 * B10 + A7 * B9 + A8 * B8 + A9 * B7 + A10 * B6 + A11 * B5 + A12 * B4 + A13 * B3 + A14 * B2 + A15 * B1;
  r->u16[4] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A2 * B15 + A3 * B14 + A4 * B13 + A5 * B12 + A6 * B11 + A7 * B10 + A8 * B9 + A9 * B8 + A10 * B7 + A11 * B6 + A12 * B5 + A13 * B4 + A14 * B3 + A15 * B2;
  r->u16[5] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A3 * B15 + A4 * B14 + A5 * B13 + A6 * B12 + A7 * B11 + A8 * B10 + A9 * B9 + A10 * B8 + A11 * B7 + A12 * B6 + A13 * B5 + A14 * B4 + A15 * B3;
  r->u16[6] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A4 * B15 + A5 * B14 + A6 * B13 + A7 * B12 + A8 * B11 + A9 * B10 + A10 * B9 + A11 * B8 + A12 * B7 + A13 * B6 + A14 * B5 + A15 * B4;
  r->u16[7] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A5 * B15 + A6 * B14 + A7 * B13 + A8 * B12 + A9 * B11 + A10 * B10 + A11 * B9 + A12 * B8 + A13 * B7 + A14 * B6 + A15 * B5;
  r->u16[8] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A6 * B15 + A7 * B14 + A8 * B13 + A9 * B12 + A10 * B11 + A11 * B10 + A12 * B9 + A13 * B8 + A14 * B7 + A15 * B6;
  r->u16[9] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A7 * B15 + A8 * B14 + A9 * B13 + A10 * B12 + A11 * B11 + A12 * B10 + A13 * B9 + A14 * B8 + A15 * B7;
  r->u16[10] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A8 * B15 + A9 * B14 + A10 * B13 + A11 * B12 + A12 * B11 + A13 * B10 + A14 * B9 + A15 * B8;
  r->u16[11] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A9 * B15 + A10 * B14 + A11 * B13 + A12 * B12 + A13 * B11 + A14 * B10 + A15 * B9;
  r->u16[12] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A10 * B15 + A11 * B14 + A12 * B13 + A13 * B12 + A14 * B11 + A15 * B10;
  r->u16[13] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A11 * B15 + A12 * B14 + A13 * B13 + A14 * B12 + A15 * B11;
  r->u16[14] = (uint16_t)c;
  c >>= 16;

  c += (uint64_t)A12 * B15 + A13 * B14 + A14 * B13 + A15 * B12;
  r->u16[15] = (uint16_t)c;
  c >>= 16;

}

#if !HAVE_ASM_AMD64
void Fixed256_mul(union Fixed256 *r, const union Fixed256 *a, const union Fixed256 *b) {
  int sign = 0;
  union Fixed256 ap, bp;

  if(a->u32[7] & 0x80000000) {
    Fixed256_neg(&ap, a);
    sign ^= 1;
  } else
    ap = *a;
  if(b->u32[7] & 0x80000000) {
    Fixed256_neg(&bp, b);
    sign ^= 1;
  } else
    bp = *b;
  Fixed256_mul_unsigned(r, &ap, &bp);
  if(sign)
    Fixed256_neg(r, r);
}

void Fixed256_square(union Fixed256 *r, const union Fixed256 *a) {
  int sign = 0;
  union Fixed256 ap;

  if(a->u32[7] & 0x80000000) {
    Fixed256_neg(&ap, a);
    sign ^= 1;
  } else
    ap = *a;
  // We hope the compiler will spot that half the multiplies are duplicates
  Fixed256_mul_unsigned(r, &ap, &ap);
}
#endif

char *Fixed256_2str(char buffer[], unsigned bufsize, const union Fixed256 *a, int base) {
#define ADDCHAR(C)                                                                                                     \
  do {                                                                                                                 \
    if(i < bufsize - 1)                                                                                                \
      buffer[i++] = (C);                                                                                               \
  } while(0)

  static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

  size_t i = 0;
  union Fixed256 n = *a;
  union Fixed256 radix;
  uint64_t u;
  char ipart[256];      // Buffer for integer part
  int j = sizeof ipart; // Start at the least significant digit and work back

  Fixed256_int2(&radix, base);
  // Sort out the sign
  if(Fixed256_lt0(&n)) {
    Fixed256_neg(&n, &n);
    ADDCHAR('-');
  }
  // Work out the integer part
  u = n.u64[3];
  do {
    ipart[--j] = digits[u % base];
    u /= base;
  } while(u);
  // Add it to the buffer
  while(j < (int)sizeof ipart)
    ADDCHAR(ipart[j++]);
  // No more integer part
  n.u64[3] = 0;
  if(!Fixed256_eq0(&n)) {
    // Non-zero fractional part
    ADDCHAR('.');
    do {
      // Multiply by the radix so that the next digit comes into
      // the integer part.
      Fixed256_mul(&n, &n, &radix);
      ADDCHAR(digits[n.u64[3]]);
      n.u64[3] = 0;
    } while(!Fixed256_eq0(&n));
  }
  buffer[i] = 0;
  return buffer;
}

static void Fixed256_div_unsigned(union Fixed256 *r, const union Fixed256 *a, const union Fixed256 *b) {
  union Fixed256 rem = *a, sub = *b, quot;
  int bit;
  bit = 0;
  while(Fixed256_lt(&sub, &rem)) {
    Fixed256_shl_unsigned(&sub);
    ++bit;
  }
  Fixed256_int2(&quot, 0);
  while(bit >= -NFRACBITS256) {
    union Fixed256 diff;
    Fixed256_sub(&diff, &rem, &sub);
    if(Fixed256_ge0(&diff)) {
      rem = diff;
      Fixed256_setbit(&quot, bit);
      if(Fixed256_eq0(&diff))
        break;
    }
    Fixed256_shr_unsigned(&sub);
    --bit;
  }
  *r = quot;
  // TODO we always round down, we need another bit
}

void Fixed256_div(union Fixed256 *r, const union Fixed256 *a, const union Fixed256 *b) {
  union Fixed256 aa = {0}, bb = {0};
  int sign = 0;
  /* Sort out sign */
  if(Fixed256_lt0(a)) {
    Fixed256_neg(&aa, a);
    a = &aa;
    sign = !sign;
  }
  if(Fixed256_lt0(b)) {
    Fixed256_neg(&bb, b);
    b = &bb;
    sign = !sign;
  }
  Fixed256_div_unsigned(r, a, b);
  if(sign)
    Fixed256_neg(r, r);
}

void Fixed256_sqrt(union Fixed256 *r, const union Fixed256 *a) {
  // Slow and naive bit-by-bit algorithm
  union Fixed256 result = { .u32 = { 0 }}, product;
  int n;
  uint32_t bit;
  for(n = 6; n >= 0; --n) {
    for(bit = (uint32_t)1 << 31; bit > 0; bit >>= 1) {
      result.u32[n] |= bit;
      // TODO this could be optimized. We are calculating
      // (result+bit)^2 = result^2 + 2.bit.result + bit^2
      // but we already have result^2 from last time and 2.bit.result is just a shift.
      Fixed256_mul(&product, &result, &result);
      if(Fixed256_le_unsigned(&product, a)) {
        if(Fixed256_eq(&product, a))
          break; /* Exact answer! */
      } else {
        result.u32[n] ^= bit;
      }
    }
  }
  *r = result;
  // TODO we always round down, we need another bit
}

void Fixed256_double2(union Fixed256 *r, double n) {
  int i = 7;
  Fixed256_int2(r, 0);
  if(n < 0) {
    Fixed256_double2(r, -n);
    Fixed256_neg(r, r);
    return;
  }
  n /= 4294967296.0;
  while(n && i >= 0) {
    double ipart = floor(n);
    r->u32[i] = (uint32_t)ipart;
    n -= ipart;
    n *= 4294967296.0;
    --i;
  }
}

double Fixed256_2double(const union Fixed256 *a) {
  if(Fixed256_lt0(a)) {
    union Fixed256 b;
    Fixed256_neg(&b, a);
    return -Fixed256_2double(&b);
  }
  double r = 0.0;
  r += a->u32[7] * 4294967296.0;
  r += a->u32[6];
  r += a->u32[5] / 4294967296.0;
  r += a->u32[4] / 18446744073709551616.0;
  r += a->u32[3] / 79228162514264337593543950336.0;
  r += a->u32[2] / 340282366920938463463374607431768211456.0;
  r += a->u32[1] / 1461501637330902918203684832716283019655932542976.0;
  r += a->u32[0] / 6277101735386680763835789423207666416102355444464034512896.0;
  return r;
}

long double Fixed256_2longdouble(const union Fixed256 *a) {
  if(Fixed256_lt0(a)) {
    union Fixed256 b;
    Fixed256_neg(&b, a);
    return -Fixed256_2longdouble(&b);
  }
  double r = 0.0;
  r += a->u32[7] * 4294967296.0L;
  r += a->u32[6];
  r += a->u32[5] / 4294967296.0L;
  r += a->u32[4] / 18446744073709551616.0L;
  r += a->u32[3] / 79228162514264337593543950336.0L;
  r += a->u32[2] / 340282366920938463463374607431768211456.0L;
  r += a->u32[1] / 1461501637330902918203684832716283019655932542976.0L;
  r += a->u32[0] / 6277101735386680763835789423207666416102355444464034512896.0L;
  return r;
}

void Fixed256_to_Fixed64(Fixed64 *r, union Fixed256 *a) {
  *r = (a->u64[3] << 56) + (a->u64[2] >> 8);
}

void Fixed256_to_Fixed128(union Fixed128 *r, union Fixed256 *a) {
  r->word[0] = a->u32[3];
  r->word[1] = a->u32[4];
  r->word[2] = a->u32[5];
  r->word[3] = a->u32[6];
}

void Fixed128_to_Fixed256(union Fixed256 *r, union Fixed128 *a) {
  r->u64[0] = 0;
  r->u32[2] = 0;
  r->u32[3] = a->word[0];
  r->u32[4] = a->word[1];
  r->u32[5] = a->word[2];
  r->s64[3] = a->s32[3];
}
