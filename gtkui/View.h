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

#ifndef VIEW_H
#define VIEW_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#include <gtkmm/drawingarea.h>
#pragma GCC diagnostic pop
#include "arith.h"

class IterBuffer;
class FractalJobFactory;
class Job;

namespace mmui {

class ControlPanel;

class View: public Gtk::DrawingArea {
public:
  View();
  virtual bool on_button_press_event(GdkEventButton *);
  virtual bool on_button_release_event(GdkEventButton *);
  virtual bool on_motion_notify_event(GdkEventMotion *);
  virtual bool on_expose_event(GdkEventExpose *);

  void NewPointer(int xpos, int ypos);
  void NewLocation(int xpos = -1, int ypos = -1);
  void NewSize();
  void Drag(int deltax, int deltay);
  void Zoom(arith_t x, arith_t y, arith_t scale);
  inline void SetControlPanel(ControlPanel *p) {
    controls = p;
  }
  inline void SetJobFactory(FractalJobFactory *jf) {
    jobFactory = jf;
  }

  void GetCoordinates(arith_t &x, arith_t &y, int xpos, int tpos);

  void Save();

  // Parameters
  arith_t xcenter = 0, ycenter = 0, radius = 2;
  int maxiters = 255;
  arith_type arith = ARITH_DEFAULT;
  std::string arith_string = arith_names[ARITH_DEFAULT];

  // Results
  arith_t xpointer = 0, ypointer = 0, count = 0;
  std::string elapsed;

private:
  // Iteration count and pixel data
  IterBuffer *dest = nullptr;
  Glib::RefPtr<Gdk::Pixbuf> pixbuf;

  void Redraw(int x, int y, int w, int h);
  void NewPixels(int x, int y, int w, int h);
  void NewPixels();
  static void Completed(Job *generic_job, void *completion_data);
  struct timespec started;

  // Dragging support
  bool dragging = false;
  double dragFromX = 0, dragFromY = 0;
  double dragToX = 0, dragToY = 0;
  sigc::connection dragIdleConnection;

  void DragComplete();
  bool DragIdle();

  // Control panel interface
  ControlPanel *controls = nullptr;

  const FractalJobFactory *jobFactory = nullptr;
};

} // namespace mmui

#endif /* VIEW_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
