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
#include "arith.h"

namespace mmui {

  JuliaWindow::JuliaWindow() {
    Initialize(&view);
    set_title("Julia Set");
  }

  bool JuliaWindow::close() {
    hide();
    return true;
  }

  JuliaView::JuliaView() {
    SetJobFactory(&juliaJobFactory);
  }

  void JuliaView::Update(arith_t x, arith_t y) {
    if(juliaJobFactory.cx != x || juliaJobFactory.cy != y) {
      juliaJobFactory.cx = x;
      juliaJobFactory.cy = y;
      NewLocation();
      std::string xs = arith_traits<arith_t>::toString(x);
      std::string ys = arith_traits<arith_t>::toString(x);
      std::stringstream buffer;
      buffer << "Julia set at " << xs << "+" << ys << "i";
      dynamic_cast<Gtk::Window *>(get_parent()->get_parent())->set_title(buffer.str());
    }
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
