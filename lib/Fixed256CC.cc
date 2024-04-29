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
#include <cstdio>
#include "Fixed256.h"

std::string fixed256::toString(int base) const {
  char buffer[512];

  Fixed256_2str(buffer, sizeof buffer, &f, base);
  return buffer;
}

std::string fixed256::toHex() const {
  char buffer[256];
  sprintf(buffer, "%016lx.%016lx %016lx %016lx", f.u64[3], f.u64[2], f.u64[1], f.u64[0]);
  return buffer;
}

/*
Local Variables:
mode:c
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
