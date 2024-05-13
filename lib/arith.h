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
#ifndef ARITH_H
#define ARITH_H

#include <string>
#include <cmath>
#include <cerrno>
#include <cstdlib>
#include <cassert>
#include <cstdio>

#include <iostream>
#include <sstream>

#include "Fixed256.h"
#include "Fixed128.h"
#include "Fixed64.h"

typedef fixed256 arith_t;

enum arith_type {
  arith_double,
#if SIMD
  arith_simd,
#endif
  arith_long_double,
  arith_fixed64,
  arith_fixed128,
  arith_fixed256,

  arith_limit
};

extern const char *const arith_names[];

arith_type string_to_arith(const std::string &s);

template <typename T> class arith_traits {
public:
  static T maximum();
  static std::string toString(const T &n);
  static int fromString(T &n, const char *s, char **end);
  static int iterate(T zx, T zy, T cx, T cy, int maxiters, double &r2);
};

static inline count_t transform_iterations(int iterations, double r2, int maxiters) {
  if(iterations == maxiters)
    return maxiters;
  else
    return 1 + iterations - log2(log2(r2));
}

template <typename T> int defaultIterate(T zx, T zy, T cx, T cy, int maxiters, double &r2_out) {
  T r2, zx2, zy2;
  int iterations = 0;
  while(((r2 = (zx2 = arith_traits<T>::square(zx)) + (zy2 = arith_traits<T>::square(zy))) < T(R2LIMIT)) && iterations < maxiters) {
    zy = T(2) * zx * zy + cy;
    zx = zx2 - zy2 + cx;
    ++iterations;
  }
  r2_out = (double)r2;
  assert(r2_out >= 0.0);
  return iterations;
}

template <> class arith_traits<double> {
public:
  static inline double maximum() {
    return HUGE_VAL;
  }

  static inline double square(double x) {
    return x * x;
  }

  static std::string toString(const double &n) {
    char buffer[256];
    sprintf(buffer, "%.16g", n);
    return buffer;
  }

  static int fromString(double &n, const char *s, char **end) {
    errno = 0;
    n = strtod(s, end);
    return errno;
  }

  static int iterate(arith_t zx, arith_t zy, arith_t cx, arith_t cy, int maxiters, double &r2) {
    return defaultIterate((double)zx, (double)zy, (double)cx, (double)cy, maxiters, r2);
  }
};

template <> class arith_traits<long double> {
public:
  static inline long double maximum() {
#ifdef HUGE_VALL
    return HUGE_VALL;
#else
    return HUGE_VAL;
#endif
  }

  static inline long double square(long double x) {
    return x * x;
  }

  static std::string toString(const long double &n) {
    std::stringstream s;

    s << n;
    return s.str();
  }

  static int fromString(long double &n, const char *s, char **end) {
    errno = 0;
    n = strtold(s, end);
    return errno;
  }

  static int iterate(arith_t zx, arith_t zy, arith_t cx, arith_t cy, int maxiters, double &r2) {
    return defaultIterate((long double)zx, (long double)zy, (long double)cx, (long double)cy, maxiters, r2);
  }
};

template <> class arith_traits<fixed256> {
public:
  static inline fixed256 maximum() {
    Fixed256 f = { .u64 = { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff }};
    return fixed256(f);
  }

  static fixed256 inline square(const fixed256 &a) {
    return a.square();
  }

  static std::string toString(const fixed256 &n) {
    return n.toString();
  }

  static int fromString(fixed256 &n, const char *s, char **endptr) {
    return n.fromString(s, endptr);
  }

  static int iterate(fixed256 zx, fixed256 zy, fixed256 cx, fixed256 cy, int maxiters, double &r2_out) {
    Fixed256 r2, zx2, zy2;
    int iterations = 0;
    Fixed256 limit;
    Fixed256_int2(&limit, R2LIMIT);
    for(;;) {
      Fixed256_square(&zx2, &zx.f);
      Fixed256_square(&zy2, &zy.f);
      Fixed256_add(&r2, &zx2, &zy2);
      if(Fixed256_ge(&r2, &limit) || iterations >= maxiters)
        break;
      Fixed256_mul(&zy.f, &zx.f, &zy.f);
      Fixed256_add(&zy.f, &zy.f, &zy.f);
      Fixed256_add(&zy.f, &zy.f, &cy.f);
      Fixed256_sub(&zx.f, &zx2, &zy2);
      Fixed256_add(&zx.f, &zx.f, &cx.f);
      ++iterations;
    }
    r2_out = Fixed256_2double(&r2);
    return iterations;
  }
};

template <> class arith_traits<fixed128> {
public:
  static inline fixed128 maximum() {
    Fixed128 f;
    memset(f.word, 0xFF, sizeof f.word);
    f.word[NFIXED128 - 1] = 0x7FFFFFFF;
    return fixed128(f);
  }

  static fixed128 inline square(const fixed128 &a) {
    return a.square();
  }

  static std::string toString(const fixed128 &n) {
    return n.toString();
  }

  static int fromString(fixed128 &n, const char *s, char **endptr) {
    return n.fromString(s, endptr);
  }

  static int iterate(fixed128 zx, fixed128 zy, fixed128 cx, fixed128 cy, int maxiters, double &r2_out) {
#if HAVE_ASM_FIXED128_ITERATE
    int rawCount = Fixed128_iterate(&zx.f, &zy.f, &cx.f, &cy.f, maxiters);
    // r2 is returned in zx (rather oddly)
    r2_out = (double)zx;
    return rawCount;
#else
    Fixed128 r2, zx2, zy2;
    int iterations = 0;
    Fixed128 limit;
    Fixed128_int2(&limit, R2LIMIT);
    for(;;) {
      Fixed128_square(&zx2, &zx.f);
      Fixed128_square(&zy2, &zy.f);
      Fixed128_add(&r2, &zx2, &zy2);
      if(Fixed128_ge(&r2, &limit) || iterations >= maxiters)
        break;
      Fixed128_mul(&zy.f, &zx.f, &zy.f);
      Fixed128_add(&zy.f, &zy.f, &zy.f);
      Fixed128_add(&zy.f, &zy.f, &cy.f);
      Fixed128_sub(&zx.f, &zx2, &zy2);
      Fixed128_add(&zx.f, &zx.f, &cx.f);
      ++iterations;
    }
    r2_out = Fixed128_2double(&r2);
    return iterations;
#endif
  }
};

template <> class arith_traits<fixed64> {
public:
  static inline fixed64 maximum() {
    fixed64 n;
    n.f = 0x7fffffffffffffffLL;
    return n;
  }

  static fixed64 inline square(const fixed64 &a) {
    return a.square();
  }

  static std::string toString(const fixed64 &n) {
    return n.toString();
  }

  static int fromString(fixed64 n, const char *s, char **endptr) {
    return n.fromString(s, endptr);
  }

  static int iterate(arith_t zxa, arith_t zya, arith_t cxa, arith_t cya, int maxiters, double &r2_out) {
    fixed64 zx = zxa, zy = zya, cx = cxa, cy = cya;
#if HAVE_ASM_FIXED64_ITERATE || 0
    return Fixed64_iterate(zx.f, zy.f, cx.f, cy.f, &r2_out, maxiters);
#else
    return defaultIterate(zx, zy, cx, cy, maxiters, r2_out);
#endif
  }
};

int iterate(arith_t zx, arith_t zy, arith_t cx, arith_t cy, int maxiters, arith_type arith, double &r2);

#endif /* ARITH_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
