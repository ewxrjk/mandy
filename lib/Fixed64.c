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
#include <stdio.h>
#include "Fixed64.h"
#include "Fixed.h"
#include <errno.h>

static uint64_t Fixed64_mul_unsigned(uint64_t a, uint64_t b);
static uint64_t Fixed64_div_unsigned(uint64_t a, uint64_t b);

Fixed64 Fixed64_mul(Fixed64 a, Fixed64 b) {
  int sign = 0;
  if(a < 0) {
    a = -a;
    sign = !sign;
  }
  if(b < 0) {
    b = -b;
    sign = !sign;
  }
  Fixed64 r = Fixed64_mul_unsigned(a, b);
  return sign ? -r : r;
}

#define MLA(x,y,n) do {                                 \
  uint64_t r = (uint64_t)(uint32_t)(x) * (uint32_t)(y); \
  int i = (n);                                          \
  while(i <= 3) {                                       \
    r += result[i];                                     \
    result[i] = r;                                      \
    r >>= 32;                                           \
    ++i;                                                \
  }                                                     \
} while(0)

static uint64_t Fixed64_mul_unsigned(uint64_t a, uint64_t b) {
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
          + /*rounding*/(result[1] & 0x00800000) ? 1 : 0);
}

#undef MLA

Fixed64 Fixed64_div(Fixed64 a, Fixed64 b) {
  int sign = 0;
  if(a < 0) {
    a = -a;
    sign = !sign;
  }
  if(b < 0) {
    b = -b;
    sign = !sign;
  }
  Fixed64 r = Fixed64_div_unsigned(a, b);
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

//Fixed64 Fixed64_sqrt(Fixed64 a) {
//  TODO
//}

/* For string conversions we re-use the full-width code - the result is not as
 * fast, but it is easier. */

static int Fixed_to_Fixed64(Fixed64 *r, const struct Fixed *a) {
  int32_t intpart = a->word[NFIXED - 1];
  if(intpart > 127 || intpart < -128)
    return ERANGE;
  uint64_t result = (uint64_t)a->word[NFIXED - 1] << 56;
  result += (uint64_t)a->word[NFIXED - 2] << 24;
  result += a->word[NFIXED - 3] >> 8;
  if(a->word[NFIXED-3] & 128) {
    // Round up, checking for overflow
    if(result == 0x7fffffffffffffffLL)
      return ERANGE;
    ++result;
  }
  *r = result;
  return 0;
}

static void Fixed64_to_Fixed(struct Fixed *r, Fixed64 a) {
  memset(r, 0, sizeof *r);
  r->word[NFIXED - 1] = a >> 56;
  r->word[NFIXED - 2] = a >> 24;
  r->word[NFIXED - 3] = a << 8;
}

char *Fixed64_2str(char buffer[], unsigned bufsize, Fixed64 a, int base) {
  struct Fixed f;
  Fixed64_to_Fixed(&f, a);
  return Fixed_2str(buffer, bufsize, &f, base);
}

int Fixed64_str2(Fixed64 *r, const char *s, char **endptr) {
  struct Fixed f;
  int rc = Fixed_str2(&f, s, endptr);
  if(rc)
    return rc;
  return Fixed_to_Fixed64(r, &f);
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
