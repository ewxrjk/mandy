#include "mandy.h"
#include "Fixed128.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

static int errors;

static void printFixed128(const struct Fixed128 *f, const char *expect) {
  char buffer[128];
  int n;
  printf("%08x.%08x", f->word[NFIXED128-1], f->word[NFIXED128-2]);
  for(n = NFIXED128-3; n >= 0; --n)
    printf(" %08x", f->word[n]);
  printf(" =%s", Fixed128_2str(buffer, sizeof buffer, f, 10));
  if(strcmp(buffer, expect)) {
    printf("<EXPECTED %s>", expect);
    ++errors;
  }
}

int main() {
  struct Fixed128 a, b, c, d;
  double x;
#if HAVE_ASM_128
  int count;
#endif

  // Divide by 2 across word boundary...
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  printf("1/2:     "); printFixed128(&a, "0.5"); putchar('\n');
  // ...and within one word
  Fixed128_divu(&a, &a, 4);
  printf("0.5/4:   "); printFixed128(&a, "0.125"); putchar('\n');

  // Add across word boundary
  Fixed128_int2(&b, 1);
  Fixed128_add(&c, &a, &b);
  printf("1+0.125: "); printFixed128(&c, "1.125"); putchar('\n');

  // Add with carry
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  b = a;
  Fixed128_add(&c, &a, &b);
  printf("0.5+0.5: "); printFixed128(&c, "1"); putchar('\n');

  // Negation
  Fixed128_int2(&a, 10);
  Fixed128_neg(&b, &a);
  Fixed128_neg(&c, &b);
  printf("-10:     "); printFixed128(&b, "-10"); putchar('\n');
  printf("-(-10):  "); printFixed128(&c, "10"); putchar('\n');

  // Subtraction
  Fixed128_int2(&a, 1);
  Fixed128_int2(&b, 10);
  Fixed128_sub(&c, &b, &a);
  printf("10-1:    "); printFixed128(&c, "9"); putchar('\n');
  Fixed128_sub(&c, &a, &b);
  printf("1-10:    "); printFixed128(&c, "-9"); putchar('\n');

  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 16);
  Fixed128_int2(&b, 1);
  Fixed128_divu(&b, &b, 256);
  Fixed128_sub(&c, &a, &b);
  printf("15/256:  "); printFixed128(&c, "0.05859375"); putchar('\n');

  Fixed128_int2(&a, 0);
  b = a;
  a.word[NFIXED128-1] = 0x4321;
  a.word[NFIXED128-2] = 0x5678abcd;
  a.word[NFIXED128-3] = 0xef012345;
  b.word[NFIXED128-1] = 0x1234;
  b.word[NFIXED128-2] = 0xabcd5678;
  b.word[NFIXED128-3] = 0x12345678;
  printf("a:       "); printFixed128(&a, "17185.3377787950294118167406283437248504242234048433601856231689453125"); putchar('\n');
  printf("b:       "); printFixed128(&b, "4660.6711019557134972677962803100371047548833303153514862060546875"); putchar('\n');
  Fixed128_sub(&c, &a, &b);
  printf("a-b:     "); printFixed128(&c, "12524.6666768393159145489443480336877456693400745280086994171142578125"); putchar('\n');
  Fixed128_sub(&c, &b, &a);
  printf("b-a:     "); printFixed128(&c, "-12524.6666768393159145489443480336877456693400745280086994171142578125"); putchar('\n');

  // Multiply
  Fixed128_int2(&a, 5);
  Fixed128_int2(&b, 7);
  Fixed128_mul(&c, &a, &b);
  printf("5*7:     "); printFixed128(&c, "35"); putchar('\n');

  // Multiply by 0.5
  Fixed128_int2(&a, 101);
  Fixed128_int2(&b, 1);
  Fixed128_divu(&b, &b, 2);
  Fixed128_mul(&c, &a, &b);
  printf("101*0.5  "); printFixed128(&c, "50.5"); putchar('\n');

  // Multiply with nontrivial sign
  Fixed128_int2(&a, -10);
  Fixed128_int2(&b, -5);
  Fixed128_mul(&c, &a, &b);
  printf("-10*-5:  "); printFixed128(&c, "50"); putchar('\n');
  Fixed128_int2(&a, -10);
  Fixed128_int2(&b, 5);
  Fixed128_mul(&c, &a, &b);
  printf("-10*5:   "); printFixed128(&c, "-50"); putchar('\n');

  // Multiply regression
  memset(&a, 0, sizeof a);
  memset(&b, 0, sizeof b);
  a.word[NFIXED128-1] = 0x00000000;
  a.word[NFIXED128-2] = 0x95f61998;
  a.word[NFIXED128-3] = 0x0c433000;
  a.word[NFIXED128-4] = 0x00000000;	/* about 0.5857864... */
  b.word[NFIXED128-1] = 0xffffffff;
  b.word[NFIXED128-2] = 0xfaaaaaaa;
  b.word[NFIXED128-3] = 0xaaaaaaaa;
  b.word[NFIXED128-4] = 0xaaaaaaaa;	/* about -0.02083333... */
  printf("a:       "); printFixed128(&a, "0.5857864376269048545253781412611715495586395263671875"); putchar('\n');
  printf("b:       "); printFixed128(&b, "-0.02083333333333333333333333334174784965569079259105843802968305311651420197449624538421630859375"); putchar('\n');
  Fixed128_mul(&c, &a, &b);
  printf("a*b:     "); printFixed128(&c, "-0.01220388411722718446927871127627440728247165679931640625"); putchar('\n');

  // Very small numbers
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 65536);
  Fixed128_divu(&a, &a, 65536);
  printf("2⁻³²:    "); printFixed128(&a, "0.00000000023283064365386962890625"); putchar('\n');
  b = a;
  Fixed128_mul(&c, &a, &b);
  printf("(2⁻³²)²: "); printFixed128(&c, "0.0000000000000000000542101086242752217003726400434970855712890625"); putchar('\n');

  // Even smaller than that
  Fixed128_mul(&d, &a, &c);
  printf("(2⁻³²)³: "); printFixed128(&d, "0.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625"); putchar('\n');

  // Underflow rounding
  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  Fixed128_mul(&c, &a, &d);
  printf("½(2⁻³²)³:"); printFixed128(&d, "0.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625"); putchar('\n');

  // Division
  Fixed128_int2(&a, 1);
  Fixed128_int2(&b, 16);
  Fixed128_div(&c, &a, &b);
  printf("1/16:    "); printFixed128(&c, "0.0625"); putchar('\n');

  Fixed128_int2(&a, 1);
  Fixed128_int2(&b, 3);
  Fixed128_div(&c, &a, &b);
  printf("1/3:     "); printFixed128(&c, "0.333333333333333333333333333329126075172154603704470780985158473441742899012751877307891845703125"); putchar('\n');

  Fixed128_div(&c, &c, &b);
  printf("1/3/3:   "); printFixed128(&c, "0.111111111111111111111111111109708691724051534568156926995052824480580966337583959102630615234375"); putchar('\n');

  // Square roots
  Fixed128_int2(&a, 2);
  Fixed128_sqrt(&a, &a);
  printf("√2:      "); printFixed128(&a, "1.414213562373095048801688724210753971735753910576858008961431689698429181589744985103607177734375"); putchar('\n');
  Fixed128_mul(&b, &a, &a);
  printf("(√2)²:   "); printFixed128(&b, "2"); putchar('\n');

  Fixed128_int2(&a, 1);
  Fixed128_divu(&a, &a, 2);
  Fixed128_sqrt(&a, &a);
  printf("√½:      "); printFixed128(&a, "0.707106781186547524400844362099066098626108860845135175958453555011828939314000308513641357421875"); putchar('\n');
  Fixed128_mul(&b, &a, &a);
  printf("(√½)²:   "); printFixed128(&b, "0.499999999999999999999999999987378225516463811113412342955475420325228697038255631923675537109375"); putchar('\n');

  // Conversion to/from double
  Fixed128_double2(&a, 1.0);
  printf("1.0:     "); printFixed128(&a, "1"); putchar('\n');
  x = Fixed128_2double(&a);
  printf("and back: %g\n", x);
  if(x != 1) {
    printf("-- EXPECTED 1\n");
    ++errors;
  }
  Fixed128_double2(&a, 0.5);
  printf("0.5:     "); printFixed128(&a, "0.5"); putchar('\n');
  x = Fixed128_2double(&a);
  printf("and back: %g\n", x);
  if(x != 0.5) {
    printf("-- EXPECTED 0.5\n");
    ++errors;
  }
  Fixed128_double2(&a, M_PI);
  printf("π:       "); printFixed128(&a, "3.141592653589793115997963468544185161590576171875"); putchar('\n');
  x = Fixed128_2double(&a);
  printf("and back: %g\n", x);
  if(x != M_PI) {
    printf("-- EXPECTED π\n");
    ++errors;
  }

  // Conversion from string
  Fixed128_str2(&a, "1.0", NULL);
  printf("1.0:     "); printFixed128(&a, "1"); putchar('\n');
  Fixed128_str2(&a, "1.5", NULL);
  printf("1.5:     "); printFixed128(&a, "1.5"); putchar('\n');
  Fixed128_str2(&a, "-1.1", NULL);
  printf("-1.1:    "); printFixed128(&a, "-1.10000000000000000000000000000504870979341447555463506281780983186990852118469774723052978515625"); putchar('\n');
  Fixed128_str2(&a, "0.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625", NULL);
  printf("2⁻⁹⁶:    "); printFixed128(&a, "0.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625"); putchar('\n');
  Fixed128_str2(&a, "2147483647.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625", NULL);
  printf("2³¹-1+2⁻⁹⁶:"); printFixed128(&a, "2147483647.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625"); putchar('\n');
  Fixed128_str2(&a, "-2147483647.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625", NULL);
  printf("-(2³¹-1+2⁻⁹⁶):"); printFixed128(&a, "-2147483647.000000000000000000000000000012621774483536188886587657044524579674771302961744368076324462890625"); putchar('\n');

