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
prefix=/usr/local
exec_prefix=${prefix}
bindir=${exec_prefix}/bin

VERSION=0.0.WIP

INSTALL=install
CXX=g++ -Wall -W -Werror
CXXFLAGS=-O2 -g
CPPFLAGS:=$(shell pkg-config --cflags gtk+-2.0)
LIBS:=$(shell pkg-config --libs gtk+-2.0)
OBJECT=mandy.o fatal.o background.o colors.o location.o movement.o
SOURCE:=$(subst .o,.cc,${OBJECT}) mandy.h

mandy: $(OBJECT)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECT) $(LIBS)

%.s: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -S $<

$(OBJECT): mandy.h

installdirs:
	mkdir -p ${DESTDIR}${bindir}

install: installdirs
	$(INSTALL) -m 755 mandy ${DESTDIR}${bindir}/mandy

dist:
	rm -rf mandy-${VERSION}
	mkdir mandy-${VERSION}
	cp Makefile README COPYING mandy-${VERSION}/.
	cp ${SOURCE} mandy-${VERSION}/.
	tar cf mandy-${VERSION}.tar mandy-${VERSION}
	gzip -9f mandy-${VERSION}.tar
	rm -rf mandy-${VERSION}

distcheck: dist
	gzip -cd mandy-${VERSION}.tar.gz | tar xf -
	cd mandy-${VERSION} && $(MAKE)
	cd mandy-${VERSION} && $(MAKE) install DESTDIR=distcheck-tmp
	rm -rf mandy-${VERSION}

clean:
	rm -f mandy
	rm -f *.o
