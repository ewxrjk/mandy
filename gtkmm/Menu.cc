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
#include <gtkmm/dialog.h>
#include <gtkmm/stock.h>

namespace mmui {
  Menubar::Menubar(Toplevel *toplevel_):
    toplevel(toplevel_),
    fileItem("File"),
      saveImageItem("Save image"),
      quitItem(Gtk::Stock::QUIT),
    helpItem("Help"),
      aboutItem(Gtk::Stock::ABOUT) {

    append(fileItem);
    fileItem.set_submenu(fileMenu);

    fileMenu.append(saveImageItem);
    saveImageItem.signal_activate().connect
      (sigc::mem_fun(*this,
                     &Menubar::SaveImageActivated));

    fileMenu.append(quitItem);
    quitItem.signal_activate().connect(sigc::mem_fun(*this,
                                                     &Menubar::QuitActivated));

    append(helpItem);
    helpItem.set_submenu(helpMenu);

    helpMenu.append(aboutItem);
    aboutItem.signal_activate().connect(sigc::mem_fun(*this,
                                                      &Menubar::AboutActivated));
  }

  void Menubar::QuitActivated() {
    exit(0);
  }

  void Menubar::AboutActivated() {
    Gtk::Dialog about("About Mandy",
                      get_parent(),
                      true/*modal*/);
    Gtk::VBox *vbox = about.get_vbox();
    Gtk::Label name;
    name.set_markup("<span font_desc=\"Sans 36\">Mandy</span>");
    Gtk::Label description("Mandelbrot/Julia Set Generator");
    Gtk::Label copyright("Version "VERSION" \xC2\xA9 2010 Richard Kettlewell");
    vbox->pack_start(name);
    vbox->pack_start(description);
    vbox->pack_start(copyright);
    about.add_button("OK", 0);
    about.show_all();
    about.run();
  }

  void Menubar::SaveImageActivated() {
    toplevel->view.Save();
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
