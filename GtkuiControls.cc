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
#include <vector>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <cstdlib>

namespace Gtkui {

  class Control {
  public:
    GtkWidget *label;
    GtkWidget *entry;

    Control(const char *caption) {
      label = gtk_label_new(caption);
      gtk_misc_set_alignment((GtkMisc *)label, 1.0, 0.0);
      entry = gtk_entry_new();
      g_signal_connect(entry, "activate",
		       G_CALLBACK(Control::honorAll), NULL);
      controls.push_back(this);
    }

    static void changed() {
      for(size_t n = 0; n < controls.size(); ++n) {
	controls[n]->render();
      }
    }

  private:
    static std::vector<Control *> controls;

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
      gint w, h;
      gdk_drawable_get_size(Gtkui::Drawable, &w, &h);
      Gtkui::NewLocation(w/2, h/2);
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

  std::vector<Control *> Control::controls;

  class GtkuiIntegerControl: public Control {
  public:
    int *value;
    int min, max;

    GtkuiIntegerControl(const char *caption, int *value_, int min_, int max_):
      Control(caption),
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

  class GtkuiDoubleControl: public Control {
  public:
    double *value;
    double min, max;
    GtkuiDoubleControl(const char *caption, double *value_, double min_, double max_):
      Control(caption),
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
  GtkWidget *ControlPanel() {
    GtkWidget *table = gtk_table_new(3, 4, FALSE);

    Control *xControl = new GtkuiDoubleControl("X centre",
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

    Control *yControl = new GtkuiDoubleControl("Y centre",
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

    Control *radiusControl = new GtkuiDoubleControl("Radius",
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

    Control *maxiterControl = new GtkuiIntegerControl("Iterations",
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

  void Changed() {
    Control::changed();
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
