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
#include "MandelbrotJob.h"
#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <cerrno>
#include <cmath>
#include <cassert>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkkeysyms.h>

// The results of the most recent computation
static IterBuffer *latest_dest;
static GdkPixbuf *latest_pixbuf;

// Where and how to draw the results
static GdkDrawable *gtkuiDrawable;
static GdkGC *gtkuiGC;
static GtkWidget *gtkuiToplevel;

// Text entry boxes for parameters
static GtkWidget *xentry, *yentry, *rentry, *ientry;

// Drawing --------------------------------------------------------------------

// Called to just redraw whatever we've got
static void gtkuiRedraw(int x, int y, int w, int h) {
  gdk_draw_pixbuf(gtkuiDrawable,
		  gtkuiGC,
		  latest_pixbuf,
		  x, y, x, y, w, h,
		  GDK_RGB_DITHER_NONE, 0, 0);
}

// Job completion callback
static void gtkuiCompleted(Job *generic_job) {
  MandelbrotJob *j = dynamic_cast<MandelbrotJob *>(generic_job);
  // Ignore stale jobs
  if(j->dest != latest_dest)
    return;
  const int w = latest_dest->w;
  guchar *const pixels = gdk_pixbuf_get_pixels(latest_pixbuf);
  const int rowstride = gdk_pixbuf_get_rowstride(latest_pixbuf);
  const int lx = j->x + j->w;
  const int ly = j->y + j->h;
  for(int y = j->y; y < ly; ++y) {
    int *datarow = &latest_dest->data[y * w + j->x];
    guchar *pixelrow = pixels + y * rowstride + j->x * 3;
    for(int x = j->x; x < lx; ++x) {
      const int count = *datarow++;
      *pixelrow++ = colors[count].r;
      *pixelrow++ = colors[count].g;
      *pixelrow++ = colors[count].b;
    }
  }
  gtkuiRedraw(j->x, j->y, j->w, j->h);
}

// Called to set a new location, scale or maxiter
static void gtkuiNewLocation() {
  if(latest_dest) {
    latest_dest->release();
    latest_dest = NULL;
  }
  gint w, h;
  gdk_drawable_get_size(gtkuiDrawable, &w, &h);
  if(!latest_pixbuf)
    latest_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
  // TODO if there's a pixbuf available then ideally we would move or scale it
  // to provide continuity.
  latest_dest = MandelbrotJob::recompute(xcenter, ycenter, size,
					 maxiter, w, h,
					 gtkuiCompleted);
}

// Called when a resize is detected
static void gtkuiNewSize() {
  // If there's a pixbuf it'll be the wrong size, so delete it.
  // TODO actually what we really wanted was to create the new pixbuf
  // from whatever is lying around in the old one, to provide some
  // continuity.
  if(latest_pixbuf) {
    gdk_pixbuf_unref(latest_pixbuf);
    latest_pixbuf = NULL;
  }
  gtkuiNewLocation();
}

// Control panel --------------------------------------------------------------

static void location_text_activated(GtkEntry *entry, gpointer user_data) {
  double *value = (double *)user_data;
  const char *text = gtk_entry_get_text(entry);
  char *end;
  errno = 0;
  double n = strtod(text, &end);
  // Reject invalid values with a beep.
  if(errno
     || (value == &size && n <= 0.0)
     || *end) {
    gdk_beep();
    return;
  }
  *value = n;
  gtkuiNewLocation();
}

static void maxiter_text_activated(GtkEntry *entry,
                                   gpointer __attribute__((unused)) user_data) {
  const char *text = gtk_entry_get_text(entry);
  char *end;
  errno = 0;
  long n = strtol(text, &end, 10);
  // Reject invalid values with a beep.
  if(errno
     || n <= 0
     || n > INT_MAX
     || *end) {
    gdk_beep();
    return;
  }
  init_colors((int)n);
  // TODO there is an optimization here: if maxiter has gone up then
  // we can skip computation of points with a known non-maximum
  // iteration count.
  gtkuiNewLocation();
}

