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
                  false),
    arith_control(this, &view->arith_string,
                  &arith_names[0], &arith_names[arith_limit])
  {
    xcenter_control.Attach(0, 0, "X center");
    ycenter_control.Attach(0, 1, "Y center");
    radius_control.Attach(0, 2, "Radius");
    maxiters_control.Attach(0, 3, "Iterations");

    xpointer_control.Attach(1, 0, "X position");
    ypointer_control.Attach(1, 1, "Y position");
    count_control.Attach(1, 2, "Count");
    arith_control.Attach(1, 3, "Precision");

    UpdateDisplay();
  }

  void ControlPanel::Activated() {
    int w, h;
    view->get_window()->get_size(w, h);
    view->NewLocation(w / 2, h / 2);
  }

  void ControlPanel::controlChanged(Control *c) {
    if(c == &arith_control) {
      c->UpdateUnderlying();
      arith_type new_arith = string_to_arith(view->arith_string);
      if(new_arith != view->arith) {
        view->arith = new_arith;
        view->NewLocation();
      }
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
