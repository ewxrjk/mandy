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
#ifndef FIXED256_H
#define FIXED256_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NFRACBITS256 192

// The first 64 bits are the integer part; the remaining
// 192 bits, the fractional part. The least signficant word
// is first (so big-endian platforms won't work).
union Fixed256 {
  uint16_t u16[16];
  uint32_t u32[8];
  uint64_t u64[4];
  int64_t s64[4];
};

#define FIXED256_REP(F) F(0) F(1) F(2) F(3) F(4) F(5) F(6) F(7)
#define FIXED256_REP_COMMAS(F) F(0), F(1), F(2), F(3), F(4), F(5), F(6), F(7),

static inline void Fixed256_int2(union Fixed256 *r, int i) {
  r->u64[3] = (uint64_t)(int64_t)i;
  r->u64[2] = r->u64[1] = r->u64[0] = 0;
}

static inline void Fixed256_add(union Fixed256 *r, const union Fixed256 *a, const union Fixed256 *b) {
#if __amd64__
  uint64_t t;

  __asm__ volatile("mov (%[a]),%[t]\n\t"
                   "add (%[b]),%[t]\n\t"
                   "mov %[t],(%[r])\n\t"

                   "mov 8(%[a]),%[t]\n\t"
                   "adc 8(%[b]),%[t]\n\t"
                   "mov %[t],8(%[r])\n\t"

                   "mov 16(%[a]),%[t]\n\t"
                   "adc 16(%[b]),%[t]\n\t"
                   "mov %[t],16(%[r])\n\t"

                   "mov 24(%[a]),%[t]\n\t"
                   "adc 24(%[b]),%[t]\n\t"
                   "mov %[t],24(%[r])\n\t"
  : [t]"=&r"(t)
  : [a]"r"(a), [b]"r"(b), [r]"r"(r)
  : "cc", "memory");
#elif __aarch64__
  uint64_t av, bv;

  __asm__ volatile("ldr %[av],[%[a]]\n\t"
                   "ldr %[bv],[%[b]]\n\t"
                   "adds %[av],%[av],%[bv]\n\t"
                   "str %[av],[%[r]]\n\t"
                   
                   "ldr %[av],[%[a],8]\n\t"
                   "ldr %[bv],[%[b],8]\n\t"
                   "adcs %[av],%[av],%[bv]\n\t"
                   "str %[av],[%[r],8]\n\t"
                   
                   "ldr %[av],[%[a],16]\n\t"
                   "ldr %[bv],[%[b],16]\n\t"
                   "adcs %[av],%[av],%[bv]\n\t"
                   "str %[av],[%[r],16]\n\t"
                   
                   "ldr %[av],[%[a],24]\n\t"
                   "ldr %[bv],[%[b],24]\n\t"
                   "adc %[av],%[av],%[bv]\n\t"
                   "str %[av],[%[r],24]"
                   : [av]"=&r"(av), [bv]"=&r"(bv)
                   : [a]"r"(a), [b]"r"(b), [r]"r"(r)
                   : "cc", "memory");
#else
  uint64_t c = 0;

  for(int i = 0; i < 8; i++) {
    c = c + a->u32[i] + b->u32[i];
    r->u32[i] = (uint32_t)c; 
    c >>= 32;
  }
#endif
}

