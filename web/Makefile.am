# Copyright © 2015 Richard Kettlewell.
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
noinst_PROGRAMS=mandy.fcgi
mandy_fcgi_SOURCES=mfcgi.cc mfcgi.h process.cc
AM_CPPFLAGS=-I${top_srcdir}/lib
AM_CXXFLAGS=$(gdkpixbuf_CFLAGS)
LDADD=../lib/libmandy.a $(gdkpixbuf_LIBS)
EXTRA_DIST=mandy.html mandy.css mandy.js
