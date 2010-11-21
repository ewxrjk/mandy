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
#include "Draw.h"
#include <gtkmm/main.h>
#include <getopt.h>

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

static const struct option options[] = {
  { "help", no_argument, NULL, 'h' },
  { "threads", required_argument, NULL, 't' },
  { "draw", no_argument, NULL, 'd' },
  { NULL, 0, NULL, 0 }
};

int main(int argc, char **argv) {
  ThreadInit();
  Gtk::Main kit(argc, argv);
  int nthreads = -1, mode = 0;

  int n;
  while((n = getopt_long(argc, argv, "ht:d", options, NULL)) >= 0) {
    switch(n) {
    case 'h':
      printf("Usage:\n"
             "  mandy [OPTIONS]\n"
             "  mandy [OPTIONS] --draw WIDTH HEIGHT X Y RADIUS MAXITERS PATH\n"
             "Options:\n"
             "  --help, -h        Display help message\n"
             "  --draw, -d        Draw one image and terminate\n"
             "  --threads, -t N   Set maximum number of threads\n");
      return 0;
    case 't':
      nthreads = atoi(optarg);
      break;
    case 'd':
      mode = 'd';
      break;
    default:
      exit(1);
    }
  }

  Job::init(nthreads);

  switch(mode) {
  case 'd':
    if(optind + 7 != argc)
      fatal(0, "Usage: %s --draw WIDTH HEIGHT X Y RADIUS MAXITERS PATH", argv[0]);
    draw(argv[optind],
         argv[optind + 1],
         argv[optind + 2],
         argv[optind + 3],
         argv[optind + 4],
         argv[optind + 5],
         argv[optind + 6]);
    return 0;
  default:
    break;
  }
  if(optind != argc)
    fatal(0, "invalid argument '%s'", argv[optind]);

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
