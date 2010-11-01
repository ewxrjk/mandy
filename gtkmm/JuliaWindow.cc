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

  JuliaWindow::JuliaWindow(): controls(&view),
                              vbox(false, 0) {
    view.SetControlPanel(&controls);
    view.SetJobFactory(&juliaFactory);
    frame.add(controls);
    vbox.pack_start(frame, false, false, 1);
    vbox.pack_end(view, true, true, 0);
    add(vbox);
    set_title("Julia Set");
    show_all();
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
