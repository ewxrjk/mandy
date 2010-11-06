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

#ifndef GENERICWINDOW_H
#define GENERICWINDOW_H

#include "View.h"
#include <gtkmm/window.h>

namespace mmui {

  class GenericWindow: public Gtk::Window {
  public:
    GenericWindow();
    void Initialize(View *view_);
    virtual bool on_key_release_event(GdkEventKey *);
    virtual bool on_delete_event(GdkEventAny *);

    virtual void close() = 0;
    
    // Sub-widgets
    View *view;
    ControlPanel *controls;
  };

}

#endif /* GENERICWINDOW_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
