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
#include "View.h"
#include "Controls.h"
#include "FractalJob.h"
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/image.h>
#include <algorithm>
#include <cstring>

namespace mmui {
  View::View(): xcenter(0), ycenter(0), radius(2), maxiters(255),
                dest(NULL),
                dragging(false),
                controls(NULL),
                jobFactory(NULL) {
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
       && !(event->state & (GDK_SHIFT_MASK
                            |GDK_CONTROL_MASK
                            |GDK_LOCK_MASK))) {
      Zoom(event->x, event->y, M_SQRT1_2);
      if(controls)
        controls->Update();
      NewLocation();
      return true;
    }
    // Double-click right button zooms out; control-double-click left also works.
    if(event->type == GDK_2BUTTON_PRESS
       && ((event->button == 3
            && !(event->state & (GDK_SHIFT_MASK
                                 |GDK_CONTROL_MASK
                                 |GDK_LOCK_MASK)))
           || ((event->button == 1
                && (event->state & (GDK_SHIFT_MASK
                                    |GDK_CONTROL_MASK
                                    |GDK_LOCK_MASK)) == GDK_CONTROL_MASK)))) {
      Zoom(event->x, event->y, M_SQRT2);
      if(controls)
        controls->Update();
      NewLocation();
      return true;
    }

    // Hold left button drags
    if(event->type == GDK_BUTTON_PRESS
       && event->button == 1
       && !(event->state & (GDK_SHIFT_MASK
                            |GDK_CONTROL_MASK
                            |GDK_LOCK_MASK))) {
      dragging = true;
      dragFromX = event->x;
      dragFromY = event->y;
      return true;
    }
    return false;
  }

  bool View::on_button_release_event(GdkEventButton *event) {
    // Release left button ends drag
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
      if(controls)
        controls->Update();
      NewLocation(dragToX, dragToY);
    }
  }

  // Redrawing ----------------------------------------------------------------

  bool View::on_expose_event(GdkEventExpose *) {
    int w, h;
    get_window()->get_size(w, h);
    if(!pixbuf
       || w != pixbuf->get_width()
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
    FractalJob *j = dynamic_cast<FractalJob *>(generic_job);
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
	*pixelrow++ = v->colors[count].r;
	*pixelrow++ = v->colors[count].g;
	*pixelrow++ = v->colors[count].b;
      }
    }
    v->Redraw(j->x, j->y, j->w, j->h);
  }

  // Called to set a new location, scale or maxiters
  void View::NewLocation(int xpos, int ypos) {
    if(!property_visible())
      return;
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
    dest = FractalJob::recompute(xcenter, ycenter, radius,
                                 maxiters, w, h,
                                 Completed,
                                 this,
                                 xpos, ypos,
                                 jobFactory);
    if(colors.size() != (unsigned)(maxiters + 1))
      NewColors();
  }

  void View::NewSize() {
    if(!property_visible())
      return;
    int wNew, hNew;
    get_window()->get_size(wNew, hNew);
    // If there's a pixbuf it'll be the wrong size, so delete it.  We draw it
    // first to provide visual continuity.
    if(pixbuf) {
      // Figure out the scale factor
      int wOld = pixbuf->get_width(), hOld = pixbuf->get_height();
      double xsizeOld = (wOld > hOld
                        ? radius * 2 * wOld / hOld
                         : radius * 2);
      double xsizeNew = (wNew > hNew
                        ? radius * 2 * wNew / hNew
                         : radius * 2);
      double xpixelOld = xsizeOld / wOld;
      double xpixelNew = xsizeNew / wNew;
      double scale = xpixelOld / xpixelNew;
      // The size of the rescaled image
      int wScaled = wOld * scale, hScaled = hOld * scale;
      // The rescaling parameters.  If the rescaled image is narrower than the
      // new window then it will be offset into it, otherwise it will up at the
      // edge.
      double dest_x = wNew > wScaled ? (wNew - wScaled)/2 : 0;
      double dest_y = hNew > hScaled ? (hNew - hScaled)/2 : 0;
      // The rescaled image is clipped ot the sizeof the new window.
      double dest_w = std::min(wNew, wScaled);
      double dest_h = std::min(hNew, hScaled);
      // The rescaled image is offset to make the centres lines up.
      double offset_x = (wNew - wScaled)/2;
      double offset_y = (hNew - hScaled)/2;
      // Create the new pixbuf
      Glib::RefPtr<Gdk::Pixbuf> newPixbuf
        = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, wNew, hNew);
      // Areas outside the rescaled image will be mid-grey
      memset(newPixbuf->get_pixels(),
             0x80,
             newPixbuf->get_rowstride() * hNew);
      // Do the scale
      pixbuf->scale(newPixbuf,
                    dest_x, dest_y,
                    dest_w, dest_h,
                    offset_x, offset_y,
                    scale, scale,
                    Gdk::INTERP_NEAREST);
      // Use the new pixbuf henceforth
      pixbuf = newPixbuf;
      Redraw(0, 0, pixbuf->get_width(), pixbuf->get_height());
    }
    NewLocation(wNew/2, hNew/2);
  }

  // Colors -------------------------------------------------------------------

  void View::NewColors() {
    colors.resize(maxiters + 1);
    for(int n = 0; n < maxiters; ++n) {
      colors[n].r = (cos(2 * M_PI * n / 256) + 1.0) * 127;
      colors[n].g = (cos(2 * M_PI * n / 1024) + 1.0) * 127;
      colors[n].b = (cos(2 * M_PI * n / 512) + 1.0) * 127;
    }
    colors[maxiters].r = colors[maxiters].g = colors[maxiters].b = 0;
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
     * and [xy]position_1 use [xy]center_1 and size_1 (the after values).
     *
     * We know size_1 = k*size_0 (for some scale factor k) and what we
     * are after is [xy]center_1.
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
      ycenter -= radius * (1-scale) * (y * 2.0 / h - 1);
    } else {
      xcenter += radius * (1-scale) * (x * 2.0 / w - 1);
      ycenter -= radius * (1-scale) * (y * 2.0 - h) / w;
    }
    radius *= scale;
  }

  void View::Save() {
    Gtk::Widget *w = this;
    Gtk::Window *parent;
    do {
      w = w->get_parent();
    } while((parent = dynamic_cast<Gtk::Window *>(w)) == NULL);
    Gtk::FileChooserDialog chooser(*parent,
                                   "Save image",
                                   Gtk::FILE_CHOOSER_ACTION_SAVE);
    chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    chooser.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_ACCEPT);
    if(chooser.run() != Gtk::RESPONSE_ACCEPT)
      return;
    std::string path = chooser.get_filename();
    // TODO check for overwrite
    // TODO wait for pixbuf to finish rendering
    while(Job::pending())
      Job::poll(INT_MAX);
    pixbuf->save(path, "png");
    // TODO support other file formats!
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