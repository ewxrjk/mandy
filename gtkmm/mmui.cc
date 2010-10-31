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

static bool periodic() {
  Job::poll();
  return true;
}

int main(int argc, char **argv) {
  Gtk::Main kit(argc, argv);

  init_colors();
  Job::init();
  Glib::signal_timeout().connect
    (sigc::ptr_fun(periodic), 10);

  mmui::Toplevel toplevel;
  toplevel.view.NewSize();
  //Changed(); TODO

  Gtk::Main::run(toplevel);
  return 0;
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