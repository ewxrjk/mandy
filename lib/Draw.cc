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
#include "mandy.h"
#include "arith.h"
#include "Draw.h"
#include "MandelbrotJob.h"
#include "Color.h"

void draw(const char *xstr,
          const char *ystr,
          const char *rstr,
          const char *mistr,
          const char *path) {
  arith_t x, y, radius;
  long maxiters;
  char *eptr;
  int error;

  if((error = arith_traits<arith_t>::fromString(x, xstr, &eptr)))
    fatal(error, "cannot convert '%s'", xstr);
  if(eptr == xstr)
    fatal(0, "cannot convert '%s'", xstr);

  if((error = arith_traits<arith_t>::fromString(y, ystr, &eptr)))
    fatal(error, "cannot convert '%s'", ystr);
  if(eptr == ystr)
    fatal(0, "cannot convert '%s'", ystr);

  if((error = arith_traits<arith_t>::fromString(radius, rstr, &eptr)))
    fatal(error, "cannot convert '%s'", rstr);
  if(eptr == rstr)
    fatal(0, "cannot convert '%s'", rstr);
  if(radius <= arith_t(0))
    fatal(0, "cannot convert '%s': too small", rstr);

  maxiters = strtol(mistr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", mistr);
  if(eptr == rstr)
    fatal(0, "cannot convert '%s'", mistr);
  if(maxiters > INT_MAX || maxiters <= 0)
    fatal(0, "cannot convert '%s': out of range", mistr);

  draw(x, y, radius, maxiters, path);
}

static void completed(Job *, void *) {
  fprintf(stderr, ".");
}

void draw(arith_t x, arith_t y, arith_t radius,
	  int maxiters, const char *path) {
  int width = 256, height = 256;
  // TODO pixel sizes
  MandelbrotJobFactory jf;
  IterBuffer *dest = FractalJob::recompute(x, y, radius, maxiters,
					   width, height,
					   completed,
					   NULL,
					   0, 0, &jf);
  Job::pollAll();
  fprintf(stderr, "\n");
  FILE *fp = fopen(path, "wb");
  if(!fp)
    fatal(errno, "opening %s", path);
  if(fprintf(fp, "P6\n%d %d 255\n", width, height) < 0)
    fatal(errno, "writing %s", path);
  for(int y = 0; y < height; ++y) {
    const count_t *datarow = &dest->data[y * width];
    for(int x = 0; x < width; ++x) {
      const count_t count = *datarow++;
      int r, g, b;
      if(count < maxiters) {
	r = red(count, maxiters);
	g = green(count, maxiters);
	b = blue(count, maxiters);
      } else {
	r = g = b = 0;
      }
      if(fprintf(fp, "%c%c%c", r, g, b) < 0)
	fatal(errno, "writing %s", path);
    }
  }
  if(fclose(fp) < 0)
    fatal(errno, "closing %s", path);
}

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
