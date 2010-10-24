#ifndef MAND_H
#define MAND_H

#define MAXITER 255

struct color {
  unsigned char r, g, b;
};

void fatal(int errno_value, const char *fmt, ...);
void init_threads(void);
void destroy_threads(void);
void mand(double x, double y, double xsize, int xpixels, int ypixels,
	  int *results);
void init_colors(void);

extern struct color colors[];

#endif /* MAND_H */

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
