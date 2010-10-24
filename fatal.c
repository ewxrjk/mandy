/* Copyright © 2010 Richard Kettlewell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
