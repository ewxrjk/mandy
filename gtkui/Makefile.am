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
noinst_PROGRAMS=mklogoc
dist_noinst_DATA=logo.h images.h
mandy_SOURCES=Controls.cc GenericWindow.cc JuliaWindow.cc MainMenu.cc	\
MandelbrotView.cc MandelbrotWindow.cc View.cc mmui.cc Controls.h	\
GenericWindow.h JuliaView.h JuliaWindow.h MainMenu.h MandelbrotView.h	\
MandelbrotWindow.h View.h logo.h mmui.h
mandy_LDADD=../lib/libmandy.a $(gtkmm_LIBS) $(gthread_LIBS)
mklogoc_SOURCES=images.h mklogoc.cc
man_MANS=mandy.1
AM_CPPFLAGS=-I${top_srcdir}/lib
AM_CXXFLAGS=$(gtkmm_CFLAGS)

EMBED_IMAGE=${top_srcdir}/images/minimand256.png

MainMenu.o: logo.h

images.h: ${EMBED_IMAGE}
	gdk-pixbuf-csource --raw --name=logodata ${EMBED_IMAGE} > images.h.new
	mv images.h.new images.h

logo.h: mklogoc
	./mklogoc > logo.h
