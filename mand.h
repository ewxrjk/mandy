#ifndef MAND_H
#define MAND_H

#define _GNU_SOURCE 1

#define MAXITER 255

struct color {
  unsigned char r, g, b;
};

void fatal(int errno_value, const char *fmt, ...);
void init_threads(void);
void destroy_threads(void);
int *compute(double x, double y, double xsize, int w, int h);
void init_colors(void);
double xsize(int w, int h);
double ysize(int w, int h);
double xleft(int w, int h);
double ybottom(int w, int h);
double xposition(int w, int h, int x);
double yposition(int w, int h, int y);
void drag(int w, int h, int deltax, int deltay);
void zoom(int w, int h, int x, int y);

extern struct color colors[];
extern double xcenter, ycenter, size;

#endif /* MAND_H */

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
