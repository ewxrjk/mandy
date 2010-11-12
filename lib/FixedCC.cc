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
#include <config.h>
#include "Fixed.h"

std::string fixed::toString(int base) const {
  char buffer[256];

  Fixed_2str(buffer, sizeof buffer, &f, base);
  return buffer;
}

std::string fixed::toHex() const {
  char buffer[10 * NFIXED + 10];
  sprintf(buffer, "%08x.%08x", f.word[NFIXED-1], f.word[NFIXED-2]);
  for(int n = NFIXED-3; n >= 0; --n)
    sprintf(buffer + strlen(buffer), " %08x", f.word[n]);
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
