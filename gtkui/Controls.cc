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
#include "View.h"
#include "Controls.h"
#include <cerrno>
#include "arith.h"
#include "fixed.h"
#include "double.h"

namespace mmui {

  // Control Panel ------------------------------------------------------------

  ControlPanel::ControlPanel(View *v):
    view(v),
    xcenter_caption("X center"),
    ycenter_caption("Y center"),
    radius_caption("Radius"),
    maxiters_caption("Iterations"),
    xcenter_control(this, &view->xcenter, -arith_traits<arith_t>::maximum(),
                    arith_traits<arith_t>::maximum()),
    ycenter_control(this, &view->ycenter, -arith_traits<arith_t>::maximum(),
                    arith_traits<arith_t>::maximum()),
    radius_control(this, &view->radius, 0, arith_traits<arith_t>::maximum()),
    maxiters_control(this, &view->maxiters, 1, INT_MAX - 1) {
    attach(xcenter_caption, 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(xcenter_control, 1, 2, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(ycenter_caption, 0, 1, 1, 2, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(ycenter_control, 1, 2, 1, 2, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(radius_caption, 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(radius_control, 1, 2, 2, 3, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(maxiters_caption, 2, 3, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
    attach(maxiters_control, 3, 4, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
    Update();
  }

  void ControlPanel::Activated() {
    Glib::ustring value;
    for(size_t n = 0; n < controls.size(); ++n) {
      Control *c = controls[n];
      if(!c->Valid(c->get_text().c_str())) {
	c->grab_focus();
	view->get_window()->beep();
	return;
      }
    }
    for(size_t n = 0; n < controls.size(); ++n) {
      Control *c = controls[n];
      c->Render(value);
      if(value != c->get_text())
	c->Set(c->get_text().c_str());
    }
    int w, h;
    view->get_window()->get_size(w, h);
    view->NewLocation(w / 2, h / 2);
  }

  void ControlPanel::Update() {
    Glib::ustring value;
    for(size_t n = 0; n < controls.size(); ++n) {
      Control *c = controls[n];
      c->Render(value);
      c->set_text(value);
    }
  }

  // Captions -----------------------------------------------------------------

  Caption::Caption(const char *caption): Gtk::Label(caption) {
    set_alignment(1.0, 0.0);
  }

  // Base entry widget --------------------------------------------------------

  Control::Control(ControlPanel *p): parent(p) {
    p->Attach(this);
  }

  void Control::on_activate() {
    parent->Activated();
  }

  // Integer entry widget -----------------------------------------------------

  bool IntegerControl::Valid(const char *s) const {
    char *end;
    errno = 0;
    long n = strtol(s, &end, 10);
    if(errno || n < min || n > max || *end)
      return false;
    else
      return true;
  }

  void IntegerControl::Set(const char *s) {
    *value = strtol(s, NULL, 10);
  }

  void IntegerControl::Render(Glib::ustring &s) const {
    char buffer[128];

    snprintf(buffer, sizeof buffer, "%d", *value);
    s.assign(buffer);
  }

  // Real entry widget ------------------------------------------------------

  bool RealControl::Valid(const char *s) const {
    char *end = NULL;                   // TODO
    errno = 0;
    arith_t n = arith_traits<arith_t>::fromString(s, &end);
    if(errno || n < min || n > max || *end)
      return false;
    else
      return true;
  }

  void RealControl::Set(const char *s) {
    *value = arith_traits<arith_t>::fromString(s, NULL);
  }

  void RealControl::Render(Glib::ustring &s) const {
    s.assign(arith_traits<arith_t>::toString(*value));
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
