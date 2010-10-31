# Copyright © 2010 Richard Kettlewell.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
bin_PROGRAMS=mandy
mandy_SOURCES=Gtkui.cc GtkuiControls.cc GtkuiDrawing.cc	\
GtkuiKeyboard.cc GtkuiPointer.cc mandy.cc Gtkui.h
mandy_LDADD=../lib/libmandy.a $(GTK_LIBS)
AM_CPPFLAGS=-I../lib
AM_CXXFLAGS=$(GLIB_CFLAGS) $(GTK_CFLAGS)
