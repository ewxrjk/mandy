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
#ifndef MANDY_H
#define MANDY_H

#include <config.h>

struct color {
  unsigned char r, g, b;
};

void fatal(int errno_value, const char *fmt, ...);
void init_threads(void);
void destroy_threads(void);
int *compute(double x, double y, double xsize, int w, int h, int max);
void init_colors();
double xsize(int w, int h);
double ysize(int w, int h);
double xleft(int w, int h);
double ybottom(int w, int h);
double xposition(int w, int h, int x);
double yposition(int w, int h, int y);
void drag(int w, int h, int deltax, int deltay);
void zoom(int w, int h, int x, int y, double scale);
double pixelrate(void);

extern struct color *colors;
extern int maxiter;
extern double xcenter, ycenter, size;

#endif /* MANDY_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
