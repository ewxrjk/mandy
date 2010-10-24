#include "mand.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Report an error, perhaps including an errno value, and terminate the
 * program.
 *
 * The format strings and additional arguments follow the same rules as printf.
 */
void fatal(int errno_value, const char *fmt, ...) {
  va_list ap;

  fprintf(stderr,"FATAL: ");
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  if(errno_value)
    fprintf(stderr, ": %s\n", strerror(errno_value));
  else
    fputc('\n', stderr);
  exit(1);
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
