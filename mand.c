#include "mand.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <errno.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

static GtkLabel *report_label;

/* Report current position, size, etc */
static void report(void) {
  char buffer[128];
  snprintf(buffer, sizeof buffer,
	   "%g x %g radius %g", xcenter, ycenter, size);
  gtk_label_set_text(report_label, buffer);
}

/* Recompute the iteration counts, which are returned in newly
 * allocated, caller-owned array. */
static int *recompute(int w, int h) {
  int *iters = malloc(w * h * sizeof(int));
  mand(xcenter - xsize(w, h) / 2,
       ycenter - ysize(w, h) / 2,
       xsize(w, h),
       w, h,
       iters);
  return iters;
}

/* Redraw onto widget either if the widget size has changed, or if
 * force is TRUE (indicating that the location changed) */
static void redraw(GtkWidget *widget,
		   gboolean force) {
  static GdkPixbuf *pixbuf = NULL;
  static int lastw = -1, lasth = -1;

  gint w, h;
  gdk_drawable_get_size(widget->window, &w, &h);

  if(w != lastw || h != lasth || force) {
    // Regenerate the pixel data, either because the size has changed
    // or because we're looking at something different.
    lastw = w;
    lasth = h;
    int *iters = recompute(w, h);
    // Convert to a pixbuf
    if(pixbuf)
      gdk_pixbuf_unref(pixbuf);
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
    guchar *const pixels = gdk_pixbuf_get_pixels(pixbuf);
    const int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    for(int y = 0; y < h; ++y)
      for(int x = 0; x < w; ++x) {
	const int count = iters[((h - 1) - y) * w + x];
	pixels[y * rowstride + x * 3 + 0] = colors[count].r;
	pixels[y * rowstride + x * 3 + 1] = colors[count].g;
	pixels[y * rowstride + x * 3 + 2] = colors[count].b;
      }
  }

  // Redraw the drawing area
  gdk_draw_pixbuf(widget->window,
		  widget->style->fg_gc[widget->state],
		  pixbuf,
		  0, 0, 0, 0, w, h,
		  GDK_RGB_DITHER_NONE, 0, 0);
}

/* expose-event callback */
static gboolean exposed(GtkWidget *widget,
			GdkEventExpose __attribute__((unused)) *event,
			gpointer __attribute__((unused)) data) {
  redraw(widget, FALSE);
  return TRUE;
}

/* Drag state */
static gboolean dragging;
static double dragfromx, dragfromy, dragtox, dragtoy;

/* motion-notify-event callback */
static gboolean pointer_moved(GtkWidget __attribute__((unused)) *widget,
			      GdkEventMotion *event,
			      gpointer __attribute__((unused)) user_data) {
  if(!dragging)
    return FALSE;
  dragtox = event->x;
  dragtoy = event->y;
  return TRUE;
}

/* Timeout to actually implement dragging.  The idea is to avoid
 * piling up recomputes.  It's better than doing it from pointer_moved
 * but still not good. */
static gboolean pointer_movement_timeout(gpointer data) {
  GtkWidget *widget = data;
  if(dragging) {
    double deltax = dragtox - dragfromx;
    double deltay = dragtoy - dragfromy;
    if(!(deltax == 0 && deltay == 0)) {
      dragfromx = dragtox;
      dragfromy = dragtoy;
      gint w, h;
      gdk_drawable_get_size(widget->window, &w, &h);
      drag(w, h, deltax, deltay);
      report();
      redraw(widget, TRUE);
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
    redraw(widget, TRUE);
    return TRUE;
  }
  // Left button drags
  if(event->type == GDK_BUTTON_PRESS
     && event->button == 1
     && event->state == 0) {
    dragging = TRUE;
    dragtox = dragfromx = event->x;
    dragtoy = dragfromy = event->y;
    return TRUE;
  }
  if(event->type == GDK_BUTTON_RELEASE
     && event->button == 1) {
    dragtox = event->x;
    dragtoy = event->y;
    pointer_movement_timeout(widget);
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
  init_colors();

  // The top level window
  GtkWidget *toplevel = gtk_window_new(GTK_WINDOW_TOPLEVEL);
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

  // Handle drags with a timeout
  g_timeout_add(100, pointer_movement_timeout, da);

  // A label to say what we're doing
  GtkWidget *label = gtk_label_new("spong");
  report_label = (GtkLabel *)label;
  report();

  // Pack it together vertically
  GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start((GtkBox *)vbox, da, TRUE, TRUE, 0);
  gtk_box_pack_end((GtkBox *)vbox, label, FALSE, FALSE, 1);
  gtk_container_add((GtkContainer *)toplevel, vbox);
  gtk_widget_show_all(toplevel);

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
