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
noinst_LIBRARIES=libmandy.a
noinst_PROGRAMS=fixed-test fixed64-test speedtest
libmandy_a_SOURCES=Color.h Draw.cc Draw.h Fixed-amd64.S Fixed-str2.c	\
Fixed.c Fixed.h Fixed64-amd64.S Fixed64.c Fixed64.h Fixed64CC.cc	\
FixedCC.cc FractalJob.cc FractalJob.h IterBuffer.cc IterBuffer.h	\
Job.cc Job.h JuliaJob.cc JuliaJob.h MandelbrotJob.cc MandelbrotJob.h	\
Shell.cc Shell.h 						\
Threading-glib.cc Threading-glib.h Threading-pthreads.cc		\
Threading-pthreads.h Threading.h arith.h fatal.cc mandy.h
fixed_test_SOURCES=fixed-test.c
fixed_test_LDADD=../lib/libmandy.a -lm
fixed64_test_SOURCES=fixed64-test.c
fixed64_test_LDADD=../lib/libmandy.a -lm
speedtest_SOURCES=speedtest.cc
speedtest_LDADD=../lib/libmandy.a -lm
AM_CXXFLAGS=$(gtkmm_CFLAGS)
EXTRA_DIST=ansidecl.h getopt1.c getopt.c getopt.h

Fixed-amd64.o: Fixed-amd64.S
