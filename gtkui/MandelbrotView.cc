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
#include "JuliaView.h"
#include "MandelbrotView.h"

namespace mmui {

  MandelbrotView::MandelbrotView(): juliaView(NULL) {
    SetJobFactory(&mandelbrotJobFactory);
  }

  bool MandelbrotView::on_button_press_event(GdkEventButton *event) {

    // Shifted left button draws the corresponding Julia set
    if(event->type == GDK_BUTTON_PRESS
       && event->button == 1
       && (event->state & (GDK_SHIFT_MASK
                           |GDK_CONTROL_MASK
                           |GDK_LOCK_MASK)) == GDK_SHIFT_MASK) {
      NewJulia(event->x, event->y);
      return true;
    }

    return View::on_button_press_event(event);
  }

  bool MandelbrotView::on_button_release_event(GdkEventButton *event) {
    // Release shifted left button draws the corresponding Julia set
    if(event->type == GDK_BUTTON_RELEASE
       && event->button == 1
       && (event->state & (GDK_SHIFT_MASK
                           |GDK_CONTROL_MASK
                           |GDK_LOCK_MASK)) == GDK_SHIFT_MASK) {
      NewJulia(event->x, event->y);
      return true;
    }
    return View::on_button_release_event(event);
  }

  bool MandelbrotView::on_motion_notify_event(GdkEventMotion *event) {
    // Drag with shifted left button draws the corresponding Julia set
    if((event->state & (GDK_BUTTON1_MASK
                        |GDK_SHIFT_MASK
                        |GDK_CONTROL_MASK
                        |GDK_LOCK_MASK)) == (GDK_BUTTON1_MASK|GDK_SHIFT_MASK)) {
      NewJulia(event->x, event->y);
      return true;
    }
    return View::on_motion_notify_event(event);
  }

  // Julia set integration ----------------------------------------------------

  void MandelbrotView::NewJulia(double xpos, double ypos) {
    if(juliaView) {
      int w, h;
      double x, y;
      get_window()->get_size(w, h);
      if(w > h) {
        x = xcenter + radius * (xpos * 2.0 - w)/h;
        y = ycenter - radius * (ypos * 2.0 / h - 1);
      } else {
        x = xcenter - radius * (xpos * 2.0 / w - 1);
        y = ycenter + radius * (ypos * 2.0 - h)/w;
      }
      juliaView->Update(x, y);
    }
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
