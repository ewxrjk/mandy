/* Copyright © Richard Kettlewell.
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

#ifndef MENU_H
#define MENU_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#include <gtkmm/menubar.h>
#include <gtkmm/menuitem.h>
#pragma GCC diagnostic pop

namespace mmui {

class Menubar: public Gtk::MenuBar {
public:
  Menubar();

  Gtk::MenuItem fileItem;
  Gtk::MenuItem windowsItem;
  Gtk::MenuItem helpItem;
};

} // namespace mmui

#endif /* MENU_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
