#include "mand.h"

/* How locations and sizes are treated:
 *
 * PIXEL UNITS
 * ===========
 *
 * These are (usually) integer valued and locate pixels on the screen.
 * They are also therefore the units of the mouse pointer.  (0,0) is
 * the top left.
 *
 * PLANE UNITS
 * ===========
 *
 * These identify a location in the complex plane.  It is represented
 * in the conventional way, i.e. with increasing real (X) values
 * proceeding from left to right and increasing imaginary (Y) values
 * proceeding from bottom to top.
 *
 * The objects x and y give the center of the window in plane units
 * and the object size gives the radius of the largest displayable
 * circle, i.e. the distance parallel to the real or imaginary axis
 * from the center to the nearest window edge.  The reason for this is
 * to produce sensible behavior as the window is resized.
 *
 * THE ITERS[] ARRAY
 * =================
 *
 * This is addressed in pixel units BUT is reversed vertically.
 * Values within one row proceed from left to right as addresses
 * increase but the lowest-addressed row belongs at the bottom of the
 * screen and corresponds to the lower imaginary (Y) value in plane
 * units.
 */

// Location of the center of the window
double xcenter = 0.0;
double ycenter = 0.0;

// Distance to the nearest edge
double size = 2.0;

// Get the width of the window in plane units
double xsize(int w, int h) {
  if(w > h)
    return size * w / h;
  else
    return size;
}

// Get the height of the window in plane units
double ysize(int w, int h) {
  if(w > h)
    return size;
  else
    return size * h / w;
}

// Get the left (least real) boundary of the window in plane units
double xleft(int w, int h) {
  if(w > h)
    return xcenter - size * w / h;
  else
    return xcenter - size;
}

// Get the lower (least imaginary) boundary of the window in plane units
double ybottom(int w, int h) {
  if(w > h)
    return ycenter - size;
  else
    return ycenter - size * h / w;
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
