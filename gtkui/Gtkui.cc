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
#include "mandy.h"
#include "Gtkui.h"
#include "Job.h"

namespace Gtkui {

  /* Timeout to handle delayed recompitation */
  gboolean PeriodicPoll(gpointer) {
    // See if anything's happened lately.
    // TODO we should arrange that the timeout is suppressed if nothing
    // is going on.
    Job::poll();
    return TRUE;
  }

  /* expose-event callback */
  gboolean Exposed(GtkWidget *, GdkEventExpose *, gpointer) {
    gint w, h;
    gdk_drawable_get_size(Drawable, &w, &h);
    if(w != gdk_pixbuf_get_width(LatestPixbuf)
       || h != gdk_pixbuf_get_height(LatestPixbuf)) {
      // The pixbuf is the wrong size (i.e. the window has been
      // resized).  Attempt a recompute.
      NewSize();
    } else {
      // Just draw what we've got
      // TODO only redraw the bit that was exposed
      Redraw(0, 0, w, h);
    }
    return TRUE;
  }

  /* delete-event callback */
  gboolean ToplevelDeleted(GtkWidget *, GdkEvent *, gpointer) {
    Job::destroy();
    exit(0);
  }

  // The results of the most recent computation
  IterBuffer *LatestDest;
  GdkPixbuf *LatestPixbuf;

  // Where and how to draw the results
  GtkWidget *DrawingArea;
  GdkDrawable *Drawable;
  GdkGC *GC;
  GtkWidget *Toplevel;

  void Setup() {
    // The top level window
    Toplevel = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title((GtkWindow *)Toplevel, "mand");
    gtk_widget_add_events(Toplevel,
			  GDK_KEY_RELEASE_MASK);
    g_signal_connect(G_OBJECT(Toplevel), "delete-event",
		     G_CALLBACK(ToplevelDeleted), NULL);
    g_signal_connect(G_OBJECT(Toplevel), "key-release-event",
		     G_CALLBACK(Keypress), NULL);

    // A drawing area for the results
    DrawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(DrawingArea, 384, 384);
    g_signal_connect(DrawingArea, "expose-event", G_CALLBACK(Exposed), NULL);
    gtk_widget_add_events(DrawingArea,
			  GDK_BUTTON_PRESS_MASK
			  |GDK_BUTTON_RELEASE_MASK
			  |GDK_POINTER_MOTION_MASK);
    g_signal_connect(DrawingArea, "button-press-event", G_CALLBACK(ButtonPressed), NULL);
    g_signal_connect(DrawingArea, "button-release-event", G_CALLBACK(ButtonPressed), NULL);
    g_signal_connect(DrawingArea, "motion-notify-event", G_CALLBACK(PointerMoved), NULL);

    // Timeout to pick up the results of delayed recomputation
    g_timeout_add(10, PeriodicPoll, NULL);

    // Pack it together vertically
    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, ControlPanel(), FALSE, FALSE, 1);
    gtk_box_pack_end((GtkBox *)vbox, DrawingArea, TRUE, TRUE, 0);
    gtk_container_add((GtkContainer *)Toplevel, vbox);
    gtk_widget_show_all(Toplevel);

    // We only know these after the first _show_all call
    Drawable = DrawingArea->window;
    GC = DrawingArea->style->fg_gc[DrawingArea->state];

    // Start an initial computation.
    NewSize();
    Changed();
  }

  void Run() {
    // Run the main loop
    GMainLoop *mainloop = g_main_loop_new(0, 0);
    g_main_loop_run(mainloop);
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
