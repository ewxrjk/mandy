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
#include "Gtkui.h"
#include <cmath>

namespace Gtkui {

  /* Drag state */
  static gboolean Dragging;
  static double DragFromX, DragFromY;
  static double DragToX, DragToY;

  /* Drag from DragFrom[xy] to a new pointer location */
  static void DragComplete() {
    const int deltax = DragToX - DragFromX;
    const int deltay = DragToY - DragFromY;
    if(!(deltax == 0 && deltay == 0)) {
      DragFromX = DragToX;
      DragFromY = DragToY;
      gint w, h;
      gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
      drag(w, h, deltax, deltay);
      Gtkui::Changed();
      Gtkui::NewLocation(DragToX, DragToY);
    }
  }

  static guint DragIdleHandle;

  static gboolean DragIdle(gpointer) {
    DragComplete();
    DragIdleHandle = 0;
    return FALSE;
  }

  /* motion-notify-event callback */
  gboolean PointerMoved(GtkWidget *,
			GdkEventMotion *event,
			gpointer) {
    if(!Dragging)
      return FALSE;
    DragToX = event->x;
    DragToY = event->y;
    if(DragIdleHandle == 0)
      DragIdleHandle = g_idle_add(DragIdle, NULL);
    return TRUE;
  }

  /* button-{press,release}-event callback */
  gboolean ButtonPressed(GtkWidget *, GdkEventButton *event, gpointer) {
    // Double-click left button zooms in
    if(event->type == GDK_2BUTTON_PRESS
       && event->button == 1
       && event->state == 0) {
      gint w, h;
      gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
      zoom(w, h, event->x, event->y, M_SQRT1_2);
      Gtkui::Changed();
      Gtkui::NewLocation(-1, -1);
      return TRUE;
    }
    // Double-click right button zooms out
    if(event->type == GDK_2BUTTON_PRESS
       && event->button == 3
       && event->state == 0) {
      gint w, h;
      gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
      zoom(w, h, event->x, event->y, M_SQRT2);
      Gtkui::Changed();
      Gtkui::NewLocation(-1, -1);
      return TRUE;
    }
    // Hold left button drags
    if(event->type == GDK_BUTTON_PRESS
       && event->button == 1
       && event->state == 0) {
      Dragging = TRUE;
      DragFromX = event->x;
      DragFromY = event->y;
      return TRUE;
    }
    if(event->type == GDK_BUTTON_RELEASE
       && event->button == 1) {
      DragToX = event->x;
      DragToY = event->y;
      DragComplete();
      Dragging = FALSE;
      return TRUE;
    }
    return FALSE;
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
