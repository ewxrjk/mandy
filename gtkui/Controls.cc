/* Copyright © Richard Kettlewell.
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

// Generic control container ------------------------------------------------

void ControlContainer::UpdateDisplay() {
  Glib::ustring value;
  for(size_t n = 0; n < controls.size(); ++n)
    controls[n]->UpdateDisplay();
}

void ControlContainer::ContainerActivated() {
  for(size_t n = 0; n < controls.size(); ++n) {
    Control *c = controls[n];
    if(!c->DisplayIsValid()) {
      c->widget()->grab_focus();
      get_window()->beep();
      return;
    }
  }
  for(size_t n = 0; n < controls.size(); ++n)
    controls[n]->UpdateUnderlying();
  Activated();
}

bool ControlContainer::allDisplaysValid() const {
  for(size_t n = 0; n < controls.size(); ++n)
    if(!controls[n]->DisplayIsValid())
      return false;
  return true;
}

void ControlContainer::controlChanged(Control *) {}

void ControlContainer::Activated() {}

void ControlContainer::SetSensitivity(bool sensitivity) {
  for(size_t n = 0; n < controls.size(); ++n)
    controls[n]->widget()->set_sensitive(sensitivity);
}

// Base for controls --------------------------------------------------------

void Control::Attach(int x, int y, const char *caption, int width) {
  label.set_text(caption);
  label.set_alignment(1.0, 0.0);
  parent->attach(label, 2 * x, 2 * x + 1, y, y + 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
  parent->attach(*widget(), 2 * x + 1, 2 * (x + width), y, y + 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
}

bool Control::DisplayIsValid() const {
  return true;
}

void Control::UpdateDisplay() {}

void Control::UpdateUnderlying() {}

// Drop-down control --------------------------------------------------------

void DropDownControl::UpdateDisplay() {
  set_active_text(*m_value);
}

void DropDownControl::UpdateUnderlying() {
  *m_value = get_active_text();
}

void DropDownControl::on_changed() {
  parent->controlChanged(this);
}

Gtk::Widget *DropDownControl::widget() {
  return this;
}

// File selection widget ----------------------------------------------------

FileSelectionControl::FileSelectionControl(ControlContainer *p,
                                           std::string *path,
                                           const Glib::ustring &title,
                                           Gtk::FileChooserAction action):
    Control(p), Gtk::FileChooserButton(title, action), m_path(path) {
  select_filename(*m_path);
  signal_file_set().connect(sigc::mem_fun(*this, &FileSelectionControl::on_file_set));
}

void FileSelectionControl::UpdateDisplay() {
  select_filename(*m_path);
}

void FileSelectionControl::UpdateUnderlying() {
  *m_path = get_filename();
}

void FileSelectionControl::on_file_set() {
  parent->controlChanged(this);
}

Gtk::Widget *FileSelectionControl::widget() {
  return this;
}

// Text entry widget --------------------------------------------------------

TextEntryControl::TextEntryControl(ControlContainer *p, bool editable_): Control(p) {
  p->Attach(this);
  set_editable(editable_);
}

void TextEntryControl::UpdateDisplay() {
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

bool TextEntryControl::DisplayIsValid() const {
  if(!get_editable())
    return true;
  return DisplayIsValid(get_text().c_str());
}

void TextEntryControl::UpdateUnderlying() {
  Glib::ustring value;
  Render(value);
  if(value != get_text())
    SetUnderlying(get_text().c_str());
}

// Integer entry widget -----------------------------------------------------

bool IntegerControl::DisplayIsValid(const char *s) const {
  char *end;
  errno = 0;
  long n = strtol(s, &end, 10);
  if(errno || n < min || n > max || *end)
    return false;
  else
    return true;
}

void IntegerControl::SetUnderlying(const char *s) {
  *value = strtol(s, NULL, 10);
}

void IntegerControl::Render(Glib::ustring &s) const {
  std::stringstream buffer;
  buffer << *value;
  s.assign(buffer.str());
}

// Real entry widget ------------------------------------------------------

bool RealControl::DisplayIsValid(const char *s) const {
  char *end = NULL; // TODO
  errno = 0;
  arith_t n;
  int error = arith_traits<arith_t>::fromString(n, s, &end);
  if(error || n < min || n > max || *end) {
    return false;
  } else
    return true;
}

void RealControl::SetUnderlying(const char *s) {
  arith_traits<arith_t>::fromString(*value, s, NULL);
}

void RealControl::Render(Glib::ustring &s) const {
  s.assign(arith_traits<arith_t>::toString(*value));
}

// String entry widget

bool StringControl::DisplayIsValid(const char *) const {
  return true;
}

void StringControl::SetUnderlying(const char *s) {
  value->assign(s);
}

void StringControl::Render(Glib::ustring &s) const {
  s = *value;
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
