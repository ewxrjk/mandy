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
libmandy_a_SOURCES=Draw.cc fatal.cc Fixed64CC.cc FixedCC.cc		\
FractalJob.cc IterBuffer.cc Job.cc JuliaJob.cc MandelbrotJob.cc		\
arith.h Color.h Draw.h Fixed64.h Fixed.h FractalJob.h IterBuffer.h	\
Job.h JuliaJob.h MandelbrotJob.h mandy.h Fixed64.c Fixed.c		\
Fixed-str2.c Fixed64-amd64.S Fixed-amd64.S
fixed_test_SOURCES=fixed-test.c
fixed_test_LDADD=../lib/libmandy.a -lm
fixed64_test_SOURCES=fixed64-test.c
fixed64_test_LDADD=../lib/libmandy.a -lm
speedtest_SOURCES=speedtest.cc
speedtest_LDADD=../lib/libmandy.a -lm
AM_CXXFLAGS=$(gtkmm_CFLAGS)

Fixed-amd64.o: Fixed-amd64.S
