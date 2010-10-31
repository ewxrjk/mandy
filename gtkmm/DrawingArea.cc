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
  DrawingArea::DrawingArea(Toplevel *tl): toplevel(*tl),
                                          Dragging(false) {
    set_size_request(384, 384);
    add_events(Gdk::BUTTON_PRESS_MASK
	       |Gdk::BUTTON_RELEASE_MASK
	       |Gdk::POINTER_MOTION_MASK);
  }

  bool DrawingArea::on_button_press_event(GdkEventButton *event) {
    // Double-click left button zooms in
    if(event->type == GDK_2BUTTON_PRESS
       && event->button == 1
       && event->state == 0) {
      int w, h;
      get_window()->get_size(w, h);
      zoom(w, h, event->x, event->y, M_SQRT1_2);
      //Gtkui::Changed();
      //Gtkui::NewLocation(-1, -1);
      return true;
    }
    // Double-click right button zooms out
    if(event->type == GDK_2BUTTON_PRESS
       && event->button == 3
       && event->state == 0) {
      int w, h;
      get_window()->get_size(w, h);
      zoom(w, h, event->x, event->y, M_SQRT2);
      //Gtkui::Changed();
      //Gtkui::NewLocation(-1, -1);
      return true;
    }
    // Hold left button drags
    if(event->type == GDK_BUTTON_PRESS
       && event->button == 1
       && event->state == 0) {
      Dragging = TRUE;
      DragFromX = event->x;
      DragFromY = event->y;
      return true;
    }
    return false;
  }

  bool DrawingArea::on_button_release_event(GdkEventButton *event) {
    if(event->type == GDK_BUTTON_RELEASE
       && event->button == 1) {
      DragToX = event->x;
      DragToY = event->y;
      DragComplete();
      Dragging = false;
      return true;
    }
    return false;
  }

  bool DrawingArea::on_motion_notify_event(GdkEventMotion *event) {
    if(!Dragging)
      return false;
    DragToX = event->x;
    DragToY = event->y;
    if(!DragIdleConnection.connected())
      DragIdleConnection = Glib::signal_idle().connect
        (sigc::mem_fun(*this, &DrawingArea::DragIdle));
    return true;
  }

  bool DrawingArea::DragIdle() {
    DragComplete();
    DragIdleConnection.disconnect();
    return false;
  }

  void DrawingArea::DragComplete() {
    const int deltax = DragToX - DragFromX;
    const int deltay = DragToY - DragFromY;
    if(!(deltax == 0 && deltay == 0)) {
      DragFromX = DragToX;
      DragFromY = DragToY;
      int w, h;
      get_window()->get_size(w, h);
      drag(w, h, deltax, deltay);
      //Gtkui::Changed();
      //Gtkui::NewLocation(DragToX, DragToY);
    }
  }

  bool DrawingArea::on_expose_event(GdkEventExpose *) {
    int w, h;
    get_window()->get_size(w, h);
    /*
    if(w != gdk_pixbuf_get_width(LatestPixbuf)
       || h != gdk_pixbuf_get_height(LatestPixbuf)) {
      // The pixbuf is the wrong size (i.e. the window has been
      // resized).  Attempt a recompute.
      NewSize();
    } else {
      // Just draw what we've got
      // TODO only redraw the bit that was exposed
      Redraw(0, 0, w, h);
    }
    */
    return true;
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
