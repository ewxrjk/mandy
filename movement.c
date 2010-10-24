#include "mand.h"
#include <math.h>

// Perform a drag (in pixel units)
void drag(int w, int h, int deltax, int deltay) {
  if(w > h) {
    xcenter -= deltax * size * 2 / h;
    ycenter += deltay * size * 2 / h;
  } else {
    xcenter -= deltax * size * 2 / w;
    ycenter += deltay * size * 2 / w;
  }
}

// Perform a zoom
void zoom(int w, int h, int x, int y) {
  xcenter = xposition(w, h, x);
  ycenter = yposition(w, h, y);
  size = size * sqrt(0.5);
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
