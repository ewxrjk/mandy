/* Copyright © 2010, 2012 Richard Kettlewell.
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
#include "ControlPanel.h"
#include "FractalJob.h"
#include "Color.h"
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/image.h>
#include <algorithm>
#include <cstring>
#include "arith.h"

namespace mmui {
View::View() {
  set_size_request(384, 384);
  add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK
             | Gdk::POINTER_MOTION_MASK);
}

// Mouse movement -----------------------------------------------------------

bool View::on_button_press_event(GdkEventButton *event) {
  // Double-click left button zooms in
  if(event->type == GDK_2BUTTON_PRESS && event->button == 1
     && !(event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_LOCK_MASK))) {
    Zoom(event->x, event->y, M_SQRT1_2);
    if(controls)
      controls->UpdateDisplay();
    NewLocation();
    return true;
  }
  // Double-click right button zooms out; control-double-click left also works.
  if(event->type == GDK_2BUTTON_PRESS
     && ((event->button == 3
          && !(event->state
               & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_LOCK_MASK)))
         || ((event->button == 1
              && (event->state
                  & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_LOCK_MASK))
                     == GDK_CONTROL_MASK)))) {
    Zoom(event->x, event->y, M_SQRT2);
    if(controls)
      controls->UpdateDisplay();
    NewLocation();
    return true;
  }

  // Hold left button drags
  if(event->type == GDK_BUTTON_PRESS && event->button == 1
     && !(event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_LOCK_MASK))) {
    dragging = true;
    dragFromX = event->x;
    dragFromY = event->y;
    return true;
  }
  return false;
}

bool View::on_button_release_event(GdkEventButton *event) {
  // Release left button ends drag
  if(event->type == GDK_BUTTON_RELEASE && event->button == 1) {
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

void View::NewPointer(int xpos, int ypos) {
  GetCoordinates(xpointer, ypointer, xpos, ypos);
  if(xpos >= 0 && ypos >= 0 && xpos < dest->width() && ypos < dest->height()) {
    count = dest->pixel(xpos, ypos);
    if(controls)
      controls->UpdateDisplay();
  }
}

bool View::on_motion_notify_event(GdkEventMotion *event) {
  NewPointer(event->x, event->y);
  if(!dragging)
    return false;
  dragToX = event->x;
  dragToY = event->y;
  if(!dragIdleConnection.connected())
    dragIdleConnection =
        Glib::signal_idle().connect(sigc::mem_fun(*this, &View::DragIdle));
  return true;
}

void View::GetCoordinates(arith_t &x, arith_t &y, int xpos, int ypos) {
  int w, h;
  get_window()->get_size(w, h);
  if(w > h) {
    x = xcenter + radius * (xpos * 2.0 - w) / h;
    y = ycenter - radius * (ypos * 2.0 / h - 1);
  } else {
    x = xcenter - radius * (xpos * 2.0 / w - 1);
    y = ycenter + radius * (ypos * 2.0 - h) / w;
  }
}

bool View::DragIdle() {
  DragComplete();
  dragIdleConnection.disconnect();
  return false;
}

void View::DragComplete() {
  const int deltax = (int)(dragToX - dragFromX);
  const int deltay = (int)(dragToY - dragFromY);
  if(!(deltax == 0 && deltay == 0)) {
    dragFromX = dragToX;
    dragFromY = dragToY;
    Drag(deltax, deltay);
    if(controls)
      controls->UpdateDisplay();
    NewLocation((int)dragToX, (int)dragToY);
  }
}

// Redrawing ----------------------------------------------------------------

bool View::on_expose_event(GdkEventExpose *) {
  int w, h;
  get_window()->get_size(w, h);
  if(!pixbuf || w != pixbuf->get_width() || h != pixbuf->get_height()) {
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
  get_window()->draw_pixbuf(get_style()->get_fg_gc(Gtk::STATE_NORMAL), pixbuf,
                            x, y, x, y, w, h, Gdk::RGB_DITHER_NONE, 0, 0);
}

// Recolor the entire view
void View::NewPixels() {
  int w, h;
  get_window()->get_size(w, h);
  NewPixels(0, 0, w, h);
}

// Recolor a region of the view
void View::NewPixels(int px, int py, int pw, int ph) {
  guint8 *pixels = pixbuf->get_pixels();
  const int rowstride = pixbuf->get_rowstride();
  const int lx = px + pw;
  const int ly = py + ph;
  for(int y = py; y < ly; ++y) {
    count_t *datarow = &dest->pixel(px, y);
    guchar *pixelrow = pixels + y * rowstride + px * 3;
    for(int x = px; x < lx; ++x) {
      const count_t count = *datarow++;
      if(count < maxiters) {
        *pixelrow++ = red(count, maxiters);
        *pixelrow++ = green(count, maxiters);
        *pixelrow++ = blue(count, maxiters);
      } else {
        *pixelrow++ = 0;
        *pixelrow++ = 0;
        *pixelrow++ = 0;
      }
    }
  }
}

// Job completion callback
void View::Completed(Job *generic_job, void *data) {
  struct timespec finished;
  clock_gettime(CLOCK_REALTIME, &finished);
  View *v = (View *)data;
  FractalJob *j = dynamic_cast<FractalJob *>(generic_job);
  // Ignore stale jobs
  if(j->dest != v->dest)
    return;
  v->NewPixels(j->x, j->y, j->w, j->h);
  v->Redraw(j->x, j->y, j->w, j->h);
  double elapsed_time =
      finished.tv_sec - v->started.tv_sec
      + (finished.tv_nsec - v->started.tv_nsec) / 1000000000.0;
  char buffer[64];
  snprintf(buffer, sizeof buffer, "%gs", elapsed_time);
  v->elapsed = buffer;
  if(v->controls)
    v->controls->UpdateDisplay();
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
  // Discard stale work
  Job::cancel(this);
  clock_gettime(CLOCK_REALTIME, &started);
  dest = FractalJob::recompute(xcenter, ycenter, radius, maxiters, w, h, arith,
                               Completed, this, xpos, ypos, jobFactory);
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
    arith_t xsizeOld = (wOld > hOld ? radius * 2 * wOld / hOld : radius * 2);
    arith_t xsizeNew = (wNew > hNew ? radius * 2 * wNew / hNew : radius * 2);
    arith_t xpixelOld = xsizeOld / wOld;
    arith_t xpixelNew = xsizeNew / wNew;
    arith_t scale = xpixelOld / xpixelNew;
    // The size of the rescaled image
    int wScaled = arith_traits<arith_t>::toInt(arith_t(wOld) * scale);
    int hScaled = arith_traits<arith_t>::toInt(arith_t(hOld) * scale);
    // The rescaling parameters.  If the rescaled image is narrower than the
    // new window then it will be offset into it, otherwise it will up at the
    // edge.
    arith_t dest_x = wNew > wScaled ? (wNew - wScaled) / 2 : 0;
    arith_t dest_y = hNew > hScaled ? (hNew - hScaled) / 2 : 0;
    // The rescaled image is clipped ot the sizeof the new window.
    arith_t dest_w = std::min(wNew, wScaled);
    arith_t dest_h = std::min(hNew, hScaled);
    // The rescaled image is offset to make the centres lines up.
    arith_t offset_x = (wNew - wScaled) / 2;
    arith_t offset_y = (hNew - hScaled) / 2;
    // Create the new pixbuf
    Glib::RefPtr<Gdk::Pixbuf> newPixbuf =
        Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, wNew, hNew);
    // Areas outside the rescaled image will be mid-grey
    memset(newPixbuf->get_pixels(), 0x80, newPixbuf->get_rowstride() * hNew);
    // Do the scale
    pixbuf->scale(newPixbuf, arith_traits<arith_t>::toInt(dest_x),
                  arith_traits<arith_t>::toInt(dest_y),
                  arith_traits<arith_t>::toInt(dest_w),
                  arith_traits<arith_t>::toInt(dest_h),
                  arith_traits<arith_t>::toDouble(offset_x),
                  arith_traits<arith_t>::toDouble(offset_y),
                  arith_traits<arith_t>::toDouble(scale),
                  arith_traits<arith_t>::toDouble(scale), Gdk::INTERP_NEAREST);
    // Use the new pixbuf henceforth
    pixbuf = newPixbuf;
    Redraw(0, 0, pixbuf->get_width(), pixbuf->get_height());
  }
  NewLocation(wNew / 2, hNew / 2);
}

// Motion -------------------------------------------------------------------

void View::Drag(int deltax, int deltay) {
  int w, h;
  get_window()->get_size(w, h);
  if(w > h) {
    xcenter -= arith_t(deltax) * radius * 2 / h;
    ycenter += arith_t(deltay) * radius * 2 / h;
  } else {
    xcenter -= arith_t(deltax) * radius * 2 / w;
    ycenter += arith_t(deltay) * radius * 2 / w;
  }
}

void View::Zoom(arith_t x, arith_t y, arith_t scale) {
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
    xcenter += radius * (arith_t(1) - scale) * (x * 2 - w) / h;
    ycenter -= radius * (arith_t(1) - scale) * (y * 2 / h - 1);
  } else {
    xcenter += radius * (arith_t(1) - scale) * (x * 2 / w - 1);
    ycenter -= radius * (arith_t(1) - scale) * (y * 2 - h) / w;
  }
  radius *= scale;
}

void View::Save() {
  Gtk::Widget *w = this;
  Gtk::Window *parent;
  do {
    w = w->get_parent();
    assert(w);
  } while((parent = dynamic_cast<Gtk::Window *>(w)) == NULL);
  Gtk::FileChooserDialog chooser(*parent, "Save image",
                                 Gtk::FILE_CHOOSER_ACTION_SAVE);
  chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  chooser.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_ACCEPT);
  if(chooser.run() != Gtk::RESPONSE_ACCEPT)
    return;
  std::string path = chooser.get_filename();
  // TODO check for overwrite
  // Wait for the view to finish rendering.  The user feedback isn't
  // particularly nice but it should at least produce the correct image.
  Job::poll(this);
  pixbuf->save(path, "png");
  // TODO support other file formats!
}

} // namespace mmui

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
