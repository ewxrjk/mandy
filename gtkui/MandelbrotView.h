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

#ifndef MANDELBROTVIEW_H
#define MANDELBROTVIEW_H

#include "View.h"
#include "MandelbrotJob.h"

namespace mmui {

class MandelbrotView: public View {
public:
  MandelbrotView();
  MandelbrotJobFactory mandelbrotJobFactory;

  JuliaView *juliaView;
  JuliaWindow *juliaWindow;

  inline void SetJulia(JuliaView *v, JuliaWindow *w) {
    juliaView = v;
    juliaWindow = w;
  }
  void NewJulia(arith_t x, arith_t y);
  virtual bool on_button_press_event(GdkEventButton *);
  virtual bool on_button_release_event(GdkEventButton *);
  virtual bool on_motion_notify_event(GdkEventMotion *);

  void Movie();
};

} // namespace mmui

#endif /* MANDELBROTVIEW_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
