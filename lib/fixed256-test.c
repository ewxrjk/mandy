/* Copyright © Richard Kettlewell.
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
#include "Fixed256.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

static void fixed256_print(const union Fixed256 *f) {
  printf("%08x %08x.%08x %08x %08x %08x %08x %08x",
         f->u32[7],
         f->u32[6],
         f->u32[5],
         f->u32[4],
         f->u32[3],
         f->u32[2],
         f->u32[1],
         f->u32[0]);
}

static void
fixed256_check(const char *file, int line, const char *func, const union Fixed256 *got, const union Fixed256 *expect) {
  if(Fixed256_ne(expect, got)) {
    printf("%s:%d: %s:\n", file, line, func);
    printf("expect: ");
    fixed256_print(expect);
    printf("\n");
    printf("   got: ");
    fixed256_print(got);
    printf("\n");
    exit(1);
  }
}

static void
fixed256_check_str(const char *file, int line, const char *func, const char *got, const char *expect) {
  if(strcmp(expect, got)) {
    printf("%s:%d: %s:\n", file, line, func);
    printf("expect: %s\n", expect);
    printf("   got: %s\n", got);
    exit(1);
  }
}

static void
fixed256_check_bool(const char *file, int line, const char *func, bool got, bool expect) {
  if(expect != got) {
    printf("%s:%d: %s: expected %d got %d\n", file, line, func, expect, got);
    exit(1);
  }
}

static void
fixed256_check_double(const char *file, int line, const char *func, double got, double expect) {
  if(expect != got) {
    printf("%s:%d: %s: expected %a got %a\n", file, line, func, expect, got);
    exit(1);
  }
}

#define CHECK(GOT, EXPECT) fixed256_check(__FILE__, __LINE__, __func__, &GOT, &EXPECT)
#define CHECK_BOOL(GOT, EXPECT) fixed256_check_bool(__FILE__, __LINE__, __func__, GOT, EXPECT)
#define CHECK_STR(GOT, EXPECT) fixed256_check_str(__FILE__, __LINE__, __func__, GOT, EXPECT)
#define CHECK_DOUBLE(GOT, EXPECT) fixed256_check_double(__FILE__, __LINE__, __func__, GOT, EXPECT)

#include "fixed256-test.inc"

int main() {

  fixed256_test_neg();
  fixed256_test_square();

  fixed256_test_compare();
  fixed256_test_compare_unsigned();

  fixed256_test_add();
  fixed256_test_sub();
  fixed256_test_mul();
  fixed256_test_div();
  fixed256_test_sqrt();

  fixed256_test_2str();
  fixed256_test_str2();

  fixed256_test_double2();
  fixed256_test_2double();

  return 0;
}
