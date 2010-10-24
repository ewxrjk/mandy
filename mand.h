#ifndef MAND_H
#define MAND_H

#define MAXITER 255

void fatal(int errno_value, const char *fmt, ...);
void init_threads(void);
void destroy_threads(void);
void mand(double x, double y, double size, int xpixels, int ypixels,
	  int *results);

#endif /* MAND_H */

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
