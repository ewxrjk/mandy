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
#include "GenericWindow.h"
#include "ControlPanel.h"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>

namespace mmui {

GenericWindow::GenericWindow(): view(NULL), controls(NULL) {}

void GenericWindow::Initialize(View *view_) {
  view = view_;
  controls = new ControlPanel(view);
  view->SetControlPanel(controls);
  add_events(Gdk::KEY_RELEASE_MASK);
  Gtk::Frame *frame = manage(new Gtk::Frame());
  frame->add(*controls);
  Gtk::VBox *vbox = Gtk::manage(new Gtk::VBox(false, 0));
  vbox->pack_start(*manage(new Menubar()), false, false, 1);
  vbox->pack_start(*frame, false, false, 1);
  vbox->pack_end(*view, true, true, 0);
  add(*vbox);
}

bool GenericWindow::on_key_release_event(GdkEventKey *event) {
  if((event->state & (Gdk::SHIFT_MASK | Gdk::CONTROL_MASK))
     == Gdk::CONTROL_MASK) {
    switch(event->keyval) {
    case GDK_equal:
    case GDK_minus:
    case GDK_KP_Add:
    case GDK_KP_Subtract: {
      int w, h;
      view->get_window()->get_size(w, h);
      if(event->keyval == GDK_equal || event->keyval == GDK_KP_Add)
        view->Zoom(w / 2.0, h / 2.0, M_SQRT1_2);
      else
        view->Zoom(w / 2.0, h / 2.0, M_SQRT2);
      controls->UpdateDisplay();
      view->NewLocation(w / 2, h / 2);
      return true;
    }
    }
  }
  return false;
}

bool GenericWindow::on_delete_event(GdkEventAny *) {
  return close();
}

} // namespace mmui

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
