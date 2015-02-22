# Copyright © 2010, 2012, 2015 Richard Kettlewell.
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
noinst_LIBRARIES=libmandy.a
noinst_PROGRAMS=fixed128-test fixed64-test speedtest cgitest
libmandy_a_SOURCES=Color.h Draw.cc Draw.h Fixed128-amd64.S Fixed128-str2.c	\
Fixed128.c Fixed128.h Fixed64-amd64.S Fixed64.c Fixed64.h Fixed64CC.cc	\
Fixed128CC.cc FractalJob.cc FractalJob.h IterBuffer.cc IterBuffer.h	\
Job.cc Job.h JuliaJob.cc JuliaJob.h MandelbrotJob.cc MandelbrotJob.h	\
Shell.cc Shell.h 						\
Threading-glib.cc Threading-glib.h Threading-pthreads.cc		\
Threading-pthreads.h Threading.h arith.cc arith.h fatal.cc mandy.h	\
cgi.cc cgi.h
fixed128_test_SOURCES=fixed128-test.c
fixed128_test_LDADD=libmandy.a -lm
fixed64_test_SOURCES=fixed64-test.c
fixed64_test_LDADD=libmandy.a -lm
speedtest_SOURCES=speedtest.cc
speedtest_LDADD=libmandy.a -lm
cgitest_SOURCES=cgitest.cc
cgitest_LDADD=libmandy.a
AM_CXXFLAGS=$(gtkmm_CFLAGS)
EXTRA_DIST=ansidecl.h getopt1.c getopt.c getopt.h
TESTS=fixed64-test fixed128-test cgitest

Fixed128-amd64.o: Fixed128-amd64.S
