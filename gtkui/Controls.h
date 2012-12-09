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
#ifndef CONTROLS_H
#define CONTROLS_H

#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>

namespace mmui {

  class ControlContainer;
  class View;

  // Text edit/display entry in a control container
  class Control: public Gtk::Entry {
    ControlContainer *parent;

    // Caption
    Gtk::Label label;

    // Called when underlying value is modified; uses Render() to extract the
    // new value and updates the Gtk::Entry.
    void Update();
  public:
    Control(ControlContainer *p,
            bool editable_);

    // Called when user presses RETURN.  Calls the parent's
    // ContainerActivated() method.
    void on_activate();

    // Called when the contents changes.  Calls the parent's ControlChanged()
    // method.
    void on_changed();

    // Verify whether a proposed value is good.
    // Return true if S is valid, else false
    virtual bool Valid(const char *s) const = 0;

    // Set underlying value control from string S (which will be valid)
    virtual void Set(const char *s) = 0;

    // Render the underlying value to string V
    virtual void Render(Glib::ustring &v) const = 0;

    // Attach to parent container, with a caption
    void Attach(int x, int y, const char *caption, int width = 1);

    friend class ControlContainer;
  };

  // Control which displays/accepts an integer within some range
  class IntegerControl: public Control {
    int *value;
    int min, max;
  public:
    IntegerControl(ControlContainer *p, int *v, int min_, int max_,
                   bool editable_ = true):
      Control(p, editable_),
      value(v), min(min_), max(max_) {
    }
    bool Valid(const char *s) const;
    void Set(const char *s);
    void Render(Glib::ustring &) const;
  };

  // Control which displays/accepts a real within some range.
  class RealControl: public Control {
    arith_t *value;
    arith_t min, max;
  public:
    RealControl(ControlContainer *p, arith_t *v, arith_t min_, arith_t max_,
                bool editable_ = true):
      Control(p, editable_),
      value(v), min(min_), max(max_) {
    }
    bool Valid(const char *s) const;
    void Set(const char *);
    void Render(Glib::ustring &) const;
  };

  // Control which displays/accepts a string.
  class StringControl: public Control {
    std::string *value;
  public:
    StringControl(ControlContainer *p, std::string *v, bool editable_ = true):
      Control(p, editable_), value(v) {
    }
    bool Valid(const char *s) const;
    void Set(const char *);
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
    void Update();

    // Attach a child control to this container.
    void Attach(Control *c) {
      controls.push_back(c);
    }

    // Return true if all displayed values are valid
    bool allValid() const;

    // Called when a child control's displayed value changes.  The underlying
    // value will not have been set and the displayed value may be invalid!
    virtual void controlChanged(Control *);

    // Set the input-sensitivity of all controls.
    void sensitive(bool sensitivity);
  };

}

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
