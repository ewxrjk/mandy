#	mandy - display the Mandelbrot set and its complement

This is a simple GTK+ program that lets you zoom in on the Mandelbrot
set.

## Installation

You will need [gtkmm](https://www.gtkmm.org) and either GNU C++ or Clang.

```
apt install libgtkmm-2.4-dev autoconf-archive
./autogen.sh
./configure 
make
sudo make install
```

If you want to compile with Clang :

```
./configure CC=clang CXX=clang++ CCAS=gcc
```

See `man mandy` for documentation.

## Arithmetic Type

You can select at runtime the arithmetic type used to compute the images.

`double` gives you about 53 bits of precision.  `long double` gives
you 64 bits of precision.  Both are floating point types and are
(probably) directly supported by your hardware, so should be fast.

`fixed64` gives you 56 bits of precision after the point.  It has had
some optimization effort but is nevertheless a bit slower than using
'double' on an x86-64 CPU.

`fixed128` gives you 96 bits of precision after the point.  It's
relatively slow.

On x86-64 CPUs the fixed point types are implemented in hand-written
assembly language.  On other CPUs they are implemented in C and are
likely to be very slow.

## Copyright

Copyright © Richard Kettlewell.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
