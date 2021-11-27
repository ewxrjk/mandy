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
#ifndef CONTROLS_H
#define CONTROLS_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/comboboxtext.h>
#pragma GCC diagnostic pop

namespace mmui {

class ControlContainer;
class View;

// A labelled item in a control container
//
// Every item has two associated mutable values:
//   - The underlying value.  This is an object somewhere in the program.
//   - The display value.  This is what the widget currently displays.
//
// The base class has no idea what types either of these values have (they
// don't have to be the same).
class Control {
  // Caption
  Gtk::Label label;

public:
  inline Control(ControlContainer *p): parent(p) {}

  // Attach to parent container, with a caption
  void Attach(int x, int y, const char *caption, int width = 1);

  // Return true if the current setting is valid
  // Default is to return true, i.e. all settings are valid
  virtual bool DisplayIsValid() const;

  // Update the display value
  // Default is to do nothing(!)
  virtual void UpdateDisplay();

  // Update the underlying value
  // Default is to do nothing(!)
  virtual void UpdateUnderlying();

  // Return the item widget
  //
  // (The implementations multiply inherit from the item widget to make it
  // easy to supply virtual method overrides, but the base doesn't know about
  // this.)
  virtual Gtk::Widget *widget() = 0;

protected:
  ControlContainer *parent;
};

// Drop-down in a control container
class DropDownControl: public Control, public Gtk::ComboBoxText {
  Gtk::Widget *widget();

  std::string *m_value;

public:
  template <typename T>
  DropDownControl(ControlContainer *p, std::string *value, const T &s,
                  const T &e):
      Control(p),
      m_value(value) {
    for(T it = s; it != e; ++it)
      append_text(*it);
    set_active_text(*value);
  }

  DropDownControl(ControlContainer *p, std::string *value):
      Control(p), m_value(value) {}

  // Update the display value
  void UpdateDisplay();

  // Update the underlying value
  void UpdateUnderlying();

  // Called when the contents changes.  Calls the parent's ControlChanged()
  // method.
  void on_changed();

  // Update contents
  template <typename T> void UpdateChoices(const T &s, const T &e) {
    std::string setting = get_active_text();
    clear();
    for(T it = s; it != e; ++it)
      append_text(*it);
    set_active_text(setting);
  }
};

// Filename selector in a control container
class FileSelectionControl: public Control, public Gtk::FileChooserButton {
  Gtk::Widget *widget();

  std::string *m_path;

public:
  FileSelectionControl(
      ControlContainer *p, std::string *path, const Glib::ustring &title,
      Gtk::FileChooserAction action = Gtk::FILE_CHOOSER_ACTION_OPEN);

  // Update the display value
  void UpdateDisplay();

  // Update the underlying value
  void UpdateUnderlying();

  void on_file_set();
};

// Text edit/display entry in a control container
class TextEntryControl: public Control, public Gtk::Entry {
  Gtk::Widget *widget();

public:
  TextEntryControl(ControlContainer *p, bool editable_);

  // Overrides

  // Called when user presses RETURN.  Calls the parent's
  // ContainerActivated() method.
  void on_activate();

  // Called when the contents changes.  Calls the parent's ControlChanged()
  // method.
  void on_changed();

  // Return true if current setting is valid
  bool DisplayIsValid() const;

  // Update the display value.  Uses Render() to extract the new value and
  // updates the Gtk::Entry.
  void UpdateDisplay();

  // Update the underlying value.
  virtual void UpdateUnderlying();

  // Methods

  // Verify whether a proposed display value is good.
  // Return true if S is valid, else false
  virtual bool DisplayIsValid(const char *s) const = 0;

  // Set underlying value control from string S (which will be valid)
  virtual void SetUnderlying(const char *s) = 0;

  // Render the underlying value to string V in display format
  virtual void Render(Glib::ustring &v) const = 0;
};

// Control which displays/accepts an integer within some range
class IntegerControl: public TextEntryControl {
  int *value;
  int min, max;

public:
  IntegerControl(ControlContainer *p, int *v, int min_, int max_,
                 bool editable_ = true):
      TextEntryControl(p, editable_),
      value(v), min(min_), max(max_) {}
  bool DisplayIsValid(const char *s) const;
  void SetUnderlying(const char *s);
  void Render(Glib::ustring &) const;
};

// Control which displays/accepts a real within some range.
class RealControl: public TextEntryControl {
  arith_t *value;
  arith_t min, max;

public:
  RealControl(ControlContainer *p, arith_t *v, arith_t min_, arith_t max_,
              bool editable_ = true):
      TextEntryControl(p, editable_),
      value(v), min(min_), max(max_) {}
  bool DisplayIsValid(const char *s) const;
  void SetUnderlying(const char *);
  void Render(Glib::ustring &) const;
};

// Control which displays/accepts a string.
class StringControl: public TextEntryControl {
  std::string *value;

public:
  StringControl(ControlContainer *p, std::string *v, bool editable_ = true):
      TextEntryControl(p, editable_), value(v) {}
  bool DisplayIsValid(const char *s) const;
  void SetUnderlying(const char *);
  void Render(Glib::ustring &) const;
};

// Container of controls
class ControlContainer: public Gtk::Table {
public:
  // Collection of child controls
  std::vector<Control *> controls;

  // Called when the user presses ENTER.  The underlying values will have
  // been set to their displayed values.  The default does nothing.
  virtual void Activated();

  // Called when the user presses ENTER.  If all displayed values are valid
  // the sets the underlying values and calls Activated().  Otherwise beeps
  // and returns.
  void ContainerActivated();

  // Called when at least one underlying value has been changed.  Calls the
  // UPdate methods on the child controls.
  void UpdateDisplay();

  // Attach a child control to this container.
  void Attach(TextEntryControl *c) {
    controls.push_back(c);
  }

  // Return true if all displayed values are valid
  bool allDisplaysValid() const;

  // Called when a child control's displayed value changes.  The underlying
  // value will not have been set and the displayed value may be invalid!
  virtual void controlChanged(Control *);

  // Set the input-sensitivity of all controls.
  void SetSensitivity(bool sensitivity);
};

} // namespace mmui

#endif /* CONTROLS_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
