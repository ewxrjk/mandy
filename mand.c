#include "mand.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <errno.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

static void recompute(void);

// The results of the most recent computation
static int *latest_iters;
static GdkPixbuf *latest_pixbuf;

// Set if a computation is ongoing
static gboolean recomputing;

// Where and how to draw the results
static GdkDrawable *drawable;
static GdkGC *gc;
static GtkWidget *toplevel;

// Cursor
static GdkCursor *busy_cursor;

// Reporting entry
static GtkWidget *xentry, *yentry, *rentry;

static void text_activated(GtkEntry *entry, gpointer user_data) {
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
  recompute();
}

/* Create the control panel */
static GtkWidget *controlpanel(void) {
  GtkWidget *table = gtk_table_new(3, 4, FALSE);
  GtkWidget *xcaption, *ycaption, *rcaption;

  gtk_table_attach((GtkTable *)table,
                   (xcaption = gtk_label_new("X centre")),
                   0, 1, 0, 1,
                   GTK_FILL, 0, 1, 1);
  gtk_misc_set_alignment((GtkMisc *)xcaption, 1.0, 0.0);
  gtk_table_attach((GtkTable *)table,
                   (xentry = gtk_entry_new()),
                   1, 2, 0, 1,
                   GTK_FILL, 0, 1, 1);
  g_signal_connect(xentry, "activate", G_CALLBACK(text_activated), &xcenter);

  gtk_table_attach((GtkTable *)table,
                   (ycaption = gtk_label_new("Y centre")),
                   0, 1, 1, 2,
                   GTK_FILL, 0, 1, 1);
  gtk_misc_set_alignment((GtkMisc *)ycaption, 1.0, 0.0);
  gtk_table_attach((GtkTable *)table,
                   (yentry = gtk_entry_new()),
                   1, 2, 1, 2,
                   GTK_FILL, 0, 1, 1);
  g_signal_connect(yentry, "activate", G_CALLBACK(text_activated), &ycenter);

  gtk_table_attach((GtkTable *)table,
                   (rcaption = gtk_label_new("Radius")),
                   0, 1, 2, 3,
                   GTK_FILL, 0, 1, 1);
  gtk_misc_set_alignment((GtkMisc *)rcaption, 1.0, 0.0);
  gtk_table_attach((GtkTable *)table,
                   (rentry = gtk_entry_new()),
                   1, 2, 2, 3,
                   GTK_FILL, 0, 1, 1);
  g_signal_connect(rentry, "activate", G_CALLBACK(text_activated), &size);

  GtkWidget *frame = gtk_frame_new(NULL);
  gtk_container_add((GtkContainer *)frame, table);
  return frame;
}

/* Report current position, size, etc */
static void report(void) {
  char buffer[128];
  snprintf(buffer, sizeof buffer, "%g", xcenter);
  gtk_entry_set_text((GtkEntry *)xentry, buffer);
  snprintf(buffer, sizeof buffer, "%g", ycenter);
  gtk_entry_set_text((GtkEntry *)yentry, buffer);
  snprintf(buffer, sizeof buffer, "%g", size);
  gtk_entry_set_text((GtkEntry *)rentry, buffer);
}

/* Attempt to recompute and redraw when we either know something has
 * changed or when we have no data anyway */
static void recompute(void) {
  // Discard old data
  free(latest_iters);
  // Find the current window size
  gint w, h;
  gdk_drawable_get_size(drawable, &w, &h);
  // Attempt to get data
  latest_iters = compute(xcenter - xsize(w, h) / 2,
			 ycenter - ysize(w, h) / 2,
			 xsize(w, h),
			 w, h);
  // If it's not ready yet just return.  We'll get a timeout callback
  // soon enough.
  if(!latest_iters) {
    if(!recomputing)
      gdk_window_set_cursor(toplevel->window, busy_cursor);
    recomputing = TRUE;
    return;
  }
  // Discard any existing pixels
  if(latest_pixbuf)
    gdk_pixbuf_unref(latest_pixbuf);
  // Create a new pixbuf
  latest_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
  // Fill it in.
  guchar *const pixels = gdk_pixbuf_get_pixels(latest_pixbuf);
  const int rowstride = gdk_pixbuf_get_rowstride(latest_pixbuf);
  for(int y = 0; y < h; ++y)
    for(int x = 0; x < w; ++x) {
      const int count = latest_iters[((h - 1) - y) * w + x];
      pixels[y * rowstride + x * 3 + 0] = colors[count].r;
      pixels[y * rowstride + x * 3 + 1] = colors[count].g;
      pixels[y * rowstride + x * 3 + 2] = colors[count].b;
    }
  // Draw it.
  recomputing = FALSE;
}

/* Redraw with whatever's in the latest pixbuf (even if wrong) */
static void redraw(void) {
  if(latest_pixbuf)
    gdk_draw_pixbuf(drawable,
		    gc,
		    latest_pixbuf,
		    0, 0, 0, 0, -1, -1,
		    GDK_RGB_DITHER_NONE, 0, 0);
}

