#include "mand.h"

// Location of the centre of the window
double x = 0.0;
double y = 0.0;

// Distance to the nearest edge
double size = 2.0;

double xsize(int w, int h) {
  if(w > h)
    return size * w / h;
  else
    return size;
}

double ysize(int w, int h) {
  if(w > h)
    return size;
  else
    return size * h / w;
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
