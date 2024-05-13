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
#ifndef FIXED64_H
#define FIXED64_H

#include "Fixed128.h"
#include "Fixed256.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int64_t Fixed64;

uint64_t Fixed64_div_unsigned(uint64_t a, uint64_t b);
Fixed64 Fixed64_div(Fixed64 a, Fixed64 b);
Fixed64 Fixed64_sqrt(Fixed64 a);

static inline Fixed64 Fixed64_mul(Fixed64 a, Fixed64 b) {
#if __amd64__
  __asm__("imul %[b]\n\t"
          "shrd $56,%%rdx,%%rax\n\t"
          "adc $0,%%rax"
          : [a] "+a"(a) // first input, and output, in rax
          : [b] "r"(b)  // second input anywhere
          : "rdx");     // clobber rdx
  return a;
#else
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
#endif
}

static inline Fixed64 Fixed64_int2(int i) {
  return (Fixed64)((uint64_t)i << 56);
}

char *Fixed64_2str(char buffer[], unsigned bufsize, Fixed64 a, int base);
int Fixed64_str2(Fixed64 *r, const char *s, char **endptr);

static inline Fixed64 Fixed64_double2(double n) {
  return (Fixed64)(n * 72057594037927936.0);
}

static inline double Fixed64_2double(Fixed64 a) {
  return (double)a / 72057594037927936.0;
}

int Fixed128_to_Fixed64(Fixed64 *r, const union Fixed128 *a);
void Fixed64_to_Fixed128(union Fixed128 *r, Fixed64 a);

void Fixed256_to_Fixed64(Fixed64 *r, union Fixed256 *a);

int Fixed64_iterate(Fixed64 zx, Fixed64 zy, Fixed64 cx, Fixed64 cy, double *r2p, int maxiters);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <stdexcept>

class fixed64 {
public:
  Fixed64 f;

  fixed64(int i) {
    f = Fixed64_int2(i);
  }
  fixed64(double n) {
    f = Fixed64_double2(n);
  }
  fixed64() {
    f = 0;
  }
  fixed64(fixed128 ff) {
    int err = Fixed128_to_Fixed64(&f, &ff.f);
    if(err)
      throw std::runtime_error("fixed128 to fixed64 conversion failed");
  }
fixed64(fixed256 ff) {
  Fixed256_to_Fixed64(&f, &ff.f);
}

  // Assignment

  fixed64 &operator/(unsigned that) {
    f /= that;
    return *this;
  }

  fixed64 &operator+=(const fixed64 &that) {
    f += that.f;
    return *this;
  }

  fixed64 &operator-=(const fixed64 &that) {
    f -= that.f;
    return *this;
  }

  fixed64 &operator/=(const fixed64 &that) {
    f = Fixed64_div(f, that.f);
    return *this;
  }

  fixed64 &operator*=(const fixed64 &that) {
    f = Fixed64_mul(f, that.f);
    return *this;
  }

  fixed64 &operator=(int n) {
    f = Fixed64_int2(n);
    return *this;
  }

  // Arithmetic

  fixed64 operator-() const {
    fixed64 r;
    r.f = -f;
    return r;
  }

  fixed64 operator+(const fixed64 &that) const {
    fixed64 r;
    r.f = f + that.f;
    return r;
  }

  fixed64 operator-(const fixed64 &that) const {
    fixed64 r;
    r.f = f - that.f;
    return r;
  }

  fixed64 operator*(const fixed64 &that) const {
    fixed64 r;
    r.f = Fixed64_mul(f, that.f);
    return r;
  }

  fixed64 operator/(const fixed64 &that) const {
    fixed64 r;
    r.f = Fixed64_mul(f, that.f);
    return r;
  }

  fixed64 operator/(unsigned that) const {
    fixed64 r;
    r.f = f / that;
    return r;
  }

  // Comparison

  bool operator<(const fixed64 &that) const {
    return f < that.f;
  }
  bool operator>(const fixed64 &that) const {
    return f > that.f;
  }
  bool operator<=(const fixed64 &that) const {
    return f <= that.f;
  }
  bool operator>=(const fixed64 &that) const {
    return f >= that.f;
  }
  bool operator==(const fixed64 &that) const {
    return f == that.f;
  }
  bool operator!=(const fixed64 &that) const {
    return f != that.f;
  }

  // Conversions
  std::string toString(int base = 10) const;
  std::string toHex() const;

  int fromString(const char *s, char **endptr) {
    return Fixed64_str2(&f, s, endptr);
  }

  explicit operator int() const {
    return f >> 56;
  }

  explicit operator double() const {
    return Fixed64_2double(f);
  }
};

inline fixed64 sqrt(const fixed64 &a) {
  fixed64 r;
  r.f = Fixed64_sqrt(a.f);
  return r;
};
#endif

#endif /* FIXED64_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
