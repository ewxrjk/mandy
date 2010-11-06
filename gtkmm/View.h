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

#ifndef VIEW_H
#define VIEW_H

namespace mmui {

  class ControlPanel;

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

    void Save();

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

}

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