/* expose-event callback */
static gboolean exposed(GtkWidget __attribute__((unused)) *widget,
			GdkEventExpose __attribute__((unused)) *event,
			gpointer __attribute__((unused)) data) {
  if(latest_pixbuf) {
    // We already have some pixels.
    gint w, h;
    gdk_drawable_get_size(drawable, &w, &h);
    if(w != gdk_pixbuf_get_width(latest_pixbuf)
       || h != gdk_pixbuf_get_height(latest_pixbuf)) {
      // The pixbuf is the wrong size (i.e. the window has been
      // resized).  Attempt a recompute.
      recompute();
    }
  } else {
    // No cached pixbuf.  Attempt a recompute.
    recompute();
  }
  // Draw whatever we've got.
  redraw();
  return TRUE;
}

/* Drag state */
static gboolean dragging;
static double dragfromx, dragfromy;

/* Drag from dragfrom[xy] to a new pointer location */
static void dragto(double dragtox, double dragtoy) {
  double deltax = dragtox - dragfromx;
  double deltay = dragtoy - dragfromy;
  if(!(deltax == 0 && deltay == 0)) {
    dragfromx = dragtox;
    dragfromy = dragtoy;
    gint w, h;
    gdk_drawable_get_size(drawable, &w, &h);
    drag(w, h, deltax, deltay);
    report();
    recompute();
  }
}

/* motion-notify-event callback */
static gboolean pointer_moved(GtkWidget __attribute__((unused)) *widget,
			      GdkEventMotion *event,
			      gpointer __attribute__((unused)) user_data) {
  if(!dragging)
    return FALSE;
  dragto(event->x, event->y);
  return TRUE;
}

/* Timeout to handle delayed recompitation */
static gboolean timeout(gpointer __attribute__((unused)) data) {
  if(recomputing) {
    // The last recompute() call didn't get an answer.  Make another
    // one.
    recompute();
    if(!recomputing) {
      gdk_window_set_cursor(toplevel->window, NULL);
      // We must have got an answer
      redraw();
    }
  }
  return TRUE;
}

/* button-{press,release}-event callback */
static gboolean button_pressed(GtkWidget *widget,
			       GdkEventButton *event,
			       gpointer __attribute__((unused)) data) {
  // Middle button zooms
  if(event->type == GDK_BUTTON_PRESS
     && event->button == 2
     && event->state == 0) {
    gint w, h;
    gdk_drawable_get_size(widget->window, &w, &h);
    zoom(w, h, event->x, event->y);
    report();
    recompute();
    return TRUE;
  }
  // Left button drags
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
    dragto(event->x, event->y);
    dragging = FALSE;
    return TRUE;
  }
  return FALSE;
}

/* delete-event callback */
static gboolean deleted(GtkWidget __attribute__((unused)) *widget,
			GdkEvent __attribute__((unused)) *event,
			gpointer __attribute__((unused)) data) {
  destroy_threads();
  exit(0);
}

int main(int argc, char **argv) {
  if(!setlocale(LC_CTYPE, ""))
    fatal(errno, "error calling setlocale");
  if(!gtk_init_check(&argc, &argv))
    fatal(0, "gtk_init_check failed");

  // Bits of infrastructure
  init_threads();
  init_colors(255);

  // The top level window
  toplevel = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title((GtkWindow *)toplevel, "mand");
  g_signal_connect(G_OBJECT(toplevel), "delete-event",
                   G_CALLBACK(deleted), NULL);

  // A drawing area for the results
  GtkWidget *da = gtk_drawing_area_new();
  gtk_widget_set_size_request(da, 384, 384);
  g_signal_connect(da, "expose-event", G_CALLBACK(exposed), NULL);
  gtk_widget_add_events(da,
			GDK_BUTTON_PRESS_MASK
			|GDK_BUTTON_RELEASE_MASK
			|GDK_POINTER_MOTION_MASK);
  g_signal_connect(da, "button-press-event", G_CALLBACK(button_pressed), NULL);
  g_signal_connect(da, "button-release-event", G_CALLBACK(button_pressed), NULL);
  g_signal_connect(da, "motion-notify-event", G_CALLBACK(pointer_moved), NULL);

  // Timeout to pick up the results of delayed recomputation
  g_timeout_add(10, timeout, NULL);

  // Pack it together vertically
  GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start((GtkBox *)vbox, controlpanel(), FALSE, FALSE, 1);
  gtk_box_pack_end((GtkBox *)vbox, da, TRUE, TRUE, 0);
  gtk_container_add((GtkContainer *)toplevel, vbox);
  gtk_widget_show_all(toplevel);

  // We only know these after the first _show_all call
  drawable = da->window;
  gc = da->style->fg_gc[da->state];
  busy_cursor = gdk_cursor_new(GDK_WATCH);

  // Start an initial computation.
  recompute();
  report();

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
