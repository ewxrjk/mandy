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
#include "MainMenu.h"
#include "JuliaWindow.h"
#include <gtkmm/frame.h>
#include <gtkmm/box.h>

namespace mmui {

  JuliaWindow::JuliaWindow(): controls(&view) {
    view.SetControlPanel(&controls);
    Gtk::Frame *frame = manage(new Gtk::Frame());
    frame->add(controls);
    Gtk::VBox *vbox = manage(new Gtk::VBox(false, 0));
    vbox->pack_start(*manage(new Menubar()), false, false, 1);
    vbox->pack_start(*frame, false, false, 1);
    vbox->pack_end(view, true, true, 0);
    add(*vbox);
    set_title("Julia Set");
  }

  bool JuliaWindow::on_delete_event(GdkEventAny *) {
    hide();
    return true;
  }

  JuliaView::JuliaView() {
    SetJobFactory(&juliaJobFactory);
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
