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
#include <typeinfo>
#include "MainMenu.h"
#include "JuliaWindow.h"
#include "MandelbrotWindow.h"

namespace mmui {

  static GenericWindow *FindParent(Gtk::Menu *menu) {
    GenericWindow *parent = NULL;
    Gtk::Widget *attached = menu->property_attach_widget();
    Gtk::Container *w = attached->get_parent();
    while(w && (parent = dynamic_cast<GenericWindow *>(w)) == NULL)
      w = w->get_parent();
    return parent;
  }

  // File menu ----------------------------------------------------------------

  class FileMenu: public Gtk::Menu {
  public:
    FileMenu():
      saveMandelbrotImageItem("Save Mandelbrot set image"),
      saveJuliaImageItem("Save Julia set image"),
      closeItem(Gtk::Stock::CLOSE),
      quitItem(Gtk::Stock::QUIT)
    {
      append(saveMandelbrotImageItem);
      saveMandelbrotImageItem.signal_activate().connect
        (sigc::ptr_fun(SaveMandelbrotImageActivated));

      append(saveJuliaImageItem);
      saveJuliaImageItem.signal_activate().connect
        (sigc::ptr_fun(SaveJuliaImageActivated));

      append(closeItem);
      closeItem.signal_activate().connect(sigc::mem_fun(*this,
                                                        &FileMenu::CloseActivated));

      append(quitItem);
      quitItem.signal_activate().connect(sigc::ptr_fun(QuitActivated));
    }

    Gtk::MenuItem saveMandelbrotImageItem;
    static void SaveMandelbrotImageActivated() {
      mandelbrot->view.Save();
    }

    Gtk::MenuItem saveJuliaImageItem;
    static void SaveJuliaImageActivated() {
      julia->view.Save();
    }

    Gtk::ImageMenuItem closeItem;
    void CloseActivated() {
      GenericWindow *parent = FindParent(this);
      if(parent)
        parent->close();
    }

    Gtk::ImageMenuItem quitItem;
    static void QuitActivated() {
      exit(0);
    }

    virtual void on_show() {
      saveJuliaImageItem.set_sensitive(julia && julia->property_visible());
      Gtk::Menu::on_show();
    }
  };

  // Windows menu -------------------------------------------------------------

  class WindowsMenu: public Gtk::Menu {
  public:
    WindowsMenu(): juliaItem("Julia set") {
      append(juliaItem);
      juliaItem.signal_toggled().connect(sigc::mem_fun(*this,
                                                       &WindowsMenu::JuliaToggled));
    }

    Gtk::CheckMenuItem juliaItem;

    virtual void on_show() {
      juliaItem.set_active(julia && julia->property_visible());
      Gtk::Menu::on_show();
    }

    void JuliaToggled() {
      if(julia && juliaItem.get_active() != julia->property_visible()) {
        if(juliaItem.get_active())
          julia->show_all();
        else
          julia->hide();
      }
    }

  };

  // Help menu ----------------------------------------------------------------

  class HelpMenu: public Gtk::Menu {
  public:
    HelpMenu(): aboutItem(Gtk::Stock::ABOUT) {
      append(aboutItem);
      aboutItem.signal_activate().connect(sigc::mem_fun(*this,
                                                        &HelpMenu::AboutActivated));
    }
    Gtk::ImageMenuItem aboutItem;

    void AboutActivated() {
      Gtk::Dialog about("About Mandy",
                        FindParent(this),
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

  };

  // Menu bar -----------------------------------------------------------------

  Menubar::Menubar(): fileItem("File"),
                      windowsItem("Windows"),
                      helpItem("Help") {

    append(fileItem);
    fileItem.set_submenu(*manage(new FileMenu()));

    append(windowsItem);
    windowsItem.set_submenu(*manage(new WindowsMenu()));

    append(helpItem);
    helpItem.set_submenu(*manage(new HelpMenu()));
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