/* Create the control panel */
static GtkWidget *gtkuiControlPanel(void) {
  GtkWidget *table = gtk_table_new(3, 4, FALSE);
  GtkWidget *xcaption, *ycaption, *rcaption, *icaption;

  gtk_table_attach((GtkTable *)table,
                   (xcaption = gtk_label_new("X centre")),
                   0, 1, 0, 1,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  gtk_misc_set_alignment((GtkMisc *)xcaption, 1.0, 0.0);
  gtk_table_attach((GtkTable *)table,
                   (xentry = gtk_entry_new()),
                   1, 2, 0, 1,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  g_signal_connect(xentry, "activate", G_CALLBACK(location_text_activated),
                   &xcenter);

  gtk_table_attach((GtkTable *)table,
                   (ycaption = gtk_label_new("Y centre")),
                   0, 1, 1, 2,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  gtk_misc_set_alignment((GtkMisc *)ycaption, 1.0, 0.0);
  gtk_table_attach((GtkTable *)table,
                   (yentry = gtk_entry_new()),
                   1, 2, 1, 2,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  g_signal_connect(yentry, "activate", G_CALLBACK(location_text_activated),
                   &ycenter);

  gtk_table_attach((GtkTable *)table,
                   (rcaption = gtk_label_new("Radius")),
                   0, 1, 2, 3,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  gtk_misc_set_alignment((GtkMisc *)rcaption, 1.0, 0.0);
  gtk_table_attach((GtkTable *)table,
                   (rentry = gtk_entry_new()),
                   1, 2, 2, 3,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  g_signal_connect(rentry, "activate", G_CALLBACK(location_text_activated),
                   &size);

  gtk_table_attach((GtkTable *)table,
                   (icaption = gtk_label_new("Iterations")),
                   2, 3, 0, 1,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  gtk_misc_set_alignment((GtkMisc *)icaption, 1.0, 0.0);
  gtk_table_attach((GtkTable *)table,
                   (ientry = gtk_entry_new()),
                   3, 4, 0, 1,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  g_signal_connect(ientry, "activate", G_CALLBACK(maxiter_text_activated),
                   NULL);

  GtkWidget *frame = gtk_frame_new(NULL);
  gtk_container_add((GtkContainer *)frame, table);
  return frame;
}

/* Report current position, size, etc */
static void gtkuiReport(void) {
  char buffer[128];
  snprintf(buffer, sizeof buffer, "%g", xcenter);
  gtk_entry_set_text((GtkEntry *)xentry, buffer);
  snprintf(buffer, sizeof buffer, "%g", ycenter);
  gtk_entry_set_text((GtkEntry *)yentry, buffer);
  snprintf(buffer, sizeof buffer, "%g", size);
  gtk_entry_set_text((GtkEntry *)rentry, buffer);
  snprintf(buffer, sizeof buffer, "%d", maxiter);
  gtk_entry_set_text((GtkEntry *)ientry, buffer);
}

/* expose-event callback */
static gboolean gtkuiExposed(GtkWidget *, GdkEventExpose *, gpointer) {
  gint w, h;
  gdk_drawable_get_size(gtkuiDrawable, &w, &h);
  if(w != gdk_pixbuf_get_width(latest_pixbuf)
     || h != gdk_pixbuf_get_height(latest_pixbuf)) {
    // The pixbuf is the wrong size (i.e. the window has been
    // resized).  Attempt a recompute.
    gtkuiNewSize();
  } else {
    // Just draw what we've got
    // TODO only redraw the bit that was exposed
    gtkuiRedraw(0, 0, w, h);
  }
  return TRUE;
}

/* Drag state */
static gboolean dragging;
static double dragfromx, dragfromy;
static double dragtox, dragtoy;

/* Drag from dragfrom[xy] to a new pointer location */
static void gtkuiDragComplete() {
  const int deltax = dragtox - dragfromx;
  const int deltay = dragtoy - dragfromy;
  if(!(deltax == 0 && deltay == 0)) {
    dragfromx = dragtox;
    dragfromy = dragtoy;
    gint w, h;
    gdk_drawable_get_size(gtkuiDrawable, &w, &h);
    drag(w, h, deltax, deltay);
    gtkuiReport();
    gtkuiNewLocation();
  }
}

static guint gtkuiDragIdleHandle;

static gboolean gtkuiDragIdle(gpointer) {
  gtkuiDragComplete();
  gtkuiDragIdleHandle = 0;
  return FALSE;
}

/* motion-notify-event callback */
static gboolean gtkuiPointerMoved(GtkWidget __attribute__((unused)) *widget,
			      GdkEventMotion *event,
			      gpointer __attribute__((unused)) user_data) {
  if(!dragging)
    return FALSE;
  dragtox = event->x;
  dragtoy = event->y;
  if(gtkuiDragIdleHandle == 0)
    gtkuiDragIdleHandle = g_idle_add(gtkuiDragIdle, NULL);
  return TRUE;
}

/* Timeout to handle delayed recompitation */
static gboolean gtkuiPeriodicPoll(gpointer __attribute__((unused)) data) {
  // See if anything's happened lately.
  // TODO we should arrange that the timeout is suppressed if nothing
  // is going on.
  Job::poll();
  return TRUE;
}

/* button-{press,release}-event callback */
static gboolean gtkuiButtonPressed(GtkWidget *, GdkEventButton *event, gpointer) {
  // Double-click left button zooms in
  if(event->type == GDK_2BUTTON_PRESS
     && event->button == 1
     && event->state == 0) {
    gint w, h;
    gdk_drawable_get_size(gtkuiDrawable, &w, &h);
    zoom(w, h, event->x, event->y, M_SQRT1_2);
    gtkuiReport();
    gtkuiNewLocation();
    return TRUE;
  }
  // Double-click right button zooms out
  if(event->type == GDK_2BUTTON_PRESS
     && event->button == 3
     && event->state == 0) {
    gint w, h;
    gdk_drawable_get_size(gtkuiDrawable, &w, &h);
    zoom(w, h, event->x, event->y, M_SQRT2);
    gtkuiReport();
    gtkuiNewLocation();
    return TRUE;
  }
  // Hold left button drags
  if(event->type == GDK_BUTTON_PRESS
     && event->button == 1
     && event->state == 0) {
    dragging = TRUE;
    dragfromx = event->x;
    dragfromy = event->y;
    return TRUE;
  }
  if(event->type == GDK_BUTTON_RELEASE
     && event->button == 1) {
    dragtox = event->x;
    dragtoy = event->y;
    gtkuiDragComplete();
    dragging = FALSE;
    return TRUE;
  }
  return FALSE;
}

/* delete-event callback */
static gboolean gtkuiToplevelDeleted(GtkWidget *, GdkEvent *, gpointer) {
  Job::destroy();
  exit(0);
}

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
      gdk_drawable_get_size(gtkuiDrawable, &w, &h);
      zoom(w, h, w / 2, h / 2,
           (event->keyval == GDK_equal || event->keyval == GDK_KP_Add)
            ? M_SQRT1_2 : M_SQRT2);
      gtkuiReport();
      gtkuiNewLocation();
      return TRUE;
    }
    }
  }
  return FALSE;
}

int main(int argc, char **argv) {
  if(!setlocale(LC_CTYPE, ""))
    fatal(errno, "error calling setlocale");
  if(!gtk_init_check(&argc, &argv))
    fatal(0, "gtk_init_check failed");

  // Bits of infrastructure
  init_colors(255);
  Job::init();

  // The top level window
  gtkuiToplevel = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title((GtkWindow *)gtkuiToplevel, "mand");
  gtk_widget_add_events(gtkuiToplevel,
			GDK_KEY_RELEASE_MASK);
  g_signal_connect(G_OBJECT(gtkuiToplevel), "delete-event",
                   G_CALLBACK(gtkuiToplevelDeleted), NULL);
  g_signal_connect(G_OBJECT(gtkuiToplevel), "key-release-event",
                   G_CALLBACK(gtkuiKeypress), NULL);

  // A drawing area for the results
  GtkWidget *da = gtk_drawing_area_new();
  gtk_widget_set_size_request(da, 384, 384);
  g_signal_connect(da, "expose-event", G_CALLBACK(gtkuiExposed), NULL);
  gtk_widget_add_events(da,
			GDK_BUTTON_PRESS_MASK
			|GDK_BUTTON_RELEASE_MASK
			|GDK_POINTER_MOTION_MASK);
  g_signal_connect(da, "button-press-event", G_CALLBACK(gtkuiButtonPressed), NULL);
  g_signal_connect(da, "button-release-event", G_CALLBACK(gtkuiButtonPressed), NULL);
  g_signal_connect(da, "motion-notify-event", G_CALLBACK(gtkuiPointerMoved), NULL);

  // Timeout to pick up the results of delayed recomputation
  g_timeout_add(10, gtkuiPeriodicPoll, NULL);

  // Pack it together vertically
  GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start((GtkBox *)vbox, gtkuiControlPanel(), FALSE, FALSE, 1);
  gtk_box_pack_end((GtkBox *)vbox, da, TRUE, TRUE, 0);
  gtk_container_add((GtkContainer *)gtkuiToplevel, vbox);
  gtk_widget_show_all(gtkuiToplevel);

  // We only know these after the first _show_all call
  gtkuiDrawable = da->window;
  gtkuiGC = da->style->fg_gc[da->state];

  // Start an initial computation.
  gtkuiNewSize();
  gtkuiReport();

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
