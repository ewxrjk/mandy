/* Copyright © 2015 Richard Kettlewell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "mandy.h"
#include "cgi.h"
#include <cstdio>
#include <stdexcept>
#include <climits>

static int errors;

#define ASSERT(expr) (void)(expr ? 0 :                  \
    (++errors,                                          \
     fprintf(stderr, "%s:%d: assertion failed: %s\n",   \
             __FILE__, __LINE__, #expr)))

#define ASSERT_THROWS(expr) do {                        \
  try {                                                 \
    (void)(expr);                                       \
    ++errors;                                           \
    fprintf(stderr, "%s:%d: failed to throw: %s\n",     \
            __FILE__, __LINE__, #expr);                 \
  } catch(std::runtime_error &e) {                      \
  }                                                     \
} while(0)

int main() {

  ASSERT(unhex('0') == 0);
  ASSERT(unhex('9') == 9);
  ASSERT(unhex('a') == 10);
  ASSERT(unhex('f') == 15);
  ASSERT(unhex('A') == 10);
  ASSERT(unhex('F') == 15);

  ASSERT_THROWS(unhex(-1));
  ASSERT_THROWS(unhex(0));
  ASSERT_THROWS(unhex('0'-1));
  ASSERT_THROWS(unhex('9'+1));
  ASSERT_THROWS(unhex('a'-1));
  ASSERT_THROWS(unhex('f'+1));
  ASSERT_THROWS(unhex('A'-1));
  ASSERT_THROWS(unhex('F'+1));
#if CHAR_MIN==0                 // placate picky compiler
#define ONE_TWO_EIGHT 128
#else
#define ONE_TWO_EIGHT (-128)
#endif
  ASSERT_THROWS(unhex(ONE_TWO_EIGHT));
  ASSERT_THROWS(unhex('0'|ONE_TWO_EIGHT));
  ASSERT_THROWS(unhex('a'|ONE_TWO_EIGHT));

  //TODO more needed here

  printf("%d errors\n", errors);
  return !!errors;
}
