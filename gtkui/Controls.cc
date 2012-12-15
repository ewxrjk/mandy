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
#include "Controls.h"
#include <cerrno>
#include "arith.h"

namespace mmui {

  // Generic control container

  void ControlContainer::Update() {
    Glib::ustring value;
    for(size_t n = 0; n < controls.size(); ++n)
      controls[n]->Update();
  }

  void ControlContainer::ContainerActivated() {
    Glib::ustring value;
    for(size_t n = 0; n < controls.size(); ++n) {
      TextEntryControl *c = controls[n];
      if(!c->Valid(c->get_text().c_str())) {
	c->grab_focus();
	get_window()->beep();
	return;
      }
    }
    for(size_t n = 0; n < controls.size(); ++n) {
      TextEntryControl *c = controls[n];
      c->Render(value);
      if(value != c->get_text())
	c->Set(c->get_text().c_str());
    }
    Activated();
  }

  bool ControlContainer::allValid() const {
    for(size_t n = 0; n < controls.size(); ++n) {
      TextEntryControl *c = controls[n];
      if(!c->Valid(c->get_text().c_str()))
        return false;
    }
    return true;
  }

  void ControlContainer::controlChanged(TextEntryControl *) {
  }

  void ControlContainer::Activated() {
  }

  void ControlContainer::sensitive(bool sensitivity) {
    for(size_t n = 0; n < controls.size(); ++n) {
      TextEntryControl *c = controls[n];
      c->set_sensitive(sensitivity);
    }
  }

  // Base for controls --------------------------------------------------------

  void Control::Attach(int x, int y, const char *caption, int width) {
    label.set_text(caption);
    label.set_alignment(1.0, 0.0);
    parent->attach(label,
                   2 * x, 2 * x + 1,
                   y, y + 1,
                   Gtk::FILL, Gtk::SHRINK, 1, 1);
    parent->attach(*widget(),
                   2 * x + 1, 2 * (x + width),
                   y, y + 1,
                   Gtk::FILL, Gtk::SHRINK, 1, 1);
  }

  // Text entry widget --------------------------------------------------------

  TextEntryControl::TextEntryControl(ControlContainer *p,
                                     bool editable_): Control(p) {
    p->Attach(this);
    set_editable(editable_);
  }

  void TextEntryControl::Update() {
    Glib::ustring value;
    Render(value);
    set_text(value);
  }

  void TextEntryControl::on_changed() {
    parent->controlChanged(this);
  }

  void TextEntryControl::on_activate() {
    parent->ContainerActivated();
  }

  Gtk::Widget *TextEntryControl::widget() {
    return this;
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
    std::stringstream buffer;
    buffer << *value;
    s.assign(buffer.str());
  }

  // Real entry widget ------------------------------------------------------

  bool RealControl::Valid(const char *s) const {
    char *end = NULL;                   // TODO
    errno = 0;
    arith_t n;
    int error = arith_traits<arith_t>::fromString(n, s, &end);
    if(error || n < min || n > max || *end)
      return false;
    else
      return true;
  }

  void RealControl::Set(const char *s) {
    arith_traits<arith_t>::fromString(*value, s, NULL);
  }

  void RealControl::Render(Glib::ustring &s) const {
    s.assign(arith_traits<arith_t>::toString(*value));
  }

  // String entry widget

  bool StringControl::Valid(const char *) const {
    return true;
  }

  void StringControl::Set(const char *s) {
    value->assign(s);
  }

  void StringControl::Render(Glib::ustring &s) const {
    s = *value;
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
