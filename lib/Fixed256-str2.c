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
#include "Fixed256.h"
#include <string.h>
#include <ctype.h>
#include <errno.h>

/* The idea is to build up a long binary integer and then divide it down at
 * the end.
 *
 * How much space do we need?  The smallest possible number we can represent
 * is 2^-FRACBITS.  In decimal this has NFRACBITS256 digits after the point
 * (each time you multiply by 10 to read off another digit the rightmost bit
 * moves one place left, because 10 is even).  For the smallest number
 * there's a lot of initial 0s but you don't get that in the general case.
 *
 * Add to that the integer part, since we only ever use a single 32-bit word
 * for that we have at most 10 digits.  So the biggest integer we need to
 * represent is 10^(10+NFRACBITS256).
 *
 * In hex the computation is simpler: 16^(8+NFRACBITS256/4).  Simple experiment
 * reveals that this is much smaller (at least for realistic values of
 * NFRACBITS256).\
 *
 * So our integer needs at least
 *            lg(10^(10+NFRACBITS256))
 *          = log10(10^(10+NFRACBITS256))/log10(2)
 *          = (10+NFRACBITS256)/log10(2)
 *          ~ 3 * (10+NFRACBITS256)
 * bits or about (3 * (10+NFRACBITS256) + 31)/32 words.
 *
 * We are conservative in teh arithmetic, (+32 instead of +31) and add
 * a guard word to detect overflow.
 *
 * As with the main fixed-point representation the word order is
 * little-endian, the first words is units, the second is 2^32s, etc.
 */
#define NINTWORDS ((3 * (10 + NFRACBITS256) + 32) / 32 + 1)

static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

static int c2digit(int c, int base) {
  int value;
  const char *ptr = strchr(digits, c);
  if(!ptr)
    return -1;
  value = (int)(ptr - digits);
  if(value >= base)
    return -1;
  return value;
}

static void mla(uint32_t value[NINTWORDS], int base, int digit) {
  uint64_t c = digit;
  int n;
  for(n = 0; n < NINTWORDS; ++n) {
    c += base * (uint64_t)value[n];
    value[n] = (uint32_t)c;
    c >>= 32;
  }
}

static int sl(uint32_t value[NINTWORDS]) {
  int overflow = !!(value[NINTWORDS - 1] & 0x80000000);
  int n;
  for(n = NINTWORDS - 1; n > 0; --n)
    value[n] = (value[n] << 1) + !!(value[n - 1] & 0x80000000);
  value[0] <<= 1;
  return overflow;
}

static void sr(uint32_t value[NINTWORDS]) {
  int n;
  for(n = 0; n < NINTWORDS - 1; ++n)
    value[n] = (value[n] >> 1) + ((value[n + 1] & 1) ? 0x80000000 : 0);
  value[NINTWORDS - 1] >>= 1;
}

static int lt(const uint32_t a[NINTWORDS], const uint32_t b[NINTWORDS]) {
  int n;
  for(n = NINTWORDS - 1; n >= 0; --n)
    if(a[n] != b[n])
      return a[n] < b[n];
  return 0;
}

static inline int le(const uint32_t a[NINTWORDS], const uint32_t b[NINTWORDS]) {
  return !lt(b, a);
}

static int isZero(const uint32_t value[NINTWORDS]) {
  int n;
  for(n = 0; n < NINTWORDS; ++n)
    if(value[n])
      return 0;
  return 1;
}

static void sub(uint32_t r[NINTWORDS], const uint32_t a[NINTWORDS], const uint32_t b[NINTWORDS]) {
  uint64_t s = 1;
  int n;

  for(n = 0; n < NINTWORDS; ++n) {
    s = s + a[n] + (b[n] ^ 0xFFFFFFFF);
    r[n] = (uint32_t)s;
    s >>= 32;
  }
}

#if 0
static void print(const uint32_t value[NINTWORDS]) {
  int n;
  /* Find the most significant nonzero word */
  for(n = NINTWORDS-1; n > 0; --n)
    if(value[n])
      break;
  /* Print out words most significant first */
  for(; n >= 0; --n)
    printf("%08x", value[n]);
}
#endif

