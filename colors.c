#include "mand.h"
#include <math.h>
#include <stdlib.h>

// Color lookup table
struct color *colors;

// Maximum iteration count
int maxiter;

void init_colors(int new_maxiter) {
  if(maxiter == new_maxiter)
    return;
  maxiter = new_maxiter;
  if(colors)
    free(colors);
  colors = malloc((maxiter + 1) * sizeof *colors);
  // The complement is colorful
  for(int n = 0; n < maxiter; ++n) {
    colors[n].r = (cos(2 * M_PI * (double)n / maxiter) + 1.0) * 127;
    colors[n].g = 255 - (cos(4 * M_PI * (double)n / maxiter) + 1.0) * 127;
    colors[n].b = 255 - (cos(8 * M_PI * (double)n / maxiter) + 1.0) * 127;
  }
  // The set itself is black
  colors[maxiter].r = colors[maxiter].g = colors[maxiter].b = 0;
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
