#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#define MAXITER 255

static int calc(double cx, double cy) {
  // let c = cx + icy
  // let z = zx + izy
  //
  // then z^2 + c = zx^2 - zy^2 + cx + i(2zxzy+cy)
  int iterations = 0;
  double zx = 0, zy = 0;
  while(iterations < MAXITER && (zx * zx + zy * zy < 4.0)) {
    double nzx = zx * zx - zy * zy + cx;
    double nzy = 2 * zx * zy  + cy;
    zx = nzx;
    zy = nzy;
    ++iterations;
  }
  return iterations;
}

static void mand(double x, double y, double size, int xpixels, int ypixels,
		 int *results) {
  // TODO take advantage of multiple cores!
  for(int py = 0; py < ypixels; ++py) {
    for(int px = 0; px < xpixels; ++px) {
      *results++ = calc(x + px * size / xpixels,
			y + py * size / xpixels);
    }
  }
}

static void fatal(int errno_value, const char *fmt, ...) {
  va_list ap;

  fprintf(stderr,"FATAL: ");
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  if(errno_value)
    fprintf(stderr, ": %s\n", strerror(errno));
  else
    fputc('\n', stderr);
  exit(1);
}

// x, y are the centre
// size is the distance from the centre to the nearest edge
static double x = 0.0, y = 0.0, size = 2.0;
static GtkLabel *report_label;

static void report(void) {
  char buffer[128];
  snprintf(buffer, sizeof buffer,
	   "%g x %g radius %g", x, y, size);
  gtk_label_set_text(report_label, buffer);
}

static int *recompute(int w, int h) {
  int *iters = malloc(w * h * sizeof(int));
  if(w > h)
    mand(x - size * w / h, y - size, size * 2 * w / h, w, h, iters);
  else
    mand(x - size, y - size * h / w, size * 2, w, h, iters);
  return iters;
}

// Color lookup table
static struct {
  unsigned char r, g, b;
} colors[MAXITER];

static void init_colors(void) {
  // The complement is colorful
  for(int n = 0; n < MAXITER; ++n) {
    colors[n].r = (255 * (MAXITER - n)) / MAXITER;
    colors[n].g = 255 - (cos((double)n / MAXITER) + 1.0) * 127;
    colors[n].b = (255 * n) / MAXITER;
  }
  // The set itself is black
  colors[MAXITER].r = colors[MAXITER].g = colors[MAXITER].b = 0;
}

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
		  widget->style->fg_gc[gtk_widget_get_state (widget)],
		  pixbuf,
		  0, 0, 0, 0, w, h,
		  GDK_RGB_DITHER_NONE, 0, 0);
}

static gboolean exposed(GtkWidget *widget,
			GdkEventExpose __attribute__((unused)) *event,
			gpointer __attribute__((unused)) data) {
  redraw(widget, FALSE);
  return TRUE;
}


static gboolean dragging;
static double dragx, dragy;

static gboolean button_pressed(GtkWidget *widget,
			       GdkEventButton *event,
			       gpointer __attribute__((unused)) data) {
  // Middle button zooms
  if(event->type == GDK_BUTTON_PRESS
     && event->button == 2
     && event->state == 0) {
    gint w, h;
    gdk_drawable_get_size(widget->window, &w, &h);
    double xleft, ybottom, xsize, ysize;
    if(w > h) {
      xleft = x - size * w / h;
      ybottom = y - size;
      xsize = size * 2 * w / h;
      ysize = size * 2;
    } else {
      xleft = x - size;
      ybottom = y - size * h / w;
      xsize = size * 2;
      ysize = size * 2 * h / w;
    }
    x = xleft + event->x * xsize / w;
    y = ybottom + (h - 1 - event->y) * ysize / h;
    size = size / 1.414;
    report();
    redraw(widget, TRUE);
    return TRUE;
  }
  // Left button drags
  if(event->type == GDK_BUTTON_PRESS
     && event->button == 1
     && event->state == 0) {
    dragging = TRUE;
    dragx = event->x;
    dragy = event->y;
    return TRUE;
  }
  if(event->type == GDK_BUTTON_RELEASE
     && event->button == 1) {
    dragging = FALSE;
    return TRUE;
  }
  return FALSE;
}

static gboolean pointer_moved(GtkWidget *widget,
			      GdkEventMotion *event,
			      gpointer __attribute__((unused)) user_data) {
  if(!dragging)
    return FALSE;
  double deltax = event->x - dragx;
  double deltay = event->y - dragy;
  if(!(deltax == 0 && deltay == 0)) {
    dragx = event->x;
    dragy = event->y;
    gint w, h;
    gdk_drawable_get_size(widget->window, &w, &h);
    if(w > h) {
      x -= deltax * size * 2 / h;
      y += deltay * size * 2 / h;
    } else {
      x -= deltax * size * 2 / w;
      y += deltay * size * 2 / w;
    }
    report();
    redraw(widget, TRUE);
  }
  return TRUE;
}

static gboolean deleted(GtkWidget __attribute__((unused)) *widget,
			GdkEvent __attribute__((unused)) *event,
			gpointer __attribute__((unused)) data) {
  exit(0);
}

int main(int argc, char **argv) {
  if(!setlocale(LC_CTYPE, ""))
    fatal(errno, "error calling setlocale");
  if(!gtk_init_check(&argc, &argv))
    fatal(0, "gtk_init_check failed");

  init_colors();

  GtkWidget *toplevel = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title((GtkWindow *)toplevel, "mand");
  g_signal_connect(G_OBJECT(toplevel), "delete-event",
                   G_CALLBACK(deleted), NULL);

  // A drawing area for the results
  GtkWidget *da = gtk_drawing_area_new();
  gtk_widget_set_size_request(da, 256, 256);
  g_signal_connect(da, "expose-event", G_CALLBACK(exposed), NULL);
  gtk_widget_add_events(da,
			GDK_BUTTON_PRESS_MASK
			|GDK_BUTTON_RELEASE_MASK
			|GDK_POINTER_MOTION_MASK);
  g_signal_connect(da, "button-press-event", G_CALLBACK(button_pressed), NULL);
  g_signal_connect(da, "button-release-event", G_CALLBACK(button_pressed), NULL);
  g_signal_connect(da, "motion-notify-event", G_CALLBACK(pointer_moved), NULL);

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
