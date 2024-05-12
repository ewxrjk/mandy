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
#include "Fixed128.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

static void fixed128_print(const union Fixed128 *f) {
  printf("%08x %08x.%08x %08x",
         f->word[3],
         f->word[2],
         f->word[1],
         f->word[0]);
}

static void
fixed128_check(const char *file, int line, const char *func, const union Fixed128 *got, const union Fixed128 *expect) {
  if(Fixed128_ne(expect, got)) {
    printf("%s:%d: %s:\n", file, line, func);
    printf("expect: ");
    fixed128_print(expect);
    printf("\n");
    printf("   got: ");
    fixed128_print(got);
    printf("\n");
    exit(1);
  }
}

static void
fixed128_check_str(const char *file, int line, const char *func, const char *got, const char *expect) {
  if(strcmp(expect, got)) {
    printf("%s:%d: %s:\n", file, line, func);
    printf("expect: %s\n", expect);
    printf("   got: %s\n", got);
    exit(1);
  }
}

static void
fixed128_check_bool(const char *file, int line, const char *func, bool got, bool expect) {
  if(expect != got) {
    printf("%s:%d: %s: expected %d got %d\n", file, line, func, expect, got);
    exit(1);
  }
}

static void
fixed128_check_double(const char *file, int line, const char *func, double got, double expect) {
  if(expect != got) {
    printf("%s:%d: %s: expected %a got %a\n", file, line, func, expect, got);
    exit(1);
  }
}

#define CHECK(GOT, EXPECT) fixed128_check(__FILE__, __LINE__, __func__, &GOT, &EXPECT)
#define CHECK_BOOL(GOT, EXPECT) fixed128_check_bool(__FILE__, __LINE__, __func__, GOT, EXPECT)
#define CHECK_STR(GOT, EXPECT) fixed128_check_str(__FILE__, __LINE__, __func__, GOT, EXPECT)
#define CHECK_DOUBLE(GOT, EXPECT) fixed128_check_double(__FILE__, __LINE__, __func__, GOT, EXPECT)

#include "fixed128-test.inc"


static int errors;

static void printFixed128(const union Fixed128 *f, const char *expect) {
  char buffer[128];
  printf("%08x.%08x %08x %08x",
         f->word[NFIXED128 - 1],
         f->word[NFIXED128 - 2],
         f->word[NFIXED128 - 3],
         f->word[NFIXED128 - 4]);
  printf(" =%s", Fixed128_2str(buffer, sizeof buffer, f, 10));
  if(strcmp(buffer, expect)) {
    printf("<EXPECTED %s>", expect);
    ++errors;
  }
}

