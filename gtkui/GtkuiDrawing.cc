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
#include "MandelbrotJob.h"
#include "Gtkui.h"

namespace Gtkui {

  // Called to just redraw whatever we've got
  void Redraw(int x, int y, int w, int h) {
    gdk_draw_pixbuf(Drawable,
		    GC,
		    LatestPixbuf,
		    x, y, x, y, w, h,
		    GDK_RGB_DITHER_NONE, 0, 0);
  }

  // Job completion callback
  void Completed(Job *generic_job) {
    MandelbrotJob *j = dynamic_cast<MandelbrotJob *>(generic_job);
    // Ignore stale jobs
    if(j->dest != LatestDest)
      return;
    const int w = LatestDest->w;
    guchar *const pixels = gdk_pixbuf_get_pixels(LatestPixbuf);
    const int rowstride = gdk_pixbuf_get_rowstride(LatestPixbuf);
    const int lx = j->x + j->w;
    const int ly = j->y + j->h;
    for(int y = j->y; y < ly; ++y) {
      int *datarow = &LatestDest->data[y * w + j->x];
      guchar *pixelrow = pixels + y * rowstride + j->x * 3;
      for(int x = j->x; x < lx; ++x) {
	const int count = *datarow++;
	*pixelrow++ = colors[count].r;
	*pixelrow++ = colors[count].g;
	*pixelrow++ = colors[count].b;
      }
    }
    Redraw(j->x, j->y, j->w, j->h);
  }

  // Called to set a new location, scale or maxiter
  void NewLocation(int xpos, int ypos) {
    if(LatestDest) {
      LatestDest->release();
      LatestDest = NULL;
    }
    gint w, h;
    gdk_drawable_get_size(Drawable, &w, &h);
    if(!LatestPixbuf)
      LatestPixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
    // TODO if there's a pixbuf available then ideally we would move or scale it
    // to provide continuity.
    if(xpos == -1 || ypos == -1)
      gtk_widget_get_pointer(DrawingArea, &xpos, &ypos);
    LatestDest = MandelbrotJob::recompute(xcenter, ycenter, size,
					  maxiter, w, h,
					  Completed,
					  xpos, ypos);
  }

  // Called when a resize is detected
  void NewSize() {
    // If there's a pixbuf it'll be the wrong size, so delete it.
    if(LatestPixbuf) {
      // TODO ideally we would rescale the pixbuf
      Redraw(0, 0,
	     gdk_pixbuf_get_width(LatestPixbuf),
	     gdk_pixbuf_get_height(LatestPixbuf));
      gdk_pixbuf_unref(LatestPixbuf);
      LatestPixbuf = NULL;
    }
    gint w, h;
    gdk_drawable_get_size(Drawable, &w, &h);
    NewLocation(w/2, h/2);
  }

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
