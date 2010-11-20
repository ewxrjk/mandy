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
#ifndef FIXED64_H
#define FIXED64_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int64_t Fixed64;

  Fixed64 Fixed64_mul(Fixed64 a, Fixed64 b);
  Fixed64 Fixed64_div(Fixed64 a, Fixed64 b);
  Fixed64 Fixed64_sqrt(Fixed64 a);

  static inline Fixed64 Fixed64_int2(int i) {
    return (int64_t)i << 56;
  }

  char *Fixed64_2str(char buffer[], unsigned bufsize, Fixed64 a, int base);
  int Fixed64_str2(Fixed64 *r, const char *s, char **endptr);

  static inline Fixed64 Fixed64_double2(double n) {
    return (Fixed64)(n* 72057594037927936.0);
  }

  static inline double Fixed64_2double(Fixed64 a) {
    return (double)a / 72057594037927936.0;
  }

  int Fixed64_iterate(Fixed64 zx, Fixed64 zy,
                      Fixed64 cx, Fixed64 cy,
                      double *r2p,
                      int maxiters);

#ifdef __cplusplus
}
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