int main() {

  // Generated tests

  fixed128_test_neg();
  fixed128_test_square();

  fixed128_test_compare();
  fixed128_test_compare_unsigned();

  fixed128_test_add();
  fixed128_test_sub();
  fixed128_test_mul();
  fixed128_test_div();
  fixed128_test_sqrt();

  fixed128_test_2str();
  fixed128_test_str2();

  fixed128_test_double2();
  fixed128_test_2double();

  // Legacy manual tests

  union Fixed128 a, b, c, d;
  double x;
#if HAVE_ASM_FIXED128_ITERATE
  int count;
#endif

  // Divide by 2 across word boundary...
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  printf("1/2:     ");
  printFixed128(&a, "0.5");
  putchar('\n');
  // ...and within one word
  Fixed128_divu(&a, &a, 4);
  printf("0.5/4:   ");
  printFixed128(&a, "0.125");
  putchar('\n');

  // Add across word boundary
  Fixed128_int2(&b, 1);
  Fixed128_add(&c, &a, &b);
  printf("1+0.125: ");
  printFixed128(&c, "1.125");
  putchar('\n');

  // Add with carry
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  b = a;
  Fixed128_add(&c, &a, &b);
  printf("0.5+0.5: ");
  printFixed128(&c, "1");
  putchar('\n');

  // Negation
  Fixed128_int2(&a, 10);
  Fixed128_neg(&b, &a);
  Fixed128_neg(&c, &b);
  printf("-10:     ");
  printFixed128(&b, "-10");
  putchar('\n');
  printf("-(-10):  ");
  printFixed128(&c, "10");
  putchar('\n');

  // Subtraction
  Fixed128_int2(&a, 1);
  Fixed128_int2(&b, 10);
  Fixed128_sub(&c, &b, &a);
  printf("10-1:    ");
  printFixed128(&c, "9");
  putchar('\n');
  Fixed128_sub(&c, &a, &b);
  printf("1-10:    ");
  printFixed128(&c, "-9");
  putchar('\n');

  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 16);
  Fixed128_int2(&b, 1);
  Fixed128_divu(&b, &b, 256);
  Fixed128_sub(&c, &a, &b);
  printf("15/256:  ");
  printFixed128(&c, "0.05859375");
  putchar('\n');

  Fixed128_int2(&a, 0);
  b = a;
  a.word[NFIXED128 - 1] = 0x4321;
  a.word[NFIXED128 - 2] = 0x5678abcd;
  a.word[NFIXED128 - 3] = 0xef012345;
  b.word[NFIXED128 - 1] = 0x1234;
  b.word[NFIXED128 - 2] = 0xabcd5678;
  b.word[NFIXED128 - 3] = 0x12345678;
  printf("a:       ");
  printFixed128(&a, "17185.3377787950294118167406283437248504242234048433601856231689453125");
  putchar('\n');
  printf("b:       ");
  printFixed128(&b, "4660.6711019557134972677962803100371047548833303153514862060546875");
  putchar('\n');
  Fixed128_sub(&c, &a, &b);
  printf("a-b:     ");
  printFixed128(&c, "12524.6666768393159145489443480336877456693400745280086994171142578125");
  putchar('\n');
  Fixed128_sub(&c, &b, &a);
  printf("b-a:     ");
  printFixed128(&c,
                "-12524."
                "6666768393159145489443480336877456693400745280086994171142578125");
  putchar('\n');

  // Multiply
  Fixed128_int2(&a, 5);
  Fixed128_int2(&b, 7);
  Fixed128_mul(&c, &a, &b);
  printf("5*7:     ");
  printFixed128(&c, "35");
  putchar('\n');

  // Multiply by 0.5
  Fixed128_int2(&a, 101);
  Fixed128_int2(&b, 1);
  Fixed128_divu(&b, &b, 2);
  Fixed128_mul(&c, &a, &b);
  printf("101*0.5  ");
  printFixed128(&c, "50.5");
  putchar('\n');

  // Multiply with nontrivial sign
  Fixed128_int2(&a, -10);
  Fixed128_int2(&b, -5);
  Fixed128_mul(&c, &a, &b);
  printf("-10*-5:  ");
  printFixed128(&c, "50");
  putchar('\n');
  Fixed128_int2(&a, -10);
  Fixed128_int2(&b, 5);
  Fixed128_mul(&c, &a, &b);
  printf("-10*5:   ");
  printFixed128(&c, "-50");
  putchar('\n');

  // Multiply regression
  memset(&a, 0, sizeof a);
  memset(&b, 0, sizeof b);
  a.word[NFIXED128 - 1] = 0x00000000;
  a.word[NFIXED128 - 2] = 0x95f61998;
  a.word[NFIXED128 - 3] = 0x0c433000;
  a.word[NFIXED128 - 4] = 0x00000000; /* about 0.5857864... */
  b.word[NFIXED128 - 1] = 0xffffffff;
  b.word[NFIXED128 - 2] = 0xfaaaaaaa;
  b.word[NFIXED128 - 3] = 0xaaaaaaaa;
  b.word[NFIXED128 - 4] = 0xaaaaaaaa; /* about -0.02083333... */
  printf("a:       ");
  printFixed128(&a, "0.5857864376269048545253781412611715495586395263671875");
  putchar('\n');
  printf("b:       ");
  printFixed128(&b,
                "-0."
                "0208333333333333333333333333417478496556907925910584380296"
                "8305311651420197449624538421630859375");
  putchar('\n');
  Fixed128_mul(&c, &a, &b);
  printf("a*b:     ");
  printFixed128(&c, "-0.01220388411722718446927871127627440728247165679931640625");
  putchar('\n');

  // Very small numbers
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 65536);
  Fixed128_divu(&a, &a, 65536);
  printf("2⁻³²:    ");
  printFixed128(&a, "0.00000000023283064365386962890625");
  putchar('\n');
  b = a;
  Fixed128_mul(&c, &a, &b);
  printf("(2⁻³²)²: ");
  printFixed128(&c, "0.0000000000000000000542101086242752217003726400434970855712890625");
  putchar('\n');

  // Even smaller than that
  Fixed128_mul(&d, &a, &c);
  printf("(2⁻³²)³: ");
  printFixed128(&d,
                "0."
                "0000000000000000000000000000126217744835361888865876570445"
                "24579674771302961744368076324462890625");
  putchar('\n');

  // Underflow rounding
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  Fixed128_mul(&c, &a, &d);
  printf("½(2⁻³²)³:");
  printFixed128(&d,
                "0."
                "0000000000000000000000000000126217744835361888865876570445"
                "24579674771302961744368076324462890625");
  putchar('\n');

  // Division
  Fixed128_int2(&a, 1);
  Fixed128_int2(&b, 16);
  Fixed128_div(&c, &a, &b);
  printf("1/16:    ");
  printFixed128(&c, "0.0625");
  putchar('\n');

  Fixed128_int2(&a, 1);
  Fixed128_int2(&b, 3);
  Fixed128_div(&c, &a, &b);
  printf("1/3:     ");
  printFixed128(&c,
                "0."
                "3333333333333333333333333333291260751721546037044707809851"
                "58473441742899012751877307891845703125");
  putchar('\n');

  Fixed128_div(&c, &c, &b);
  printf("1/3/3:   ");
  printFixed128(&c,
                "0."
                "1111111111111111111111111111097086917240515345681569269950"
                "52824480580966337583959102630615234375");
  putchar('\n');

  // Square roots
  Fixed128_int2(&a, 2);
  Fixed128_sqrt(&a, &a);
  printf("√2:      ");
  printFixed128(&a,
                "1."
                "4142135623730950488016887242107539717357539105768580089614"
                "31689698429181589744985103607177734375");
  putchar('\n');
  Fixed128_mul(&b, &a, &a);
  printf("(√2)²:   ");
  printFixed128(&b, "2");
  putchar('\n');

  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  Fixed128_sqrt(&a, &a);
  printf("√½:      ");
  printFixed128(&a,
                "0."
                "7071067811865475244008443620990660986261088608451351759584"
                "53555011828939314000308513641357421875");
  putchar('\n');
  Fixed128_mul(&b, &a, &a);
  printf("(√½)²:   ");
  printFixed128(&b,
                "0."
                "4999999999999999999999999999873782255164638111134123429554"
                "75420325228697038255631923675537109375");
  putchar('\n');

  // Conversion to/from double
  Fixed128_double2(&a, 1.0);
  printf("1.0:     ");
  printFixed128(&a, "1");
  putchar('\n');
  x = Fixed128_2double(&a);
  printf("and back: %g\n", x);
  if(x != 1) {
    printf("-- EXPECTED 1\n");
    ++errors;
  }
  Fixed128_double2(&a, 0.5);
  printf("0.5:     ");
  printFixed128(&a, "0.5");
  putchar('\n');
  x = Fixed128_2double(&a);
  printf("and back: %g\n", x);
  if(x != 0.5) {
    printf("-- EXPECTED 0.5\n");
    ++errors;
  }
  Fixed128_double2(&a, M_PI);
  printf("π:       ");
  printFixed128(&a, "3.141592653589793115997963468544185161590576171875");
  putchar('\n');
  x = Fixed128_2double(&a);
  printf("and back: %g\n", x);
  if(x != M_PI) {
    printf("-- EXPECTED π\n");
    ++errors;
  }

  // Conversion from string
  Fixed128_str2(&a, "1.0", NULL);
  printf("1.0:     ");
  printFixed128(&a, "1");
  putchar('\n');
  Fixed128_str2(&a, "1.5", NULL);
  printf("1.5:     ");
  printFixed128(&a, "1.5");
  putchar('\n');
  Fixed128_str2(&a, "-1.1", NULL);
  printf("-1.1:    ");
  printFixed128(&a,
                "-1."
                "1000000000000000000000000000050487097934144755546350628178"
                "0983186990852118469774723052978515625");
  putchar('\n');
  Fixed128_str2(&a,
                "0."
                "00000000000000000000000000001262177448353618888658765704452457"
                "9674771302961744368076324462890625",
                NULL);
  printf("2⁻⁹⁶:    ");
  printFixed128(&a,
                "0."
                "0000000000000000000000000000126217744835361888865876570445"
                "24579674771302961744368076324462890625");
  putchar('\n');
  Fixed128_str2(&a,
                "2147483647."
                "00000000000000000000000000001262177448353618888658765704452457"
                "9674771302961744368076324462890625",
                NULL);
  printf("2³¹-1+2⁻⁹⁶:");
  printFixed128(&a,
                "2147483647."
                "0000000000000000000000000000126217744835361888865876570445"
                "24579674771302961744368076324462890625");
  putchar('\n');
  Fixed128_str2(&a,
                "-2147483647."
                "00000000000000000000000000001262177448353618888658765704452457"
                "9674771302961744368076324462890625",
                NULL);
  printf("-(2³¹-1+2⁻⁹⁶):");
  printFixed128(&a,
                "-2147483647."
                "0000000000000000000000000000126217744835361888865876570445"
                "24579674771302961744368076324462890625");
  putchar('\n');
  Fixed128_str2(&a, "1e4", NULL);
  printf("1e4      ");
  printFixed128(&a, "10000");
  putchar('\n');
  Fixed128_str2(&a, "5e-1", NULL);
  printf("5e-1    ");
  printFixed128(&a, "0.5");
  putchar('\n');

