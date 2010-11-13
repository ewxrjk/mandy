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
#ifndef DOUBLE_H
#define DOUBLE_H

#include "arith.h"

template<>
class arith_traits<double> {
public:
  static inline double maximum() {
    return HUGE_VAL;
  }

  static std::string toString(const double &n) {
    char buffer[128];

    snprintf(buffer, sizeof buffer, "%g", n);
    return buffer;
  }

  static int toInt(const double &n) {
    return floor(n);
  }

  static int fromString(double *n, const char *s, char **end) {
    errno = 0;
    *n = strtod(s, end);
    return errno;
  }

  static double toDouble(const double &n) {
    return n;
  }
};

#endif /* DOUBLE_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
