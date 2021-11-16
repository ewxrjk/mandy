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
#ifndef FIXED128_H
#define FIXED128_H

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NFIXED128 4 /* == 128 bits */
#define NFRACBITS 32 * (NFIXED128 - 1)

typedef unsigned __int128 uint128_t;
typedef __int128 int128_t;

union Fixed128 {
  // In a word-based interpreation:
  //
  // Least significant word is first
  // Point last before last (most significant) word
  // So you get 1 sign bit, 31 integer bits, and 32 * (NFIXED128-1) fractional
  // bits.
  uint32_t word[NFIXED128];
  // Interpreted as two 64-bit integers, little-endian order
  // i.e. qword[0] * 2^-96 * qword[1] ^ 2^-32
  uint64_t u64[2];
  // Interpreted as a 128-bit integer, u128 * 2^-96 or s128 * 2^-96
  uint128_t u128;
  int128_t s128;
  // The combination of these representations mean we only work on
  // little-endian platforms.
};

static inline void Fixed128_add(union Fixed128 *r, const union Fixed128 *a,
                                const union Fixed128 *b) {
  r->s128 = a->s128 + b->s128;
}

static inline void Fixed128_sub(union Fixed128 *r, const union Fixed128 *a,
                                const union Fixed128 *b) {
  r->s128 = a->s128 - b->s128;
}

static inline int Fixed128_neg(union Fixed128 *r, const union Fixed128 *a) {
  uint32_t sign = a->word[NFIXED128 - 1] & 0x80000000;
  r->s128 = -a->s128;
  if(sign && (r->word[NFIXED128 - 1] & 0x80000000))
    return 1;
  else
    return 0;
}

int Fixed128_mul(union Fixed128 *r, const union Fixed128 *a,
                 const union Fixed128 *b);

void Fixed128_divu(union Fixed128 *r, const union Fixed128 *a, unsigned u);

void Fixed128_div(union Fixed128 *r, const union Fixed128 *a,
                  const union Fixed128 *b);

void Fixed128_sqrt(union Fixed128 *r, const union Fixed128 *a);

static inline void Fixed128_int2(union Fixed128 *r, int i) {
  r->s128 = (int128_t)i << 96;
}

static inline void Fixed128_shl_unsigned(union Fixed128 *a) {
  a->u128 <<= 1;
}

static inline void Fixed128_shr_unsigned(union Fixed128 *a) {
  a->u128 >>= 1;
}

static inline void Fixed128_setbit(union Fixed128 *a, int bit) {
  a->u128 |= (uint128_t)1 << (bit + 96);
}

static inline int Fixed128_lt0(const union Fixed128 *a) {
  return a->s128 < 0;
}

static inline int Fixed128_ge0(const union Fixed128 *a) {
  return a->s128 >= 0;
}

static inline int Fixed128_eq(const union Fixed128 *a,
                              const union Fixed128 *b) {
  return a->u128 == b->u128;
}

static inline int Fixed128_ne(const union Fixed128 *a,
                              const union Fixed128 *b) {
  return a->u128 != b->u128;
}

static inline int Fixed128_lt(const union Fixed128 *a,
                              const union Fixed128 *b) {
  return a->s128 < b->s128;
}

static inline int Fixed128_lt_unsigned(const union Fixed128 *a,
                                       const union Fixed128 *b) {
  return a->u128 < b->u128;
}

static inline int Fixed128_gt(const union Fixed128 *a,
                              const union Fixed128 *b) {
  return a->s128 > b->s128;
}

static inline int Fixed128_le(const union Fixed128 *a,
                              const union Fixed128 *b) {
  return a->s128 <= b->s128;
}

static inline int Fixed128_ge(const union Fixed128 *a,
                              const union Fixed128 *b) {
  return a->s128 >= b->s128;
}

static inline int Fixed128_gt_unsigned(const union Fixed128 *a,
                                       const union Fixed128 *b) {
  return a->u128 > b->u128;
}

static inline int Fixed128_le_unsigned(const union Fixed128 *a,
                                       const union Fixed128 *b) {
  return a->u128 <= b->u128;
}

static inline int Fixed128_eq0(const union Fixed128 *a) {
  return a->u128 == 0;
}

