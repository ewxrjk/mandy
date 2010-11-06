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
#include "JuliaWindow.h"
#include "MandelbrotWindow.h"
#include <gtkmm/main.h>

mmui::MandelbrotWindow *mmui::mandelbrot;
mmui::JuliaWindow *mmui::julia;

static sigc::connection pollAgainConnection;

static bool pollAgainHandler() {
  bool more = Job::poll(1);
  return more;
}

static bool periodic() {
  bool more = Job::poll(1);
  if(more && !pollAgainConnection.connected())
    pollAgainConnection = Glib::signal_idle().connect
      (sigc::ptr_fun(pollAgainHandler));
  return true;
}

int main(int argc, char **argv) {
  Gtk::Main kit(argc, argv);

  Job::init();
  Glib::signal_timeout().connect
    (sigc::ptr_fun(periodic), 10);

  mmui::mandelbrot = new mmui::MandelbrotWindow();
  mmui::julia = new mmui::JuliaWindow();
  mmui::mandelbrot->view.NewSize();
  mmui::mandelbrot->view.SetJuliaView(&mmui::julia->view);
  mmui::julia->view.NewSize();

  Gtk::Main::run(*mmui::mandelbrot);
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