#if HAVE_ASM_FIXED128_ITERATE
  // Mandelbrot computation
  Fixed128_int2(&a, 0);
  Fixed128_int2(&b, 0);
  memset(&c, 0, sizeof c);
  c.word[NFIXED128 - 1] = 0;
  c.word[NFIXED128 - 2] = 0xa6aaaaaa;
  c.word[NFIXED128 - 3] = 0xaaaaaaaa;
  c.word[NFIXED128 - 4] = 0xaaaaaaab;
  printf("cx:      ");
  printFixed128(&c,
                "0."
                "6510416666666666666666666666708739248278453962955292190148"
                "41526558257100987248122692108154296875");
  putchar('\n');
  memset(&d, 0, sizeof d);
  d.word[NFIXED128 - 1] = 0xffffffff;
  d.word[NFIXED128 - 2] = 0xfd555555;
  d.word[NFIXED128 - 3] = 0x55555555;
  d.word[NFIXED128 - 4] = 0x55555555;
  printf("cy:      ");
  printFixed128(&d,
                "-0."
                "0104166666666666666666666666708739248278453962955292190148"
                "41526558257100987248122692108154296875");
  putchar('\n');
  count = Fixed128_iterate(&a, &b, &c, &d, 255);
  printf("iterate: %d\n", count);
  assert(count == 4);
  printf("r2:      ");
  printFixed128(&a,
                "15.32360374487664264857127912540696759670677650117087795385295567740513433818705379962921142578125");
  putchar('\n');

  Fixed128_int2(&a, 0);
  Fixed128_int2(&b, 0);
  Fixed128_int2(&c, -1);
  Fixed128_int2(&d, -1);
  printf("cx:      ");
  printFixed128(&c, "-1");
  putchar('\n');
  printf("cy:      ");
  printFixed128(&d, "-1");
  putchar('\n');
  count = Fixed128_iterate(&a, &b, &c, &d, 255);
  if(count != 3) {
    printf("-- EXPECTED 3\n");
    ++errors;
  }
  printf("r2:      ");
  printFixed128(&a, "10");
  putchar('\n');
#endif

  printf("%d errors\n", errors);
  return !!errors;
}
