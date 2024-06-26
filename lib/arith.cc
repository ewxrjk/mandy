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
#include "mandy.h"
#include "arith.h"

const char *const arith_names[] = {
    "double",
#if SIMD
    "simd",
#endif
    "long double",
    "fixed64",
    "fixed128",
    "fixed256",
};

arith_type string_to_arith(const std::string &s) {
  for(size_t a = 0; a < arith_limit; ++a)
    if(arith_names[a] == s)
      return arith_type(a);
  abort();
}

int iterate(arith_t zx, arith_t zy, arith_t cx, arith_t cy, int maxiters, arith_type arith, double &r2) {
  switch(arith) {
  case arith_double: return arith_traits<double>::iterate(zx, zy, cx, cy, maxiters, r2); break;
  case arith_long_double: return arith_traits<long double>::iterate(zx, zy, cx, cy, maxiters, r2); break;
  case arith_fixed64: return arith_traits<fixed64>::iterate(zx, zy, cx, cy, maxiters, r2); break;
  case arith_fixed128: return arith_traits<fixed128>::iterate(zx, zy, cx, cy, maxiters, r2); break;
  case arith_fixed256: return arith_traits<fixed256>::iterate(zx, zy, cx, cy, maxiters, r2); break;
  default: throw std::logic_error("iterate unrecognized/unsuitable arith_t");
  }
}