#if HAVE_ASM_128
  // Mandelbrot computation
  Fixed128_int2(&a, 0);
  Fixed128_int2(&b, 0);
  memset(&c, 0, sizeof c);
  c.word[NFIXED128-1] = 0;
  c.word[NFIXED128-2] = 0xa6aaaaaa;
  c.word[NFIXED128-3] = 0xaaaaaaaa;
  c.word[NFIXED128-4] = 0xaaaaaaab;
  printf("cx:      "); printFixed128(&c, "0.651041666666666666666666666670873924827845396295529219014841526558257100987248122692108154296875"); putchar('\n');
  memset(&d, 0, sizeof d);
  d.word[NFIXED128-1] = 0xffffffff;
  d.word[NFIXED128-2] = 0xfd555555;
  d.word[NFIXED128-3] = 0x55555555;
  d.word[NFIXED128-4] = 0x55555555;
  printf("cy:      "); printFixed128(&d, "-0.010416666666666666666666666670873924827845396295529219014841526558257100987248122692108154296875"); putchar('\n');
  count = Fixed128_iterate(&a, &b, &c, &d, 255);
  printf("iterate: %d\n", count);
  assert(count == 5);
  printf("r2:      "); printFixed128(&a, "255.084714623168092182249097929488373379693706291486597300711969271702628248021937906742095947265625"); putchar('\n');

  Fixed128_int2(&a, 0);
  Fixed128_int2(&b, 0);
  Fixed128_int2(&c, -1);
  Fixed128_int2(&d, -1);
  printf("cx:      "); printFixed128(&c, "-1"); putchar('\n');
  printf("cy:      "); printFixed128(&d, "-1"); putchar('\n');
  count = Fixed128_iterate(&a, &b, &c, &d, 255);
  printf("iterate: %d\n", count);
  if(count != 4) {
    printf("-- EXPECTED 4\n");
    ++errors;
  }
  printf("r2:      "); printFixed128(&a, "106"); putchar('\n');
#endif

  printf("%d errors\n", errors);
  return !!errors;
}
