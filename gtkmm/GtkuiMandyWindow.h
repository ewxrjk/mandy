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
#ifndef GTKUIMANDYWINDOW_H
#define GTKUIMANDYWINDOW_H

#include "GtkuiMandyDrawingArea.h"

namespace Gtkui {

  class MandyWindow: public Gtk::Window {
  public:
    MandyWindow();
    bool on_delete_event(GdkEventAny *);
    bool on_key_release_event(GdkEventKey *);

    Gtk::VBox vbox;
    MandyDrawingArea DrawingArea;
  };

}

#endif /* GTKUIMANDYWINDOW_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
