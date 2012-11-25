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
#ifndef CONTROLS_H
#define CONTROLS_H

#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>

namespace mmui {

  class ControlPanel;
  class View;

  class Caption: public Gtk::Label {
  public:
    Caption(const char *caption);
  };

  class Control: public Gtk::Entry {
    ControlPanel *parent;
  public:
    Control(ControlPanel *p,
            bool editable_);

    void on_activate();

    virtual bool Valid(const char *s) const = 0;
    virtual void Set(const char *s) = 0;
    virtual void Render(Glib::ustring &) const = 0;
  };

  class IntegerControl: public Control {
    int *value;
    int min, max;
  public:
    IntegerControl(ControlPanel *p, int *v, int min_, int max_,
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
    RealControl(ControlPanel *p, arith_t *v, arith_t min_, arith_t max_,
                bool editable_ = true):
      Control(p, editable_),
      value(v), min(min_), max(max_) {
    }
    bool Valid(const char *s) const;
    void Set(const char *);
    void Render(Glib::ustring &) const;
  };

  class ControlPanel: public Gtk::Table {
    std::vector<Control *> controls;
    View *view;
    Caption xcenter_caption, ycenter_caption, radius_caption, maxiters_caption;
    Caption xpointer_caption, ypointer_caption, count_caption;
    RealControl xcenter_control, ycenter_control, radius_control;
    IntegerControl maxiters_control;
    RealControl xpointer_control, ypointer_control, count_control;
  public:
    ControlPanel(View *);

    void Activated();                   // someone hit ENTER
    void Update();                      // underlying values changed

    void Attach(Control *c) {
      controls.push_back(c);
    }
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
