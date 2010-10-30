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

// Called to just redraw whatever we've got
void Gtkui::Redraw(int x, int y, int w, int h) {
  gdk_draw_pixbuf(Gtkui::Drawable,
		  Gtkui::GC,
		  Gtkui::LatestPixbuf,
		  x, y, x, y, w, h,
		  GDK_RGB_DITHER_NONE, 0, 0);
}

// Job completion callback
void Gtkui::Completed(Job *generic_job) {
  MandelbrotJob *j = dynamic_cast<MandelbrotJob *>(generic_job);
  // Ignore stale jobs
  if(j->dest != Gtkui::LatestDest)
    return;
  const int w = Gtkui::LatestDest->w;
  guchar *const pixels = gdk_pixbuf_get_pixels(Gtkui::LatestPixbuf);
  const int rowstride = gdk_pixbuf_get_rowstride(Gtkui::LatestPixbuf);
  const int lx = j->x + j->w;
  const int ly = j->y + j->h;
  for(int y = j->y; y < ly; ++y) {
    int *datarow = &Gtkui::LatestDest->data[y * w + j->x];
    guchar *pixelrow = pixels + y * rowstride + j->x * 3;
    for(int x = j->x; x < lx; ++x) {
      const int count = *datarow++;
      *pixelrow++ = colors[count].r;
      *pixelrow++ = colors[count].g;
      *pixelrow++ = colors[count].b;
    }
  }
  Gtkui::Redraw(j->x, j->y, j->w, j->h);
}

// Called to set a new location, scale or maxiter
void Gtkui::NewLocation(int xpos, int ypos) {
  if(Gtkui::LatestDest) {
    Gtkui::LatestDest->release();
    Gtkui::LatestDest = NULL;
  }
  gint w, h;
  gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
  if(!Gtkui::LatestPixbuf)
    Gtkui::LatestPixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
  // TODO if there's a pixbuf available then ideally we would move or scale it
  // to provide continuity.
  if(xpos == -1 || ypos == -1)
    gtk_widget_get_pointer(Gtkui::DrawingArea, &xpos, &ypos);
  Gtkui::LatestDest = MandelbrotJob::recompute(xcenter, ycenter, size,
                                             maxiter, w, h,
                                             Completed,
                                             xpos, ypos);
}

// Called when a resize is detected
void Gtkui::NewSize() {
  // If there's a pixbuf it'll be the wrong size, so delete it.
  if(Gtkui::LatestPixbuf) {
    // TODO ideally we would rescale the pixbuf
    Redraw(0, 0,
	   gdk_pixbuf_get_width(Gtkui::LatestPixbuf),
	   gdk_pixbuf_get_height(Gtkui::LatestPixbuf));
    gdk_pixbuf_unref(Gtkui::LatestPixbuf);
    Gtkui::LatestPixbuf = NULL;
  }
  gint w, h;
  gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
  Gtkui::NewLocation(w/2, h/2);
}
