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
#ifndef GTKUI_H
#define GTKUI_H

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkkeysyms.h>

class IterBuffer;
class Job;

namespace Gtkui {

  // The results of the most recent computation
  extern IterBuffer *LatestDest;
  extern GdkPixbuf *LatestPixbuf;

  // Where and how to draw the results
  extern GtkWidget *DrawingArea;
  extern GdkDrawable *Drawable;
  extern GdkGC *GC;
  extern GtkWidget *Toplevel;

  // Drawing functions
  void Redraw(int x, int y, int w, int h);
  void Completed(Job *generic_job);
  void NewLocation(int xpos, int ypos);
  void NewSize();

  // Control panel
  GtkWidget *ControlPanel();
  void Changed();

  // Pointer handling
  gboolean PointerMoved(GtkWidget *, GdkEventMotion *, gpointer);
  gboolean ButtonPressed(GtkWidget *, GdkEventButton *, gpointer);

  // Keyboard handling
  gboolean Keypress(GtkWidget *, GdkEventKey *, gpointer);

  // Miscellaneous callbacks
  gboolean PeriodicPoll(gpointer);
  gboolean Exposed(GtkWidget *, GdkEventExpose *, gpointer);
  gboolean ToplevelDeleted(GtkWidget *, GdkEvent *, gpointer);

  // Setup
  void Setup();
  void Run();
  
};

#endif /* GTKUI_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/