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
#include "mandy.h"
#include <stdio.h>
#include "Fixed64.h"
#include "Fixed128.h"
#include <errno.h>

uint64_t Fixed64_mul_unsigned(uint64_t a, uint64_t b);
static uint64_t Fixed64_div_unsigned(uint64_t a, uint64_t b);

#if ! HAVE_ASM_64
Fixed64 Fixed64_mul(Fixed64 a, Fixed64 b) {
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
  r = Fixed64_mul_unsigned(a, b);
  return sign ? -r : r;
}

#define MLA(x,y,n) do {                                 \
  uint64_t r = (uint64_t)(uint32_t)(x) * (uint32_t)(y); \
  int i = (n);                                          \
  while(i <= 3) {                                       \
    r += result[i];                                     \
    result[i] = (uint32_t)r;                            \
    r >>= 32;                                           \
    ++i;                                                \
  }                                                     \
} while(0)

uint64_t Fixed64_mul_unsigned(uint64_t a, uint64_t b) {
  uint32_t result[4];                   /* accumulator for result */
  memset(result, 0, sizeof result);
  MLA(a, b, 0);
  MLA(a >> 32, b, 1);
  MLA(a, b >> 32, 1);
  MLA(a >> 32, b >> 32, 2);
  /*
   * iIFF FFFF Ffff ffff
   *
   * -> IFF.FFFF.F
   */
  return (((uint64_t)result[3] << 40)
          + ((uint64_t)result[2] << 8)
          + ((uint64_t)result[1] >> 24)
          + /*rounding*/!!(result[1] & 0x00800000));
}

#undef MLA
#endif

Fixed64 Fixed64_div(Fixed64 a, Fixed64 b) {
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

static uint64_t Fixed64_div_unsigned(uint64_t a, uint64_t b) {
  uint64_t q = 0, bit = (uint64_t)1<<56;
  while(b < a) {
    bit <<= 1;
    b <<= 1;
  }
  while(a && bit) {
    if(a >= b) {
      q |= bit;
      a -= b;
    }
    bit >>= 1;
    b >>= 1;
  }
  // TODO rounding
  return q;
}

Fixed64 Fixed64_sqrt(Fixed64 a) {
  uint64_t r = 0, bit = (uint64_t)8 << 56;
  while(a && bit) {
    uint64_t rb = r + bit;
    uint64_t p = Fixed64_mul_unsigned(rb, rb);
    if(p <= (uint64_t)a)
      r = rb;
    bit >>= 1;
  }
  // TODO rounding
  return r;
}

int Fixed128_to_Fixed64(Fixed64 *r, const struct Fixed128 *a) {
  int32_t intpart = a->word[NFIXED128 - 1];
  uint64_t result;
  if(intpart > 127 || intpart < -128)
    return ERANGE;
  result = (uint64_t)a->word[NFIXED128 - 1] << 56;
  result += (uint64_t)a->word[NFIXED128 - 2] << 24;
  result += a->word[NFIXED128 - 3] >> 8;
  if(a->word[NFIXED128-3] & 128) {
    // Round up, checking for overflow
    if(result == 0x7fffffffffffffffLL)
      return ERANGE;
    ++result;
  }
  *r = result;
  return 0;
}

void Fixed64_to_Fixed128(struct Fixed128 *r, Fixed64 a) {
  memset(r, 0, sizeof *r);
  r->word[NFIXED128 - 1] = a >> 56;
  r->word[NFIXED128 - 2] = (uint32_t)(a >> 24);
  r->word[NFIXED128 - 3] = (uint32_t)((uint64_t)a << 8);
}

/* For string conversions we re-use the full-width code - the result is not as
 * fast, but it is easier. */

char *Fixed64_2str(char buffer[], unsigned bufsize, Fixed64 a, int base) {
  struct Fixed128 f;
  Fixed64_to_Fixed128(&f, a);
  return Fixed128_2str(buffer, bufsize, &f, base);
}

int Fixed64_str2(Fixed64 *r, const char *s, char **endptr) {
  struct Fixed128 f;
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
