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
#include <cerrno>
#include "arith.h"

namespace mmui {

  // Control Panel ------------------------------------------------------------

  ControlPanel::ControlPanel(View *v):
    view(v),
    xcenter_caption("X center"),
    ycenter_caption("Y center"),
    radius_caption("Radius"),
    maxiters_caption("Iterations"),
    xpointer_caption("X position"),
    ypointer_caption("Y position"),
    count_caption("Count"),
    xcenter_control(this, &view->xcenter, -arith_traits<arith_t>::maximum(),
                    arith_traits<arith_t>::maximum()),
    ycenter_control(this, &view->ycenter, -arith_traits<arith_t>::maximum(),
                    arith_traits<arith_t>::maximum()),
    radius_control(this, &view->radius, 0, arith_traits<arith_t>::maximum()),
    maxiters_control(this, &view->maxiters, 1, INT_MAX - 1),
    xpointer_control(this, &view->xpointer, -arith_traits<arith_t>::maximum(),
                     arith_traits<arith_t>::maximum(),
                     false),
    ypointer_control(this, &view->ypointer, -arith_traits<arith_t>::maximum(),
                     arith_traits<arith_t>::maximum(),
                     false),
    count_control(this, &view->count, 0,
                  arith_traits<arith_t>::maximum(),
                  false)
  {
    attach(xcenter_caption, 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(xcenter_control, 1, 2, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(ycenter_caption, 0, 1, 1, 2, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(ycenter_control, 1, 2, 1, 2, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(radius_caption, 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(radius_control, 1, 2, 2, 3, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(maxiters_caption, 0, 1, 3, 4, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(maxiters_control, 1, 2, 3, 4, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(xpointer_caption, 2, 3, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(xpointer_control, 3, 4, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(ypointer_caption, 2, 3, 1, 2, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(ypointer_control, 3, 4, 1, 2, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(count_caption, 2, 3, 2, 3, Gtk::FILL, Gtk::SHRINK, 1, 1); 
    attach(count_control, 3, 4, 2, 3, Gtk::FILL, Gtk::SHRINK, 1, 1);
    Update();
  }

  void ControlPanel::Activated() {
    int w, h;
    view->get_window()->get_size(w, h);
    view->NewLocation(w / 2, h / 2);
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
