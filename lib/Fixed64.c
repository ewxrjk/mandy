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
#include <stdio.h>
#include "Fixed64.h"
#include "Fixed128.h"
#include <errno.h>
#include <assert.h>

Fixed64 Fixed64_mul_generic(Fixed64 a, Fixed64 b) {
  int sign = 0;
  if(a < 0) {
    a = -a;
    sign = !sign;
  }
  if(b < 0) {
    b = -b;
    sign = !sign;
  }
  uint128_t r128 = (uint128_t)a * (uint128_t)b;
  uint64_t c = (uint64_t)(r128 >> 55) & 1; // the bit we're going to carry out the bottom
  Fixed64 r = (uint64_t)(r128 >> 56) + c;  // move the point back to the right place and round up
  return sign ? -r : r;
}

Fixed64 Fixed64_div(Fixed64 a, Fixed64 b) {
  assert(b != 0);
  Fixed64 r;
  int sign = 0;
  if(a < 0) {
    a = -a;
    sign = !sign;
  }
  if(b < 0) {
    b = -b;
    sign = !sign;
  }
  r = Fixed64_div_unsigned(a, b);
  return sign ? -r : r;
}

#if !__amd64__
uint64_t Fixed64_div_unsigned(uint64_t a, uint64_t b) {
  uint128_t a128 = (uint128_t)a << 64, b128 = (uint128_t)b << 64, q128 = 0, bit = (uint128_t)1 << (56 + 64);
  // Shift b up until b>=a. We adjust initial quotient bit
  // up to match.
  while(b128 < a128) {
    bit <<= 1;
    b128 <<= 1;
  }
  // Work out bits from the quotient down far enough to get rounding the right
  while(a128 && bit >= (uint128_t)1 << 63) {
    // See if this bit belongs in the result
    if(a128 >= b128) {
      q128 |= bit;
      a128 -= b128;
    }
    // Move on to the next bit down
    bit >>= 1;
    b128 >>= 1;
  }
  // Round up
  uint64_t q = (q128 >> 64) + ((q128 >> 63) & 1);
  return q;
}
#endif

Fixed64 Fixed64_sqrt(Fixed64 a) {
  assert(a >= 0);

  // The general idea is similar to long division.
  // Starting from the most significant end we see if
  // each bit belongs in the result by doing a trial
  // addition to a lower bound and seeing if the
  // square of that value breaches the target (a).
  //
  // We do it in an unsigned 128-bit accumulator with 8 integer bits
  // and 120 fractional bits, to avoid underflow issues.
  //
  // Finally we observe that
  //
  //   r^2 + r.2^(n+1) + 2^(2n)
  //
  // ...meaning we can avoid a full multiply and operate
  // with shifts and adds alone.

  uint128_t a128 = ((uint128_t)a) << 64;
  uint128_t r = 0, r2 = 0;
  for(int n = 3; n >= -57; n--) {
    // rshifted = r<<(n+1) = 2.2^(n+1), without shifting by a negative
    // quantity (which is undefined behavior).
    int rshift = n + 1;
    uint128_t rshifted = rshift >= 0 ? r << rshift : r >> -rshift;
    // r2next = r^2 + r.2^(n+1) + 2^(2n)
    uint128_t r2next = r2 + rshifted + ((uint128_t)1 << (120 + 2 * n));
#if 0
    fprintf(stderr,
            "n=%3d a=%016lx%016lx r=%016lx%016lx r2=%016lx%016lx r2next=%016lx%016lx %s\n",
            n,
            (uint64_t)(a128 >> 64),
            (uint64_t)a128,
            (uint64_t)(r >> 64),
            (uint64_t)r,
            (uint64_t)(r2 >> 64),
            (uint64_t)r2,
            (uint64_t)(r2next >> 64),
            (uint64_t)r2next,
            r2next <= a128 ? "✓" : "");
#endif
    if(r2next <= a128) {
      r += (uint128_t)1 << (120 + n);
      r2 = r2next;
    }
  }
  uint64_t c = (uint64_t)(r >> 63) & 1; // the bit we're going to carry out the bottom
  return (uint64_t)(r >> 64) + c;       // move the point back to the right place and round up
}

int Fixed128_to_Fixed64(Fixed64 *r, const union Fixed128 *a) {
  int32_t intpart = (int32_t)(a->word[NFIXED128 - 1]);
  uint64_t result;
  if(intpart > 127 || intpart < -128)
    return ERANGE;
  result = (uint64_t)a->word[NFIXED128 - 1] << 56;
  result += (uint64_t)a->word[NFIXED128 - 2] << 24;
  result += a->word[NFIXED128 - 3] >> 8;
  if(a->word[NFIXED128 - 3] & 128) {
    // Round up, checking for overflow
    if(result == 0x7fffffffffffffffLL)
      return ERANGE;
    ++result;
  }
  *r = (Fixed64)result;
  return 0;
}

void Fixed64_to_Fixed128(union Fixed128 *r, Fixed64 a) {
  memset(r, 0, sizeof *r);
  r->word[NFIXED128 - 1] = (uint32_t)(a >> 56);
  r->word[NFIXED128 - 2] = (uint32_t)(a >> 24);
  r->word[NFIXED128 - 3] = (uint32_t)((uint64_t)a << 8);
}

/* For string conversions we re-use the full-width code - the result is not as
 * fast, but it is easier. */

char *Fixed64_2str(char buffer[], unsigned bufsize, Fixed64 a, int base) {
  union Fixed128 f;
  Fixed64_to_Fixed128(&f, a);
  return Fixed128_2str(buffer, bufsize, &f, base);
}

int Fixed64_str2(Fixed64 *r, const char *s, char **endptr) {
  union Fixed128 f;
  int rc = Fixed128_str2(&f, s, endptr);
  if(rc)
    return rc;
  return Fixed128_to_Fixed64(r, &f);
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