int Fixed256_str2(union Fixed256 *r, const char *start, char **endptr) {
  int base, error = 0, sign = 0, digit, scale = 0;
  uint32_t value[NINTWORDS];
  const char *s = start;
  Fixed256_int2(r, 0);
  // Consume leading whitespace
  while(isspace((unsigned char)*s))
    ++s;
  // Consume sign
  if(*s == '+')
    ++s;
  else if(*s == '-') {
    sign = 1;
    ++s;
  }
  // Set default base to match strtod() rules
  if(*s == '0' && (s[1] == 'x' || s[1] == 'X')) {
    s += 2;
    base = 16;
  } else
    base = 10;
  // There had better be a . or a valid digit
  if(!(*s == '.' || c2digit(*s, base) >= 0)) {
  noconversion:
    if(endptr)
      *endptr = (char *)start;
    return 0;
  }
  memset(value, 0, sizeof value);
  // Integer part
  while((digit = c2digit(*s, base)) >= 0) {
    if(!value[NINTWORDS - 1])
      mla(value, base, digit);
    else
      /* If the integer part would overflow we just bump the scale
       * instead.  The whole input might be something stupid like
       * 1000000..0000E-100.  We'll lose precision but we'll get an
       * answer. */
      ++scale;
    ++s;
  }
  if(*s == '.') {
    // Fractional part
    ++s;
    while((digit = c2digit(*s, base)) >= 0) {
      if(!value[NINTWORDS - 1]) {
        /* If the fractional part would overflow we just ditch the
         * extra digits.  We only lose precision thereby. */
        mla(value, base, digit);
        --scale;
      }
      ++s;
    }
  }
  if(*s == 'e' || *s == 'E') {
    int exponent = 0, expsign = 0;
    /* Exponent part.  (This only works for base<=15, and TODO the 'p'
     * form for a binary exponent is not implemented at all.) */
    ++s;
    if(*s == '+')
      ++s;
    else if(*s == '-') {
      ++s;
      expsign = 1;
    }
    if(!isdigit((unsigned char)*s))
      goto noconversion; /* must be at least 1 digit */
    while(isdigit((unsigned char)*s))
      exponent = 10 * exponent + *s++ - '0';
    if(expsign)
      exponent = -exponent;
    scale += exponent;
  }
  /* Apply the computed scale */
  if(scale < 0) {
    // TODO missing some overflow detection in here
    /* We need to divide down by base^-scale.  First compute the divisor. */
    uint32_t divisor[NINTWORDS];
    int bit = NFRACBITS256;
    memset(divisor, 0, sizeof divisor);
    divisor[0] = 1;
    while(scale < 0) {
      mla(divisor, base, 0);
      ++scale;
    }
    /* Shift the divisor up until it exceeds or equals the dividend,
     * keeping track of the bit number. */
    while(lt(divisor, value)) {
      if(sl(divisor)) {
        error = ERANGE;
        goto done;
      }
      ++bit;
      if(bit > 256)
        return -1;
    }
    /* Now we can start extracting bits.  The dividend will be the
     * remainder at each step. */
    while(bit >= 0 && !isZero(value)) {
      if(le(divisor, value)) {
        sub(value, value, divisor);
        r->u32[bit / 32] |= ((uint32_t)1 << (bit & 31));
      }
      /* Halve the divisor (by doubling the remainder if the divisor
       * is now odd) */
      if(divisor[0] & 1)
        sl(value);
      else
        sr(divisor);
      --bit;
    }
    if(bit == -1 && le(divisor, value)) {
      // Round up
      union Fixed256 round;
      memset(&round, 0, sizeof round);
      round.u32[0] = 1;
      Fixed256_add(r, r, &round);
    }
  } else if(scale >= 0) {
    /* The result will just be an integer.  It had better fit (with
     * its sign) into 32 bits. */
    int i;
    uint64_t n;
    uint32_t limit = sign ? 0x80000000 : 0x7FFFFFFF;
    for(i = 1; i < NINTWORDS; ++i)
      if(value[i]) {
        error = ERANGE;
        goto done;
      }
    n = value[0];
    if(n >= limit) {
      error = ERANGE;
      goto done;
    }
    while(scale > 0) {
      n *= base;
      if(n >= limit) {
        error = ERANGE;
        goto done;
      }
      --scale;
    }
    Fixed256_int2(r, (int)n);
  }
done:
  if(error == ERANGE) {
    // Set the maximum value of the given sign
    if(sign) {
      memset(r->u32, 0x00, sizeof r->u32);
      r->u32[7] = 0x80000000;
    } else {
      memset(r->u32, 0xFF, sizeof r->u32);
      r->u32[7] = 0x7FFFFFFF;
    }
  } else {
    /* Set the sign of the result */
    if(sign)
      Fixed256_neg(r, r);
  }
  if(endptr)
    *endptr = (char *)s;
  return error;
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