static inline void Fixed256_sub(union Fixed256 *r, const union Fixed256 *a, const union Fixed256 *b) {
#if __amd64__
  uint64_t t;

  __asm__ volatile("mov (%[a]),%[t]\n\t"
                   "sub (%[b]),%[t]\n\t"
                   "mov %[t],(%[r])\n\t"

                   "mov 8(%[a]),%[t]\n\t"
                   "sbb 8(%[b]),%[t]\n\t"
                   "mov %[t],8(%[r])\n\t"
                   
                   "mov 16(%[a]),%[t]\n\t"
                   "sbb 16(%[b]),%[t]\n\t"
                   "mov %[t],16(%[r])\n\t"
                   
                   "mov 24(%[a]),%[t]\n\t"
                   "sbb 24(%[b]),%[t]\n\t"
                   "mov %[t],24(%[r])\n\t"
  : [t]"=&r"(t)
  : [a]"r"(a), [b]"r"(b), [r]"r"(r)
  : "cc", "memory");
#elif __aarch64__
  uint64_t av, bv;

  __asm__ volatile("ldr %[av],[%[a]]\n\t"
                   "ldr %[bv],[%[b]]\n\t"
                   "subs %[av],%[av],%[bv]\n\t"
                   "str %[av],[%[r]]\n\t"
                   
                   "ldr %[av],[%[a],8]\n\t"
                   "ldr %[bv],[%[b],8]\n\t"
                   "sbcs %[av],%[av],%[bv]\n\t"
                   "str %[av],[%[r],8]\n\t"
                   
                   "ldr %[av],[%[a],16]\n\t"
                   "ldr %[bv],[%[b],16]\n\t"
                   "sbcs %[av],%[av],%[bv]\n\t"
                   "str %[av],[%[r],16]\n\t"
                   
                   "ldr %[av],[%[a],24]\n\t"
                   "ldr %[bv],[%[b],24]\n\t"
                   "sbc %[av],%[av],%[bv]\n\t"
                   "str %[av],[%[r],24]"
                   : [av]"=&r"(av), [bv]"=&r"(bv)
                   : [a]"r"(a), [b]"r"(b), [r]"r"(r)
                   : "cc", "memory");
#else
  uint64_t c = 1;

  for(int i = 0; i < 8; i++) {
    c = c + a->u32[i] + ~b->u32[i];
    r->u32[i] = (uint32_t)c; 
    c >>= 32;
  }
  #endif
}

static inline void Fixed256_neg(union Fixed256 *r, const union Fixed256 *a) {
#if __amd64__
  uint64_t t;

  __asm__ volatile("mov (%[a]),%[t]\n\t"
                   "not %[t]\n\t"
                   "add $1,%[t]\n\t"
                   "mov %[t],(%[r])\n\t"

                   "mov 8(%[a]),%[t]\n\t"
                   "not %[t]\n\t"
                   "adc $0,%[t]\n\t"
                   "mov %[t],8(%[r])\n\t"
                   
                   "mov 16(%[a]),%[t]\n\t"
                   "not %[t]\n\t"
                   "adc $0,%[t]\n\t"
                   "mov %[t],16(%[r])\n\t"
                   
                   "mov 24(%[a]),%[t]\n\t"
                   "not %[t]\n\t"
                   "adc $0,%[t]\n\t"
                   "mov %[t],24(%[r])\n\t"
  : [t]"=&r"(t)
  : [a]"r"(a), [r]"r"(r)
  : "cc", "memory");
#elif __aarch64__
  uint64_t av;

  __asm__ volatile("ldr %[av],[%[a]]\n\t"
                   "negs %[av],%[av]\n\t"
                   "str %[av],[%[r]]\n\t"
                   
                   "ldr %[av],[%[a],8]\n\t"
                   "ngcs %[av],%[av]\n\t"
                   "str %[av],[%[r],8]\n\t"
                   
                   "ldr %[av],[%[a],16]\n\t"
                   "ngcs %[av],%[av]\n\t"
                   "str %[av],[%[r],16]\n\t"
                   
                   "ldr %[av],[%[a],24]\n\t"
                   "ngc %[av],%[av]\n\t"
                   "str %[av],[%[r],24]"
                   : [av]"=&r"(av)
                   : [a]"r"(a), [r]"r"(r)
                   : "cc", "memory");
#else
  const union Fixed256 zero = { .u64 = { 0 }};
  Fixed256_sub(r, &zero, a);
#endif
}

void Fixed256_mul(union Fixed256 *r, const union Fixed256 *a, const union Fixed256 *b);
void Fixed256_square(union Fixed256 *r, const union Fixed256 *a);

