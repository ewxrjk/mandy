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
#ifndef GTKUIMANDYDRAWINGAREA_H
#define GTKUIMANDYDRAWINGAREA_H

namespace Gtkui {

  class MandyDrawingArea: public Gtk::DrawingArea {
  public:
    MandyDrawingArea();
    bool on_button_press_event(GdkEventButton *);
    bool on_button_release_event(GdkEventButton *);
    bool on_motion_notify_event(GdkEventMotion *);
    bool on_expose_event(GdkEventExpose *);
  private:
    bool Dragging;
    double DragFromX, DragFromY;
    double DragToX, DragToY;
    guint DragIdleHandle;
    void DragComplete();
    static gboolean DragIdle(gpointer);
  };

}

#endif /* GTKUIMANDYDRAWINGAREA_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
