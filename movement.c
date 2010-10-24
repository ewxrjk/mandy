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

// Perform a drag (in pixel units)
void drag(int w, int h, int deltax, int deltay) {
  if(w > h) {
    xcenter -= deltax * size * 2 / h;
    ycenter += deltay * size * 2 / h;
  } else {
    xcenter -= deltax * size * 2 / w;
    ycenter += deltay * size * 2 / w;
  }
}

/* Perform a zoom.  scale is the ratio of 'size', less than 1 to zoom
 * in and more than 1 to zoom out. */
void zoom(int w, int h, int x, int y, double scale) {
#if 1
  /* The idea is that when you click on a point, it should zoom
   * *around that point*.  Formally we require that:
   *
   * xposition_0(w, h, x) = xposition_1(w, h, x)
   * xposition_0(w, h, y) = xposition_1(w, h, y)
   *
   * Where [xy]position_0 use [xy]center_0 and size_0 (the before values)
   * and [xy]position_1 use [xy]centre_1 and size_1 (the after values).
   *
   * We know size_1 = k*size_0 (for some scale factor k) and what we
   * are after is [xy]centre_1.
   *
   * Expanding on the X axis for the w>h case:
   *
   * xposition_0(w,h,x) = xleft_0(w,h)+x*xsize_0(w,h)/w
   *                    = xcenter_0 - size_0*w/h + x*(size_0*w*2/h)/w
   *                    = xcenter_0 + size_0*(x*2 - w)/h
   * xposition_1(w,h,x) = xcenter_1 + size_1*(x*2 - w)/h
   *
   * Equating these:
   *
   * xcenter_0 + size_0*(x*2 - w)/h = xcenter_1 + size_1*(x*2 - w)/h
   * xcenter_1 = xcenter_0 + size_0*(x*2 - w)/h - size_1*(x*2 - w)/h
   *           = xcenter_0 + (size_0-size_1) * (x*2 - w)/h
   *           = xcenter_0 + size_0*(1-k) * (x*2 - w)/h
   *
   * For the h>w case:
   *
   * xposition_0(w,h,x) = xleft_0(w,h)+x*xsize_0(w,h)/w
   *                    = xcenter_0 - size_0 + x*size_0*2/w
   *                    = xcenter_0 + size_0*(x*2/w - 1)
   * xposition_1(w,h,x) = xcenter_1 + size_1*(x*2/w - 1)
   *
   * Equating:
   *
   * xcenter_0 + size_0*(x*2/w - 1) = xcenter_1 + size_1*(x*2/w - 1)
   * xcenter_1 = xcenter_0 + (size_0-size_1)*(x*2/w - 1)
   *           = xcenter_0 + size_0*(1-k)*(x*2/w - 1)
   */
  if(w > h) {
    xcenter += size * (1-scale) * (x * 2 - w) / h;
    ycenter += size * (1-scale) * ((h - 1 - y) * 2.0 / h - 1);
  } else {
    xcenter += size * (1-scale) * (x * 2.0 / w - 1);
    ycenter += size * (1-scale) * ((h - 1 - y) * 2 - h) / w;
  }
  size = size * scale;
#else
  // This is actually zoom + recenter.  It's a bit annoying in
  // practice.
  xcenter = xposition(w, h, x);
  ycenter = yposition(w, h, y);
  size = size * sqrt(0.5);
#endif
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
