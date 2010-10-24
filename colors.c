#include "mand.h"
#include <math.h>

// Color lookup table
struct color colors[MAXITER + 1];

void init_colors(void) {
  // The complement is colorful
  for(int n = 0; n < MAXITER; ++n) {
    colors[n].r = (cos(2 * M_PI * (double)n / MAXITER) + 1.0) * 127;
    colors[n].g = 255 - (cos(4 * M_PI * (double)n / MAXITER) + 1.0) * 127;
    colors[n].b = 255 - (cos(8 * M_PI * (double)n / MAXITER) + 1.0) * 127;
  }
  // The set itself is black
  colors[MAXITER].r = colors[MAXITER].g = colors[MAXITER].b = 0;
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