char *Fixed128_2str(char buffer[], unsigned bufsize, const union Fixed128 *a,
                    int base);
int Fixed128_str2(union Fixed128 *r, const char *s, char **endptr);

int Fixed128_str2_cs(union Fixed128 *r, const char *s);
#define FIXED128_STR_OK 0
#define FIXED128_STR_RANGE 1
#define FIXED128_STR_FORMAT 2

void Fixed128_double2(union Fixed128 *r, double n);
double Fixed128_2double(const union Fixed128 *a);
long double Fixed128_2longdouble(const union Fixed128 *a);

int Fixed128_iterate(union Fixed128 *zx, union Fixed128 *zy,
                     const union Fixed128 *cx, const union Fixed128 *cy,
                     int64_t maxiters);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <string>

class fixed128 {
public:
  Fixed128 f;

  // Constructors

  fixed128(int i) {
    Fixed128_int2(&f, i);
  }

  fixed128(double n) {
    Fixed128_double2(&f, n);
  }

  fixed128() {
    Fixed128_int2(&f, 0);
  }

  fixed128(const Fixed128 &raw): f(raw) {}

  // Assignment

  fixed128 &operator/=(unsigned that) {
    Fixed128_divu(&f, &f, that);
    return *this;
  }

  fixed128 &operator+=(const fixed128 &that) {
    Fixed128_add(&f, &f, &that.f);
    return *this;
  }

  fixed128 &operator-=(const fixed128 &that) {
    Fixed128_sub(&f, &f, &that.f);
    return *this;
  }

  fixed128 &operator*=(const fixed128 &that) {
    Fixed128_mul(&f, &f, &that.f);
    return *this;
  }

  fixed128 &operator/=(const fixed128 &that) {
    Fixed128_div(&f, &f, &that.f);
    return *this;
  }

  fixed128 &operator=(int n) {
    Fixed128_int2(&f, n);
    return *this;
  }

  // Arithmetic

  fixed128 operator-() const {
    fixed128 r;
    Fixed128_neg(&r.f, &f);
    return r;
  }

  fixed128 operator+(const fixed128 &that) const {
    fixed128 r;
    Fixed128_add(&r.f, &f, &that.f);
    return r;
  }

  fixed128 operator-(const fixed128 &that) const {
    fixed128 r;
    Fixed128_sub(&r.f, &f, &that.f);
    return r;
  }

  fixed128 operator*(const fixed128 &that) const {
    fixed128 r;
    Fixed128_mul(&r.f, &f, &that.f);
    return r;
  }

  fixed128 operator/(const fixed128 &that) const {
    fixed128 r;
    Fixed128_div(&r.f, &f, &that.f);
    return r;
  }

  fixed128 operator/(unsigned that) const {
    fixed128 r;
    Fixed128_divu(&r.f, &f, that);
    return r;
  }

  // Comparison

  bool operator<(const fixed128 &that) const {
    return !!Fixed128_lt(&f, &that.f);
  }

  bool operator>(const fixed128 &that) const {
    return !!Fixed128_lt(&that.f, &f);
  }

  bool operator>=(const fixed128 &that) const {
    return !Fixed128_lt(&f, &that.f);
  }

  bool operator<=(const fixed128 &that) const {
    return !Fixed128_lt(&that.f, &f);
  }

  bool operator==(const fixed128 &that) const {
    return !!Fixed128_eq(&that.f, &f);
  }

  bool operator!=(const fixed128 &that) const {
    return !Fixed128_eq(&that.f, &f);
  }

  // Conversions
  std::string toString(int base = 10) const;
  std::string toHex() const;
  int fromString(const char *s, char **endptr) {
    return Fixed128_str2(&f, s, endptr);
  }

  int toInt() const {
    return f.word[NFIXED128 - 1];
  }

  double toDouble() const {
    return Fixed128_2double(&f);
  }

  long double toLongDouble() const {
    return Fixed128_2longdouble(&f);
  }
};

inline fixed128 sqrt(const fixed128 &f) {
  fixed128 r;
  Fixed128_sqrt(&r.f, &f.f);
  return r;
}

#endif

#endif /* FIXED128_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
