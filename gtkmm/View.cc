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
#include "mmui.h"

namespace mmui {
  View::View(): xcenter(0), ycenter(0), radius(2), maxiter(255),
                dest(NULL),
                dragging(false) {
    set_size_request(384, 384);
    add_events(Gdk::BUTTON_PRESS_MASK
	       |Gdk::BUTTON_RELEASE_MASK
	       |Gdk::POINTER_MOTION_MASK);
  }

  // Mouse movement -----------------------------------------------------------

  bool View::on_button_press_event(GdkEventButton *event) {
    // Double-click left button zooms in
    if(event->type == GDK_2BUTTON_PRESS
       && event->button == 1
       && event->state == 0) {
      Zoom(event->x, event->y, M_SQRT1_2);
      //Gtkui::Changed();
      NewLocation();
      return true;
    }
    // Double-click right button zooms out
    if(event->type == GDK_2BUTTON_PRESS
       && event->button == 3
       && event->state == 0) {
      Zoom(event->x, event->y, M_SQRT2);
      //Gtkui::Changed();
      NewLocation();
      return true;
    }

    // Hold left button drags
    if(event->type == GDK_BUTTON_PRESS
       && event->button == 1
       && event->state == 0) {
      dragging = true;
      dragFromX = event->x;
      dragFromY = event->y;
      return true;
    }
    return false;
  }

  bool View::on_button_release_event(GdkEventButton *event) {
    if(event->type == GDK_BUTTON_RELEASE
       && event->button == 1) {
      if(dragging) {
        dragToX = event->x;
        dragToY = event->y;
        DragComplete();
        dragging = false;
        return true;
      }
    }
    return false;
  }

  bool View::on_motion_notify_event(GdkEventMotion *event) {
    if(!dragging)
      return false;
    dragToX = event->x;
    dragToY = event->y;
    if(!dragIdleConnection.connected())
      dragIdleConnection = Glib::signal_idle().connect
        (sigc::mem_fun(*this, &View::DragIdle));
    return true;
  }

  bool View::DragIdle() {
    DragComplete();
    dragIdleConnection.disconnect();
    return false;
  }

  void View::DragComplete() {
    const int deltax = dragToX - dragFromX;
    const int deltay = dragToY - dragFromY;
    if(!(deltax == 0 && deltay == 0)) {
      dragFromX = dragToX;
      dragFromY = dragToY;
      Drag(deltax, deltay);
      //Gtkui::Changed();
      NewLocation(dragToX, dragToY);
    }
  }

  // Redrawing ----------------------------------------------------------------

  bool View::on_expose_event(GdkEventExpose *) {
    int w, h;
    get_window()->get_size(w, h);
    if(w != pixbuf->get_width()
       || h != pixbuf->get_height()) {
      // The pixbuf is the wrong size (i.e. the window has been
      // resized).  Attempt a recompute.
      NewSize();
    } else {
      // Just draw what we've got
      // TODO only redraw the bit that was exposed
      Redraw(0, 0, w, h);
    }
    return true;
  }

  void View::Redraw(int x, int y, int w, int h) {
    get_window()->draw_pixbuf(get_style()->get_fg_gc(Gtk::STATE_NORMAL),
                              pixbuf, x, y, x, y, w, h,
                              Gdk::RGB_DITHER_NONE, 0, 0);
  }

  // Job completion callback
  void View::Completed(Job *generic_job, void *data) {
    View *v = (View *)data;
    MandelbrotJob *j = dynamic_cast<MandelbrotJob *>(generic_job);
    // Ignore stale jobs
    if(j->dest != v->dest)
      return;
    const int w = v->dest->w;
    guint8 *pixels = v->pixbuf->get_pixels();
    const int rowstride = v->pixbuf->get_rowstride();
    const int lx = j->x + j->w;
    const int ly = j->y + j->h;
    for(int y = j->y; y < ly; ++y) {
      int *datarow = &v->dest->data[y * w + j->x];
      guchar *pixelrow = pixels + y * rowstride + j->x * 3;
      for(int x = j->x; x < lx; ++x) {
	const int count = *datarow++;
	*pixelrow++ = colors[count].r;
	*pixelrow++ = colors[count].g;
	*pixelrow++ = colors[count].b;
      }
    }
    v->Redraw(j->x, j->y, j->w, j->h);
  }

  // Called to set a new location, scale or maxiter
  void View::NewLocation(int xpos, int ypos) {
    if(dest) {
      dest->release();
      dest = NULL;
    }
    int w, h;
    get_window()->get_size(w, h);
    if(!pixbuf)
      pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, w, h);
    // TODO if there's a pixbuf available then ideally we would move or scale it
    // to provide continuity.
    if(xpos == -1 || ypos == -1)
      get_pointer(xpos, ypos);
    dest = MandelbrotJob::recompute(xcenter, ycenter, radius,
                                    maxiter, w, h,
                                    Completed,
                                    this,
                                    xpos, ypos);
  }

  void View::NewSize() {
    // If there's a pixbuf it'll be the wrong size, so delete it.  We draw it
    // first to provide visual continuity.
    if(pixbuf) {
      // TODO ideally we would rescale the pixbuf
      Redraw(0, 0, pixbuf->get_width(), pixbuf->get_height());
      pixbuf.reset();
    }
    int w, h;
    get_window()->get_size(w, h);
    NewLocation(w/2, h/2);
  }

  // Motion -------------------------------------------------------------------

  void View::Drag(int deltax, int deltay) {
    int w, h;
    get_window()->get_size(w, h);
    if(w > h) {
      xcenter -= deltax * radius * 2 / h;
      ycenter += deltay * radius * 2 / h;
    } else {
      xcenter -= deltax * radius * 2 / w;
      ycenter += deltay * radius * 2 / w;
    }
  }

  void View::Zoom(double x, double y, double scale) {
    int w, h;
    get_window()->get_size(w, h);
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
      xcenter += radius * (1-scale) * (x * 2.0 - w) / h;
      ycenter += radius * (1-scale) * ((h - 1 - y) * 2.0 / h - 1);
    } else {
      xcenter += radius * (1-scale) * (x * 2.0 / w - 1);
      ycenter += radius * (1-scale) * ((h - 1 - y) * 2 - h) / w;
    }
    radius *= scale;
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
