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

INSTALL=install
CC=gcc -Wall -W -Werror -std=c99
CFLAGS=-O2 -g
CPPFLAGS:=$(shell pkg-config --cflags gtk+-2.0)
LIBS:=$(shell pkg-config --libs gtk+-2.0)
OBJECT=mand.o fatal.o background.o colors.o location.o movement.o

mand: $(OBJECT)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECT) $(LIBS)

$(OBJECT): mand.h

installdirs:
	mkdir -p ${DESTDIR}${bindir}

install: installdirs
	$(INSTALL) -m 755 mand ${DESTDIR}${bindir}/mand

clean:
	rm -f mand
	rm -f *.o
