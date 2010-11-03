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
#include "mmui.h"

namespace mmui {
  Menubar::Menubar(): fileItem("File"),
                        quitItem("Quit"),
                      helpItem("Help"),
                        aboutItem("About") {
    append(fileItem);
    fileItem.set_submenu(fileMenu);
    fileMenu.append(quitItem);
    quitItem.signal_activate().connect(sigc::mem_fun(*this,
                                                     &Menubar::QuitActivated));

    append(helpItem);
    helpItem.set_submenu(helpMenu);
    helpMenu.append(aboutItem);
  }

  void Menubar::QuitActivated() {
    exit(0);
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
