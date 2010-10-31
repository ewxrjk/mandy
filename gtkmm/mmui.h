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

#ifndef MMUI_H
#define MMUI_H

#include "mandy.h"
#include "IterBuffer.h"

#include <gtkmm.h>

namespace mmui {

  class Toplevel;

  class DrawingArea: public Gtk::DrawingArea {
  public:
    DrawingArea(Toplevel *toplevel);
    bool on_button_press_event(GdkEventButton *);
    bool on_button_release_event(GdkEventButton *);
    bool on_motion_notify_event(GdkEventMotion *);
    bool on_expose_event(GdkEventExpose *);
  private:
    // Reference back to top-level window
    Toplevel &toplevel;

    // Iteration count and pixel data
    IterBuffer *dest;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;

    void Redraw(int x, int y, int w, int h);

    // Dragging support
    bool Dragging;
    double DragFromX, DragFromY;
    double DragToX, DragToY;
    sigc::connection DragIdleConnection;

    void DragComplete();
    bool DragIdle();
  };

  class Toplevel: public Gtk::Window {
  public:
    Toplevel();
    bool on_delete_event(GdkEventAny *);
    bool on_key_release_event(GdkEventKey *);

    // Sub-widgets
    Gtk::VBox vbox;
    DrawingArea draw;

    // Parameters
    double x, y, radius;
    int maxiter;
  };

}

#endif /* MMUI_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
