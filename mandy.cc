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
#include <cstring>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkkeysyms.h>

static gboolean gtkuiToplevelDeleted(GtkWidget *, GdkEvent *, gpointer);

// The results of the most recent computation
static IterBuffer *gtkuiLatestDest;
static GdkPixbuf *gtkuiLatestPixbuf;

// Where and how to draw the results
static GdkDrawable *gtkuiDrawable;
static GdkGC *gtkuiGC;
static GtkWidget *gtkuiToplevel;

// Drawing --------------------------------------------------------------------

// Called to just redraw whatever we've got
static void gtkuiRedraw(int x, int y, int w, int h) {
  gdk_draw_pixbuf(gtkuiDrawable,
		  gtkuiGC,
		  gtkuiLatestPixbuf,
		  x, y, x, y, w, h,
		  GDK_RGB_DITHER_NONE, 0, 0);
}

// Job completion callback
static void gtkuiCompleted(Job *generic_job) {
  MandelbrotJob *j = dynamic_cast<MandelbrotJob *>(generic_job);
  // Ignore stale jobs
  if(j->dest != gtkuiLatestDest)
    return;
  const int w = gtkuiLatestDest->w;
  guchar *const pixels = gdk_pixbuf_get_pixels(gtkuiLatestPixbuf);
  const int rowstride = gdk_pixbuf_get_rowstride(gtkuiLatestPixbuf);
  const int lx = j->x + j->w;
  const int ly = j->y + j->h;
  for(int y = j->y; y < ly; ++y) {
    int *datarow = &gtkuiLatestDest->data[y * w + j->x];
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
  if(gtkuiLatestDest) {
    gtkuiLatestDest->release();
    gtkuiLatestDest = NULL;
  }
  gint w, h;
  gdk_drawable_get_size(gtkuiDrawable, &w, &h);
  if(!gtkuiLatestPixbuf)
    gtkuiLatestPixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
  // TODO if there's a pixbuf available then ideally we would move or scale it
  // to provide continuity.
  gtkuiLatestDest = MandelbrotJob::recompute(xcenter, ycenter, size,
					 maxiter, w, h,
					 gtkuiCompleted);
}

// Called when a resize is detected
static void gtkuiNewSize() {
  // If there's a pixbuf it'll be the wrong size, so delete it.
  // TODO actually what we really wanted was to create the new pixbuf
  // from whatever is lying around in the old one, to provide some
  // continuity.
  if(gtkuiLatestPixbuf) {
    gdk_pixbuf_unref(gtkuiLatestPixbuf);
    gtkuiLatestPixbuf = NULL;
  }
  gtkuiNewLocation();
}

// Control panel --------------------------------------------------------------

class GtkuiControl {
public:
  GtkWidget *label;
  GtkWidget *entry;

  GtkuiControl(const char *caption) {
    label = gtk_label_new(caption);
    gtk_misc_set_alignment((GtkMisc *)label, 1.0, 0.0);
    entry = gtk_entry_new();
    g_signal_connect(entry, "activate",
                    G_CALLBACK(GtkuiControl::honorAll), NULL);
    controls.push_back(this);
  }

  static void changed() {
    for(size_t n = 0; n < controls.size(); ++n) {
      controls[n]->render();
    }
  }

private:
  static std::vector<GtkuiControl *> controls;

  static void honorAll() {
    // Check everything is valid
    for(size_t n = 0; n < controls.size(); ++n) {
      if(!controls[n]->validate()) {
        gdk_beep();
        gtk_widget_grab_focus(controls[n]->entry);
        return;
      }
    }
    // Set the new values
    for(size_t n = 0; n < controls.size(); ++n)
      controls[n]->honor();
    // Redraw accordingly
    init_colors();
    // TODO there is an optimization here: if maxiter has gone up then
    // we can skip computation of points with a known non-maximum
    // iteration count.
    gtkuiNewLocation();
  }

  void render() {
    char buffer[128];
    renderText(buffer, sizeof buffer);
    gtk_entry_set_text((GtkEntry *)entry, buffer);
  }

  bool validate() {
    const char *text = gtk_entry_get_text((GtkEntry *)entry);
    return validateText(text);
  }

  void honor() {
    const char *text = gtk_entry_get_text((GtkEntry *)entry);
    char buffer[128];
    renderText(buffer, sizeof buffer);
    if(strcmp(buffer, text))
      honorText(text);
  }

protected:
  virtual void renderText(char buffer[], size_t bufsize) = 0;
  virtual bool validateText(const char *text) = 0;
  virtual void honorText(const char *text) = 0;
};

std::vector<GtkuiControl *> GtkuiControl::controls;

class GtkuiIntegerControl: public GtkuiControl {
public:
  int *value;
  int min, max;

  GtkuiIntegerControl(const char *caption, int *value_, int min_, int max_):
    GtkuiControl(caption),
    value(value_),
    min(min_), max(max_) {
  }

  void renderText(char buffer[], size_t bufsize) {
    snprintf(buffer, bufsize, "%d", *value);
  }

  bool validateText(const char *text) {
    char *end;
    errno = 0;
    long n = strtol(text, &end, 10);
    if(errno
       || n < min
       || n > max
       || *end)
      return false;
    return true;
  }

  void honorText(const char *text) {
    *value = strtol(text, NULL, 10);
  }
};

class GtkuiDoubleControl: public GtkuiControl {
public:
  double *value;
  double min, max;
  GtkuiDoubleControl(const char *caption, double *value_, double min_, double max_):
    GtkuiControl(caption),
    value(value_),
    min(min_), max(max_) {
  }

  void renderText(char buffer[], size_t bufsize) {
    snprintf(buffer, bufsize, "%g", *value);
  }

  bool validateText(const char *text) {
    char *end;
    errno = 0;
    double n = strtod(text, &end);
    if(errno
       || n < min
       || n > max
       || *end)
      return false;
    return true;
  }

  void honorText(const char *text) {
    *value = strtod(text, NULL);
  }
};

/* Create the control panel */
static GtkWidget *gtkuiControlPanel(void) {
  GtkWidget *table = gtk_table_new(3, 4, FALSE);

  GtkuiControl *xControl = new GtkuiDoubleControl("X centre",
                                                  &xcenter,
                                                  -HUGE_VAL, HUGE_VAL);
  gtk_table_attach((GtkTable *)table,
                   xControl->label,
                   0, 1, 0, 1,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  gtk_table_attach((GtkTable *)table,
                   xControl->entry,
                   1, 2, 0, 1,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);

  GtkuiControl *yControl = new GtkuiDoubleControl("Y centre",
                                                  &ycenter,
                                                  -HUGE_VAL, HUGE_VAL);
  gtk_table_attach((GtkTable *)table,
                   yControl->label,
                   0, 1, 1, 2,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  gtk_table_attach((GtkTable *)table,
                   yControl->entry,
                   1, 2, 1, 2,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);

  GtkuiControl *radiusControl = new GtkuiDoubleControl("Radius",
                                                       &size,
                                                       0.0, HUGE_VAL);
  gtk_table_attach((GtkTable *)table,
                   radiusControl->label,
                   0, 1, 2, 3,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  gtk_table_attach((GtkTable *)table,
                   radiusControl->entry,
                   1, 2, 2, 3,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);

  GtkuiControl *maxiterControl = new GtkuiIntegerControl("Iterations",
                                                         &maxiter,
                                                         1, INT_MAX);
  gtk_table_attach((GtkTable *)table,
                   maxiterControl->label,
                   2, 3, 0, 1,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);
  gtk_table_attach((GtkTable *)table,
                   maxiterControl->entry,
                   3, 4, 0, 1,
                   GTK_FILL, (GtkAttachOptions)0, 1, 1);

  GtkWidget *frame = gtk_frame_new(NULL);
  gtk_container_add((GtkContainer *)frame, table);
  return frame;
}

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
    gdk_drawable_get_size(gtkuiDrawable, &w, &h);
    drag(w, h, deltax, deltay);
    GtkuiControl::changed();
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
    gdk_drawable_get_size(gtkuiDrawable, &w, &h);
    zoom(w, h, event->x, event->y, M_SQRT1_2);
    GtkuiControl::changed();
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
    GtkuiControl::changed();
    gtkuiNewLocation();
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
      gdk_drawable_get_size(gtkuiDrawable, &w, &h);
      if(event->keyval == GDK_equal || event->keyval == GDK_KP_Add)
        size *= M_SQRT1_2;
      else
        size *= M_SQRT2;
      GtkuiControl::changed();
      gtkuiNewLocation();
      return TRUE;
    }
    }
  }
  return FALSE;
}

// Miscelleneous callbacks ----------------------------------------------------

/* Timeout to handle delayed recompitation */
static gboolean gtkuiPeriodicPoll(gpointer __attribute__((unused)) data) {
  // See if anything's happened lately.
  // TODO we should arrange that the timeout is suppressed if nothing
  // is going on.
  Job::poll();
  return TRUE;
}

/* expose-event callback */
static gboolean gtkuiExposed(GtkWidget *, GdkEventExpose *, gpointer) {
  gint w, h;
  gdk_drawable_get_size(gtkuiDrawable, &w, &h);
  if(w != gdk_pixbuf_get_width(gtkuiLatestPixbuf)
     || h != gdk_pixbuf_get_height(gtkuiLatestPixbuf)) {
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
  GtkuiControl::changed();

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