static inline void Fixed256_shl_unsigned(union Fixed256 *a) {
  a->u64[3] = (a->u64[3] << 1) | (a->u64[2] >> 63);
  a->u64[2] = (a->u64[2] << 1) | (a->u64[1] >> 63);
  a->u64[1] = (a->u64[1] << 1) | (a->u64[0] >> 63);
  a->u64[0] = a->u64[0] << 1;
}

static inline void Fixed256_shr_unsigned(union Fixed256 *a) {
  a->u64[0] = (a->u64[0] >> 1) | (a->u64[1] << 63);
  a->u64[1] = (a->u64[1] >> 1) | (a->u64[2] << 63);
  a->u64[2] = (a->u64[2] >> 1) | (a->u64[3] << 63);
  a->u64[3] = a->u64[3] >> 1;
}

static inline void Fixed256_setbit(union Fixed256 *a, int bit) {
  bit += NFRACBITS256;
  a->u64[bit / 64] |= ((uint64_t)1 << (bit & 63));
}

static inline bool Fixed256_lt0(const union Fixed256 *a) {
  return a->s64[3] < 0;
}

static inline bool Fixed256_ge0(const union Fixed256 *a) {
  return a->s64[3] >= 0;
}

static inline bool Fixed256_eq(const union Fixed256 *a, const union Fixed256 *b) {
  return a->u64[0] == b->u64[0] && a->u64[1] == b->u64[1] && a->u64[2] == b->u64[2] && a->u64[3] == b->u64[3];
}

static inline bool Fixed256_ne(const union Fixed256 *a, const union Fixed256 *b) {
  return !Fixed256_eq(a, b);
}

static inline bool Fixed256_lt(const union Fixed256 *a, const union Fixed256 *b) {
  if(a->s64[3] < b->s64[3]) return true;
  if(a->s64[3] > b->s64[3]) return false;

  if(a->u64[2] < b->u64[2]) return true;
  if(a->u64[2] > b->u64[2]) return false;

  if(a->u64[1] < b->u64[1]) return true;
  if(a->u64[1] > b->u64[1]) return false;

  if(a->u64[0] < b->u64[0]) return true;
  if(a->u64[0] > b->u64[0]) return false;

  return false;
}

static inline bool Fixed256_gt(const union Fixed256 *a, const union Fixed256 *b) {
  return Fixed256_lt(b, a);
}

static inline bool Fixed256_le(const union Fixed256 *a, const union Fixed256 *b) {
  return !Fixed256_lt(b, a);
}

static inline bool Fixed256_ge(const union Fixed256 *a, const union Fixed256 *b) {
  return !Fixed256_lt(a, b);
}

static inline bool Fixed256_lt_unsigned(const union Fixed256 *a, const union Fixed256 *b) {
  if(a->u64[3] < b->u64[3]) return true;
  if(a->u64[3] > b->u64[3]) return false;

  if(a->u64[2] < b->u64[2]) return true;
  if(a->u64[2] > b->u64[2]) return false;

  if(a->u64[1] < b->u64[1]) return true;
  if(a->u64[1] > b->u64[1]) return false;

  if(a->u64[0] < b->u64[0]) return true;
  if(a->u64[0] > b->u64[0]) return false;

  return false;
}

static inline bool Fixed256_gt_unsigned(const union Fixed256 *a, const union Fixed256 *b) {
  return Fixed256_lt_unsigned(b, a);
}

static inline bool Fixed256_le_unsigned(const union Fixed256 *a, const union Fixed256 *b) {
  return !Fixed256_lt_unsigned(b, a);
}

static inline bool Fixed256_ge_unsigned(const union Fixed256 *a, const union Fixed256 *b) {
  return !Fixed256_lt_unsigned(a,b);
}

static inline bool Fixed256_eq0(const union Fixed256 *a) {
  return a->u64[0] == 0 && a->u64[1] == 0 && a->u64[2] == 0 && a->u64[3] == 0;
}

