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
#include <glibmm.h>
#include <stdio.h>
#include "images.h"

/* gdk-pixbuf-csource generates a source file with a gigantic string
 * constant in it.  However Microsoft's compiler is a toy and cannot
 * cope with string literals in excess of 64k.  Therefore it is
 * necessary to rewrite it as a char array. */

int main() {
  size_t n;
  printf("static const unsigned char logodata[]\n"
         "#if __GNUC__\n"
         "    __attribute__((__aligned__(4)))\n"
         "#endif\n"
         "    = {\n");
  for(n = 0; n < sizeof logodata; ++n) {
    if(n % 14 == 0)
      printf("        ");
    int b = logodata[n];
    int padding = (b < 10) + (b < 100) + 1;
    printf("%d,", logodata[n]);
    if(n % 14 == 13 || n + 1 == sizeof logodata)
      printf("\n");
    else
      printf("%*s", padding, "");
  }
  printf("};\n");
  if(ferror(stdout) || fclose(stdout) < 0) {
    perror("stdout");
    return 1;
  }
  return 0;
}
