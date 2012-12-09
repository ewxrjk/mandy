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

  class Caption: public Gtk::Label {
  public:
    Caption(const char *caption);
  };

  class Control: public Gtk::Entry {
    ControlContainer *parent;
    void Update();
  public:
    Control(ControlContainer *p,
            bool editable_);

    void on_activate();
    void on_changed();

    // Return true if S is valid, else false
    virtual bool Valid(const char *s) const = 0;
    // Set underlying value control from string S (which will be valid)
    virtual void Set(const char *s) = 0;
    // Render the underlying value to string V
    virtual void Render(Glib::ustring &v) const = 0;

    friend class ControlContainer;
  };

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

  class ControlContainer: public Gtk::Table {
  public:
    std::vector<Control *> controls;
    virtual void Activated();
    void ContainerActivated();
    void Update();

    void Attach(Control *c) {
      controls.push_back(c);
    }

    bool allValid() const;
    virtual void controlChanged(Control *);

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
