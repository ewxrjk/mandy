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

#ifndef MMUI_H
#define MMUI_H

#include "mandy.h"
#include "IterBuffer.h"
#include "Job.h"
#include "MandelbrotJob.h"
#include "JuliaJob.h"

#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>
#include <gtkmm/frame.h>

#include "Controls.h"

namespace mmui {

  class Toplevel;
  class ControlPanel;
  class JuliaWindow;
  class JuliaView;

  class View: public Gtk::DrawingArea {
  public:
    View();
    virtual bool on_button_press_event(GdkEventButton *);
    virtual bool on_button_release_event(GdkEventButton *);
    virtual bool on_motion_notify_event(GdkEventMotion *);
    virtual bool on_expose_event(GdkEventExpose *);

    void NewLocation(int xpos = -1, int ypos = -1);
    void NewSize();
    void Drag(int deltax, int deltay);
    void Zoom(double x, double y, double scale);
    inline void SetControlPanel(ControlPanel *p) { controls = p; }
    inline void SetJobFactory(FractalJobFactory *jf) { jobFactory = jf; }

    // Parameters
    double xcenter, ycenter, radius;
    int maxiters;

  private:
    // Colors
    struct color { unsigned char r, g, b; };
    std::vector<color> colors;
    void NewColors();                   // recompute colors array

    // Iteration count and pixel data
    IterBuffer *dest;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;

    void Redraw(int x, int y, int w, int h);
    static void Completed(Job *generic_job, void *completion_data);

    // Dragging support
    bool dragging;
    double dragFromX, dragFromY;
    double dragToX, dragToY;
    sigc::connection dragIdleConnection;

    void DragComplete();
    bool DragIdle();

    // Control panel interface
    ControlPanel *controls;

    const FractalJobFactory *jobFactory;
  };

  class MandelbrotView: public View {
  public:
    MandelbrotView();
    MandelbrotJobFactory mandelbrotJobFactory;

    JuliaView *juliaView;

    inline void SetJuliaView(JuliaView *v) { juliaView = v; }
    void NewJulia(double x, double y);
    virtual bool on_button_press_event(GdkEventButton *);
    virtual bool on_button_release_event(GdkEventButton *);
    virtual bool on_motion_notify_event(GdkEventMotion *);
  };

  class JuliaView: public View {
  public:
    JuliaView();
    JuliaJobFactory juliaFactory;

    void Update(double x, double y) {
      if(juliaFactory.cx != x || juliaFactory.cy != y) {
        juliaFactory.cx = x;
        juliaFactory.cy = y;
        NewLocation();
      }
    }
  };

  class Toplevel: public Gtk::Window {
  public:
    Toplevel();
    bool on_delete_event(GdkEventAny *);
    bool on_key_release_event(GdkEventKey *);

    // Sub-widgets
    MandelbrotView view;
    ControlPanel controls;
    Gtk::Frame frame;
    Gtk::VBox vbox;
  };

  class JuliaWindow: public Gtk::Window {
  public:
    JuliaWindow();
    JuliaView view;
    ControlPanel controls;
    Gtk::Frame frame;
    Gtk::VBox vbox;
  };

}

#endif /* MMUI_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