char *Fixed256_2str(char buffer[], unsigned bufsize, const union Fixed256 *a, int base);

int Fixed256_str2(union Fixed256 *r, const char *s, char **endptr);

void Fixed256_div(union Fixed256 *r, const union Fixed256 *a, const union Fixed256 *b);

void Fixed256_sqrt(union Fixed256 *r, const union Fixed256 *a);

void Fixed256_double2(union Fixed256 *r, double n);
double Fixed256_2double(const union Fixed256 *a);
long double Fixed256_2longdouble(const union Fixed256 *a);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <string>

class fixed256 {
public:
  Fixed256 f;

  // Constructors

  fixed256(int i) {
    Fixed256_int2(&f, i);
  }

  fixed256(double n) {
    Fixed256_double2(&f, n);
  }

  fixed256() {
    Fixed256_int2(&f, 0);
  }

  fixed256(const Fixed256 &raw): f(raw) {}

  // Assignment

#if 0
  fixed256 &operator/=(unsigned that) {
    Fixed256_divu(&f, &f, that);
    return *this;
  }
#endif

  fixed256 &operator+=(const fixed256 &that) {
    Fixed256_add(&f, &f, &that.f);
    return *this;
  }

  fixed256 &operator-=(const fixed256 &that) {
    Fixed256_sub(&f, &f, &that.f);
    return *this;
  }

  fixed256 &operator*=(const fixed256 &that) {
    Fixed256_mul(&f, &f, &that.f);
    return *this;
  }

  fixed256 &operator/=(const fixed256 &that) {
    Fixed256_div(&f, &f, &that.f);
    return *this;
  }

  fixed256 &operator=(int n) {
    Fixed256_int2(&f, n);
    return *this;
  }

  // Arithmetic

  fixed256 operator-() const {
    fixed256 r;
    Fixed256_neg(&r.f, &f);
    return r;
  }

  fixed256 operator+(const fixed256 &that) const {
    fixed256 r;
    Fixed256_add(&r.f, &f, &that.f);
    return r;
  }

  fixed256 operator-(const fixed256 &that) const {
    fixed256 r;
    Fixed256_sub(&r.f, &f, &that.f);
    return r;
  }

  fixed256 operator*(const fixed256 &that) const {
    fixed256 r;
    Fixed256_mul(&r.f, &f, &that.f);
    return r;
  }

  fixed256 operator/(const fixed256 &that) const {
    fixed256 r;
    Fixed256_div(&r.f, &f, &that.f);
    return r;
  }

#if 0
  fixed256 operator/(unsigned that) const {
    fixed256 r;
    Fixed256_divu(&r.f, &f, that);
    return r;
  }
#endif

  // Comparison

  bool operator<(const fixed256 &that) const {
    return Fixed256_lt(&f, &that.f);
  }

  bool operator>(const fixed256 &that) const {
    return Fixed256_gt(&f, &that.f);
  }

  bool operator>=(const fixed256 &that) const {
    return Fixed256_ge(&f, &that.f);
  }

  bool operator<=(const fixed256 &that) const {
    return Fixed256_le(&f, &that.f);
  }

  bool operator==(const fixed256 &that) const {
    return Fixed256_eq(&f, &that.f);
  }

  bool operator!=(const fixed256 &that) const {
    return Fixed256_ne(&f, &that.f);
  }

  // Conversions
  std::string toString(int base = 10) const;
  std::string toHex() const;
  int fromString(const char *s, char **endptr) {
    return Fixed256_str2(&f, s, endptr);
  }

  explicit operator int() const {
    return f.u64[3];
  }

  explicit operator double() const {
    return Fixed256_2double(&f);
  }

  explicit operator long double() const {
    return Fixed256_2longdouble(&f);
  }
};

inline fixed256 sqrt(const fixed256 &f) {
  fixed256 r;
  Fixed256_sqrt(&r.f, &f.f);
  return r;
}

#endif

#endif