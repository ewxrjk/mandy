#	mandy - display the Mandelbrot set and its complement

This is a simple GTK+ program that lets you zoom in on the Mandelbrot
set.

## Installation

You will need [gtkmm](https://www.gtkmm.org) and either GNU C++ or Clang.

```
apt install libgtkmm-2.4-dev autoconf-archive
./autogen.sh
./configure 
make check
sudo make install
```

If you want to compile with Clang :

```
./configure CC=clang CXX=clang++ CCAS=gcc
```

See `man mandy` for documentation.

## Arithmetic Type

You can select at runtime the arithmetic type used to compute the images.

`double` gives you about 53 bits of precision.
It is usually directly supported by CPU hardware, so very fast.

`simd` is the same data type as `double` but uses vector instructions
if available, giving a 2x or 4x acceleration depending on platform.

`long double` is very platform dependent. On x86 it gives you 64 bits of precision. On Arm it is more precise but also quite slow, due to lack of hardware support.

`fixed64` gives you 56 bits of precision after the point.  The x86 implementation
is well optimized.

`fixed128` gives you 96 bits of precision after the point.  The x86 implementation
is well optimized.

`fixed256` gives you 192 bits of precision after the point.  The x86 and Arm
implementations are somewhat optimized.

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
