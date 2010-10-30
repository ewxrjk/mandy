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

  MandyWindow::MandyWindow() {
    set_title("mandy");
    add_events(Gdk::KEY_RELEASE_MASK);
  }

  bool MandyWindow::on_delete_event(GdkEventAny *) {
    Job::destroy();
    exit(0);
  }

  bool MandyWindow::on_key_release_event(GdkEventKey *event) {
    if(event->state == Gdk::CONTROL_MASK) {
      switch(event->keyval) {
      case 'w': case 'W':
	Job::destroy();
	exit(0);
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
