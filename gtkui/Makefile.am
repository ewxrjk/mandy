# Copyright © Richard Kettlewell.
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
MandelbrotView.cc MandelbrotWindow.cc Movie.cc View.cc mmui.cc \
Controls.h ControlPanel.h ControlPanel.cc	\
GenericWindow.h JuliaView.h JuliaWindow.h MainMenu.h MandelbrotView.h	\
MandelbrotWindow.h Movie.h View.h logo.h mmui.h
mandy_LDADD=../lib/libmandy.a $(gtkmm_LIBS) $(gthread_LIBS)
mklogoc_SOURCES=images.h mklogoc.cc
man_MANS=mandy.1
AM_CPPFLAGS=-I${top_srcdir}/lib
AM_CXXFLAGS=$(gtkmm_CFLAGS)
EXTRA_DIST=${man_MANS}

EMBED_IMAGE=${top_srcdir}/images/minimand256.png

MainMenu.o: ${srcdir}/logo.h
mklogoc.o: ${srcdir}/images.h

${srcdir}/images.h: ${EMBED_IMAGE}
	gdk-pixbuf-csource --raw --name=logodata ${EMBED_IMAGE} > images.h.new
	@if cmp -s images.h.new ${srcdir}/images.h; then	\
	  echo rm -f images.h.new;					\
	  rm -f images.h.new;					\
	else							\
	  echo mv ${srcdir}/images.h.new ${srcdir}/images.h;	\
	  mv ${srcdir}/images.h.new ${srcdir}/images.h;		\
	fi

${srcdir}/logo.h: mklogoc
	./mklogoc > logo.h.new
	@if cmp -s logo.h.new ${srcdir}/logo.h; then		\
	  echo rm -f logo.h.new;				\
	  rm -f logo.h.new;					\
	else							\
	  echo mv ${srcdir}/logo.h.new ${srcdir}/logo.h;	\
	  mv ${srcdir}/logo.h.new ${srcdir}/logo.h;		\
	fi
