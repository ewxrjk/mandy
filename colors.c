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
#include "mand.h"
#include <math.h>
#include <stdlib.h>

// Color lookup table
struct color *colors;

// Maximum iteration count
int maxiter;

void init_colors(int new_maxiter) {
  if(maxiter == new_maxiter)
    return;
  maxiter = new_maxiter;
  if(colors)
    free(colors);
  colors = malloc((maxiter + 1) * sizeof *colors);
  // The complement is colorful
  for(int n = 0; n < maxiter; ++n) {
#if 0
    colors[n].r = (cos(2 * M_PI * (double)n / maxiter) + 1.0) * 127;
    colors[n].g = 255 - (cos(4 * M_PI * (double)n / maxiter) + 1.0) * 127;
    colors[n].b = 255 - (cos(8 * M_PI * (double)n / maxiter) + 1.0) * 127;
#else
    colors[n].r = (cos(2 * M_PI * n / 256) + 1.0) * 127;
    colors[n].g = (cos(2 * M_PI * n / 1024) + 1.0) * 127;
    colors[n].b = (cos(2 * M_PI * n / 512) + 1.0) * 127;
#endif
  }
  // The set itself is black
  colors[maxiter].r = colors[maxiter].g = colors[maxiter].b = 0;
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
