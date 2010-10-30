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
#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <cerrno>
#include <cmath>
#include <cassert>
#include <cstring>

static gboolean gtkuiToplevelDeleted(GtkWidget *, GdkEvent *, gpointer);

// Pointer motion -------------------------------------------------------------

/* Drag state */
static gboolean gtkuiDragging;
static double gtkuiDragFromX, gtkuiDragFromY;
static double gtkuiDragToX, gtkuiDragToY;

/* Drag from gtkuiDragFrom[xy] to a new pointer location */
static void gtkuiDragComplete() {
  const int deltax = gtkuiDragToX - gtkuiDragFromX;
  const int deltay = gtkuiDragToY - gtkuiDragFromY;
  if(!(deltax == 0 && deltay == 0)) {
    gtkuiDragFromX = gtkuiDragToX;
    gtkuiDragFromY = gtkuiDragToY;
    gint w, h;
    gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
    drag(w, h, deltax, deltay);
    Gtkui::Changed();
    Gtkui::NewLocation(gtkuiDragToX, gtkuiDragToY);
  }
}

static guint gtkuiDragIdleHandle;

static gboolean gtkuiDragIdle(gpointer) {
  gtkuiDragComplete();
  gtkuiDragIdleHandle = 0;
  return FALSE;
}

/* motion-notify-event callback */
static gboolean gtkuiPointerMoved(GtkWidget *,
                                  GdkEventMotion *event,
                                  gpointer) {
  if(!gtkuiDragging)
    return FALSE;
  gtkuiDragToX = event->x;
  gtkuiDragToY = event->y;
  if(gtkuiDragIdleHandle == 0)
    gtkuiDragIdleHandle = g_idle_add(gtkuiDragIdle, NULL);
  return TRUE;
}

/* button-{press,release}-event callback */
static gboolean gtkuiButtonPressed(GtkWidget *, GdkEventButton *event, gpointer) {
  // Double-click left button zooms in
  if(event->type == GDK_2BUTTON_PRESS
     && event->button == 1
     && event->state == 0) {
    gint w, h;
    gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
    zoom(w, h, event->x, event->y, M_SQRT1_2);
    Gtkui::Changed();
    Gtkui::NewLocation(-1, -1);
    return TRUE;
  }
  // Double-click right button zooms out
  if(event->type == GDK_2BUTTON_PRESS
     && event->button == 3
     && event->state == 0) {
    gint w, h;
    gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
    zoom(w, h, event->x, event->y, M_SQRT2);
    Gtkui::Changed();
    Gtkui::NewLocation(-1, -1);
    return TRUE;
  }
  // Hold left button drags
  if(event->type == GDK_BUTTON_PRESS
     && event->button == 1
     && event->state == 0) {
    gtkuiDragging = TRUE;
    gtkuiDragFromX = event->x;
    gtkuiDragFromY = event->y;
    return TRUE;
  }
  if(event->type == GDK_BUTTON_RELEASE
     && event->button == 1) {
    gtkuiDragToX = event->x;
    gtkuiDragToY = event->y;
    gtkuiDragComplete();
    gtkuiDragging = FALSE;
    return TRUE;
  }
  return FALSE;
}

// Keyboard -------------------------------------------------------------------

/* key-release-event callback */
static gboolean gtkuiKeypress(GtkWidget *,
                              GdkEventKey *event,
                              gpointer) {
  if(event->state == GDK_CONTROL_MASK) {
    switch(event->keyval) {
    case 'w': case 'W':
      gtkuiToplevelDeleted(NULL, NULL, NULL);
    case GDK_equal: case GDK_minus: case GDK_KP_Add: case GDK_KP_Subtract: {
      gint w, h;
      gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
      if(event->keyval == GDK_equal || event->keyval == GDK_KP_Add)
        size *= M_SQRT1_2;
      else
        size *= M_SQRT2;
      Gtkui::Changed();
      Gtkui::NewLocation(w/2, h/2);
      return TRUE;
    }
    }
  }
  return FALSE;
}

