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
#include "mandy.h"

/* How locations and sizes are treated:
 *
 * PIXEL UNITS
 * ===========
 *
 * These are (usually) integer valued and locate pixels on the screen.
 * They are also therefore the units of the mouse pointer.  (0,0) is
 * the top left.
 *
 * PLANE UNITS
 * ===========
 *
 * These identify a location in the complex plane.  It is represented
 * in the conventional way, i.e. with increasing real (X) values
 * proceeding from left to right and increasing imaginary (Y) values
 * proceeding from bottom to top.
 *
 * The objects x and y give the center of the window in plane units
 * and the object size gives the radius of the largest displayable
 * circle, i.e. the distance parallel to the real or imaginary axis
 * from the center to the nearest window edge.  The reason for this is
 * to produce sensible behavior as the window is resized.
 *
 * THE ITERBUFFER->DATA[] ARRAY
 * ============================
 *
 * This is addressed in pixel units.  Values within one row proceed from left
 * to right as addresses increase and the lowest-addressed row belongs at the
 * bottom of the screen and corresponds to the largest imaginary (Y) value in
 * plane units.  This means that you have to invert Y coordinates when
 * converting between pixel and plane units.
 */

// Location of the center of the window
double xcenter = 0.0;
double ycenter = 0.0;

// Distance to the nearest edge
double size = 2.0;

// Get the width of the window in plane units
double xsize(int w, int h) {
  if(w > h)
    return size * w * 2 / h;
  else
    return size * 2;
}

// Get the height of the window in plane units
double ysize(int w, int h) {
  if(w > h)
    return size * 2;
  else
    return size * h * 2 / w;
}

// Get the left (least real) boundary of the window in plane units
double xleft(int w, int h) {
  if(w > h)
    return xcenter - size * w / h;
  else
    return xcenter - size;
}

// Get the lower (least imaginary) boundary of the window in plane units
double ybottom(int w, int h) {
  if(w > h)
    return ycenter - size;
  else
    return ycenter - size * h / w;
}

// Get the plane X position of a window coordinate
double xposition(int w, int h, int x) {
  return xleft(w, h) + x * xsize(w, h) / w;
}

// Get the plane Y position of a window coordinate
double yposition(int w, int h, int y) {
  return ybottom(w, h) + (h - 1 - y) * ysize(w, h) / h;
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
