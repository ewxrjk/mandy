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

#include "Fixed128.h"
#include "Fixed64.h"

typedef fixed128 arith_t;

enum arith_type {
  arith_double,
#if SIMD2
  arith_simd2,
#endif
#if SIMD4
  arith_simd4,
#endif
  arith_long_double,
  arith_fixed64,
  arith_fixed128,

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
  while(((r2 = (zx2 = zx * zx) + (zy2 = zy * zy)) < T(64)) && iterations < maxiters) {
    zy = T(2) * zx * zy + cy;
    zx = zx2 - zy2 + cx;
    ++iterations;
  }
  r2_out = (double)r2;
  return iterations;
}

template <> class arith_traits<double> {
public:
  static inline double maximum() {
    return HUGE_VAL;
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

template <> class arith_traits<fixed128> {
public:
  static inline fixed128 maximum() {
    Fixed128 f;
    memset(f.word, 0xFF, sizeof f.word);
    f.word[NFIXED128 - 1] = 0x7FFFFFFF;
    return fixed128(f);
  }

  static std::string toString(const fixed128 &n) {
    return n.toString();
  }

  static int fromString(fixed128 &n, const char *s, char **endptr) {
    return n.fromString(s, endptr);
  }

  static int iterate(fixed128 zx, fixed128 zy, fixed128 cx, fixed128 cy, int maxiters, double &r2) {
#if HAVE_ASM_128 && NFIXED128 == 4
    int rawCount = Fixed128_iterate(&zx.f, &zy.f, &cx.f, &cy.f, maxiters);
    // r2 is returned in zx (rather oddly)
    r2 = (double)zx;
    return rawCount;
#else
    return defaultIterate(zx, zy, cx, cy, maxiters, r2);
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

  static std::string toString(const fixed64 &n) {
    return n.toString();
  }

  static int fromString(fixed64 n, const char *s, char **endptr) {
    return n.fromString(s, endptr);
  }

  static int iterate(arith_t zxa, arith_t zya, arith_t cxa, arith_t cya, int maxiters, double &r2) {
    fixed64 zx = zxa, zy = zya, cx = cxa, cy = cya;
#if HAVE_ASM_64
    return Fixed64_iterate(zx.f, zy.f, cx.f, cy.f, &r2, maxiters);
#else
    return defaultIterate(zx, zy, cx, cy, maxiters, r2);
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
