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
  arith_long_double,
  arith_fixed64,
  arith_fixed128,

  arith_limit
};

extern LIBMANDY_API const char *const arith_names[];

LIBMANDY_API arith_type string_to_arith(const std::string &s);

template<typename T> class arith_traits {
public:
  static T maximum();
  static std::string toString(const T &n);
  static int fromString(T &n, const char *s, char **end);
  static int toInt(const T &n);
  static double toDouble(const T &n);
  static count_t iterate(T zx, T zy, T cx, T cy, int maxiters);
};

template<typename T>
count_t defaultIterate(T zx, T zy, T cx, T cy, int maxiters) {
  T r2, zx2, zy2;
  int iterations = 0;
  while(((r2 = (zx2 = zx * zx) + (zy2 = zy * zy)) < T(64))
        && iterations < maxiters) {
    zy = T(2) * zx * zy  + cy;
    zx = zx2 - zy2 + cx;
    ++iterations;
  }
  if(iterations == maxiters)
    return maxiters;
  else
    return 1 + iterations - log2(log2(arith_traits<T>::toDouble(r2)));
}

template<>
class arith_traits<double> {
public:
  static inline double maximum() {
    return HUGE_VAL;
  }

  static std::string toString(const double &n) {
    char buffer[256];
    sprintf(buffer, "%.16g", n);
    return buffer;
  }

  static int toInt(const double &n) {
    return (int)floor(n);
  }

  static int fromString(double &n, const char *s, char **end) {
    errno = 0;
    n = strtod(s, end);
    return errno;
  }

  static double toDouble(const double &n) {
    return n;
  }

  static count_t iterate(arith_t zx, arith_t zy,
                         arith_t cx, arith_t cy, int maxiters) {
    return defaultIterate(zx.toDouble(),
                          zy.toDouble(),
                          cx.toDouble(),
                          cy.toDouble(), maxiters);
  }

};

template<>
class arith_traits<long double> {
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

  static int toInt(const long double &n) {
    return (int)floorl(n);
  }

  static int fromString(long double &n, const char *s, char **end) {
    errno = 0;
    n = strtold(s, end);
    return errno;
  }

  static double toDouble(const long double &n) {
    return n;
  }

  static count_t iterate(arith_t zx, arith_t zy,
                         arith_t cx, arith_t cy, int maxiters) {
    return defaultIterate(zx.toLongDouble(), 
                          zy.toLongDouble(),
                          cx.toLongDouble(),
                          cy.toLongDouble(),
                          maxiters);
  }

};

template<>
class arith_traits<fixed128> {
public:
  static inline fixed128 maximum() {
    Fixed128 f;
    memset(f.word, 0xFF, sizeof f.word);
    f.word[NFIXED128-1] = 0x7FFFFFFF;
    return fixed128(f);
  }

  static std::string toString(const fixed128 &n) {
    return n.toString();
  }

  static int toInt(const fixed128 &n) {
    return n.toInt();
  }

  static int fromString(fixed128 &n, const char *s, char **endptr) {
    return n.fromString(s, endptr);
  }

  static double toDouble(const fixed128 &n) {
    return n.toDouble();
  }

  static count_t iterate(fixed128 zx, fixed128 zy, fixed128 cx, fixed128 cy, int maxiters) {
#if HAVE_ASM && NFIXED128 == 4
    int rawCount = Fixed128_iterate(&zx.f, &zy.f, &cx.f, &cy.f, maxiters);
    if(rawCount == maxiters)
      return rawCount;
    else {
      // r2 is returned in zx (rather oddly)
      return 1 + rawCount - log2(log2(zx.toDouble()));
    }
#else
    return defaultIterate(zx, zy, cx, cy, maxiters);
#endif
  }
};

template<>
class arith_traits<fixed64> {
public:
  static inline fixed64 maximum() {
    fixed64 n;
    n.f = 0x7fffffffffffffffLL;
    return n;
  }

  static std::string toString(const fixed64 &n) {
    return n.toString();
  }

  static int toInt(const fixed64 &n) {
    return n.toInt();
  }

  static int fromString(fixed64 n, const char *s, char **endptr) {
    return n.fromString(s, endptr);
  }

  static double toDouble(const fixed64 &n) {
    return n.toDouble();
  }

  static count_t iterate(arith_t zxa, arith_t zya, arith_t cxa, arith_t cya,
                         int maxiters) {
    fixed64 zx = zxa, zy = zya, cx = cxa, cy = cya;
#if HAVE_ASM_AMD64_64
    double r2;
    int rawCount = Fixed64_iterate(zx.f, zy.f, cx.f, cy.f, &r2, maxiters);
    if(rawCount == maxiters)
      return rawCount;
    else
      return 1 + rawCount - log2(log2(r2));
#else
    return defaultIterate(zx, zy, cx, cy, maxiters);
#endif
  }
};

LIBMANDY_API count_t iterate(arith_t zx, arith_t zy, arith_t cx, arith_t cy,
                             int maxiters, arith_type arith);

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
