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

#define NFIXED128 4                        /* == 128 bits */
#define NFRACBITS 32 * (NFIXED128 - 1)

  struct Fixed128 {
    // Least significant word is first
    // Point last before last (most significant) word
    // So you get 1 sign bit, 31 integer bits, and 32 * (NFIXED128-1) fractional
    // bits.
    uint32_t word[NFIXED128];
  };

  LIBMANDY_API void Fixed128_add(struct Fixed128 *r, const struct Fixed128 *a, const struct Fixed128 *b);

  LIBMANDY_API void Fixed128_sub(struct Fixed128 *r, const struct Fixed128 *a, const struct Fixed128 *b);

  LIBMANDY_API int Fixed128_neg(struct Fixed128 *r, const struct Fixed128 *a);

  LIBMANDY_API int Fixed128_mul(struct Fixed128 *r, const struct Fixed128 *a, const struct Fixed128 *b);

  LIBMANDY_API void Fixed128_divu(struct Fixed128 *r, const struct Fixed128 *a, unsigned u);

  LIBMANDY_API void Fixed128_div(struct Fixed128 *r, const struct Fixed128 *a, const struct Fixed128 *b);

  LIBMANDY_API void Fixed128_sqrt(struct Fixed128 *r, const struct Fixed128 *a);

  LIBMANDY_API void Fixed128_int2(struct Fixed128 *r, int i);

  LIBMANDY_API void Fixed128_shl_unsigned(struct Fixed128 *a);

  LIBMANDY_API void Fixed128_shr_unsigned(struct Fixed128 *a);

  static inline int Fixed128_lt0(const struct Fixed128 *a) {
    return a->word[NFIXED128 - 1] & 0x80000000;
  }

  static inline int Fixed128_ge0(const struct Fixed128 *a) {
    return !(a->word[NFIXED128 - 1] & 0x80000000);
  }

  int Fixed128_eq(const struct Fixed128 *a, const struct Fixed128 *b);

  static inline int Fixed128_ne(const struct Fixed128 *a, const struct Fixed128 *b) {
    return !Fixed128_eq(a, b);
  }

  LIBMANDY_API int Fixed128_lt(const struct Fixed128 *a, const struct Fixed128 *b);

  static inline int Fixed128_gt(const struct Fixed128 *a, const struct Fixed128 *b) {
    return Fixed128_lt(b, a);
  }

  static inline int Fixed128_le(const struct Fixed128 *a, const struct Fixed128 *b) {
    return !Fixed128_gt(a, b);
  }

  static inline int Fixed128_ge(const struct Fixed128 *a, const struct Fixed128 *b) {
    return !Fixed128_lt(a, b);
  }

  LIBMANDY_API int Fixed128_eq0(const struct Fixed128 *a);

  LIBMANDY_API char *Fixed128_2str(char buffer[], unsigned bufsize, const struct Fixed128 *a,
                                   int base);
  LIBMANDY_API int Fixed128_str2(struct Fixed128 *r, const char *s, char **endptr);

  LIBMANDY_API int Fixed128_str2_cs(struct Fixed128 *r, const char *s);
#define FIXED128_STR_OK 0
#define FIXED128_STR_RANGE 1
#define FIXED128_STR_FORMAT 2

  LIBMANDY_API void Fixed128_double2(struct Fixed128 *r, double n);
  LIBMANDY_API double Fixed128_2double(const struct Fixed128 *a);
  LIBMANDY_API long double Fixed128_2longdouble(const struct Fixed128 *a);

  LIBMANDY_API int Fixed128_iterate(struct Fixed128 *zx, struct Fixed128 *zy,
                                    const struct Fixed128 *cx, const struct Fixed128 *cy,
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

  fixed128(const Fixed128 &raw): f(raw) {
  }

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

  fixed128 operator-() const{
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
