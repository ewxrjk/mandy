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
#ifndef FIXED_H
#define FIXED_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NFIXED 4                        /* == 128 bits */

  struct Fixed {
    // Least significant word is first
    // Point last before last (most significant) word
    // So you get 1 sign bit, 31 integer bits, and 32 * (NFIXED-1) fractional
    // bits.
    uint32_t word[NFIXED];
  };

  void Fixed_add(struct Fixed *r, const struct Fixed *a, const struct Fixed *b);

  void Fixed_sub(struct Fixed *r, const struct Fixed *a, const struct Fixed *b);

  int Fixed_neg(struct Fixed *r, const struct Fixed *a);

  int Fixed_mul(struct Fixed *r, const struct Fixed *a, const struct Fixed *b);

  void Fixed_divu(struct Fixed *r, const struct Fixed *a, unsigned u);

  void Fixed_div(struct Fixed *r, const struct Fixed *a, const struct Fixed *b);

  void Fixed_sqrt(struct Fixed *r, const struct Fixed *a);

  void Fixed_int2(struct Fixed *r, int i);

  static inline int Fixed_lt0(const struct Fixed *a) {
    return a->word[NFIXED - 1] & 0x80000000;
  }

  static inline int Fixed_ge0(const struct Fixed *a) {
    return !(a->word[NFIXED - 1] & 0x80000000);
  }

  int Fixed_eq(const struct Fixed *a, const struct Fixed *b);

  static inline int Fixed_ne(const struct Fixed *a, const struct Fixed *b) {
    return !Fixed_eq(a, b);
  }

  int Fixed_lt(const struct Fixed *a, const struct Fixed *b);

  static inline int Fixed_gt(const struct Fixed *a, const struct Fixed *b) {
    return Fixed_lt(b, a);
  }

  static inline int Fixed_le(const struct Fixed *a, const struct Fixed *b) {
    return !Fixed_gt(a, b);
  }

  static inline int Fixed_ge(const struct Fixed *a, const struct Fixed *b) {
    return !Fixed_lt(a, b);
  }

  int Fixed_eq0(const struct Fixed *a);

  void Fixed_2str(char buffer[], unsigned bufsize, const struct Fixed *a,
                  int base);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <string>

class fixed {
public:
  Fixed f;

  // Constructors

  fixed(int i) {
    Fixed_int2(&f, i);
  }

  fixed() {
    Fixed_int2(&f, 0);
  }

  fixed(const Fixed &raw): f(raw) {
  }

  // Assignment

  fixed &operator/=(unsigned that) {
    Fixed_divu(&f, &f, that);
    return *this;
  }

  fixed &operator+=(const fixed &that) {
    Fixed_add(&f, &f, &that.f);
    return *this;
  }

  fixed &operator-=(const fixed &that) {
    Fixed_sub(&f, &f, &that.f);
    return *this;
  }

  fixed &operator*=(const fixed &that) {
    Fixed_mul(&f, &f, &that.f);
    return *this;
  }

  fixed &operator/=(const fixed &that) {
    Fixed_div(&f, &f, &that.f);
    return *this;
  }

  fixed &operator=(int n) {
    Fixed_int2(&f, n);
    return *this;
  }

  // Arithmetic

  fixed operator+(const fixed &that) const {
    fixed r;
    Fixed_add(&r.f, &f, &that.f);
    return r;
  }

  fixed operator-(const fixed &that) const {
    fixed r;
    Fixed_sub(&r.f, &f, &that.f);
    return r;
  }

  fixed operator*(const fixed &that) const {
    fixed r;
    Fixed_mul(&r.f, &f, &that.f);
    return r;
  }

  fixed operator/(const fixed &that) const {
    fixed r;
    Fixed_div(&r.f, &f, &that.f);
    return r;
  }

  fixed operator/(unsigned that) const {
    fixed r;
    Fixed_divu(&r.f, &f, that);
    return r;
  }

  // Comparison

  bool operator<(const fixed &that) const {
    return Fixed_lt(&f, &that.f);
  }

  bool operator>(const fixed &that) const {
    return Fixed_lt(&that.f, &f);
  }

  bool operator>=(const fixed &that) const {
    return !Fixed_lt(&f, &that.f);
  }

  bool operator<=(const fixed &that) const {
    return !Fixed_lt(&that.f, &f);
  }

  bool operator==(const fixed &that) const {
    return Fixed_eq(&that.f, &f);
  }

  bool operator!=(const fixed &that) const {
    return !Fixed_eq(&that.f, &f);
  }

  // Conversions
  std::string toString(int base = 10) const;

};
#endif

#endif /* FIXED_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