// Miscelleneous callbacks ----------------------------------------------------

/* Timeout to handle delayed recompitation */
static gboolean gtkuiPeriodicPoll(gpointer) {
  // See if anything's happened lately.
  // TODO we should arrange that the timeout is suppressed if nothing
  // is going on.
  Job::poll();
  return TRUE;
}

/* expose-event callback */
static gboolean gtkuiExposed(GtkWidget *, GdkEventExpose *, gpointer) {
  gint w, h;
  gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
  if(w != gdk_pixbuf_get_width(Gtkui::LatestPixbuf)
     || h != gdk_pixbuf_get_height(Gtkui::LatestPixbuf)) {
    // The pixbuf is the wrong size (i.e. the window has been
    // resized).  Attempt a recompute.
    Gtkui::NewSize();
  } else {
    // Just draw what we've got
    // TODO only redraw the bit that was exposed
    Gtkui::Redraw(0, 0, w, h);
  }
  return TRUE;
}

/* delete-event callback */
static gboolean gtkuiToplevelDeleted(GtkWidget *, GdkEvent *, gpointer) {
  Job::destroy();
  exit(0);
}

int main(int argc, char **argv) {
  if(!setlocale(LC_CTYPE, ""))
    fatal(errno, "error calling setlocale");
  if(!gtk_init_check(&argc, &argv))
    fatal(0, "gtk_init_check failed");

  // Bits of infrastructure
  init_colors();
  Job::init();

  // The top level window
  Gtkui::Toplevel = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title((GtkWindow *)Gtkui::Toplevel, "mand");
  gtk_widget_add_events(Gtkui::Toplevel,
			GDK_KEY_RELEASE_MASK);
  g_signal_connect(G_OBJECT(Gtkui::Toplevel), "delete-event",
                   G_CALLBACK(gtkuiToplevelDeleted), NULL);
  g_signal_connect(G_OBJECT(Gtkui::Toplevel), "key-release-event",
                   G_CALLBACK(gtkuiKeypress), NULL);

  // A drawing area for the results
  Gtkui::DrawingArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(Gtkui::DrawingArea, 384, 384);
  g_signal_connect(Gtkui::DrawingArea, "expose-event", G_CALLBACK(gtkuiExposed), NULL);
  gtk_widget_add_events(Gtkui::DrawingArea,
			GDK_BUTTON_PRESS_MASK
			|GDK_BUTTON_RELEASE_MASK
			|GDK_POINTER_MOTION_MASK);
  g_signal_connect(Gtkui::DrawingArea, "button-press-event", G_CALLBACK(gtkuiButtonPressed), NULL);
  g_signal_connect(Gtkui::DrawingArea, "button-release-event", G_CALLBACK(gtkuiButtonPressed), NULL);
  g_signal_connect(Gtkui::DrawingArea, "motion-notify-event", G_CALLBACK(gtkuiPointerMoved), NULL);

  // Timeout to pick up the results of delayed recomputation
  g_timeout_add(10, gtkuiPeriodicPoll, NULL);

  // Pack it together vertically
  GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start((GtkBox *)vbox, Gtkui::ControlPanel(), FALSE, FALSE, 1);
  gtk_box_pack_end((GtkBox *)vbox, Gtkui::DrawingArea, TRUE, TRUE, 0);
  gtk_container_add((GtkContainer *)Gtkui::Toplevel, vbox);
  gtk_widget_show_all(Gtkui::Toplevel);

  // We only know these after the first _show_all call
  Gtkui::Drawable = Gtkui::DrawingArea->window;
  Gtkui::GC = Gtkui::DrawingArea->style->fg_gc[Gtkui::DrawingArea->state];

  // Start an initial computation.
  Gtkui::NewSize();
  Gtkui::Changed();

  // Run the main loop
  GMainLoop *mainloop = g_main_loop_new(0, 0);
  g_main_loop_run(mainloop);
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
