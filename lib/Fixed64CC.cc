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
#include "Fixed64.h"

std::string fixed64::toString(int base) const {
  char buffer[256];

  Fixed64_2str(buffer, sizeof buffer, f, base);
  return buffer;
}

std::string fixed64::toHex() const {
  char buffer[32];
  sprintf(buffer, "%02x.%06x%08x", (unsigned char)(f >> 56),
          (unsigned)(f >> 32) & 0x00FFFFFF, (unsigned)f);
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
