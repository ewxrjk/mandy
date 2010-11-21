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
#include "mandy.h"
#include "arith.h"
#include "Draw.h"
#include "MandelbrotJob.h"
#include "Color.h"
#include <gdkmm/pixbuf.h>

void draw(const char *wstr,
	  const char *hstr,
	  const char *xstr,
          const char *ystr,
          const char *rstr,
          const char *mistr,
          const char *path) {
  arith_t x, y, radius;
  long width, height, maxiters;
  char *eptr;
  int error;

  errno = 0;
  width = strtol(wstr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", wstr);
  if(eptr == rstr)
    fatal(0, "cannot convert '%s'", wstr);
  if(width > INT_MAX || width <= 0)
    fatal(0, "cannot convert '%s': out of range", wstr);

  errno = 0;
  height = strtol(hstr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", hstr);
  if(eptr == rstr)
    fatal(0, "cannot convert '%s'", hstr);
  if(height > INT_MAX || height <= 0)
    fatal(0, "cannot convert '%s': out of range", hstr);

  if((error = arith_traits<arith_t>::fromString(x, xstr, &eptr)))
    fatal(error, "cannot convert '%s'", xstr);
  if(eptr == xstr)
    fatal(0, "cannot convert '%s'", xstr);

  if((error = arith_traits<arith_t>::fromString(y, ystr, &eptr)))
    fatal(error, "cannot convert '%s'", ystr);
  if(eptr == ystr)
    fatal(0, "cannot convert '%s'", ystr);

  if((error = arith_traits<arith_t>::fromString(radius, rstr, &eptr)))
    fatal(error, "cannot convert '%s'", rstr);
  if(eptr == rstr)
    fatal(0, "cannot convert '%s'", rstr);
  if(radius <= arith_t(0))
    fatal(0, "cannot convert '%s': too small", rstr);

  errno = 0;
  maxiters = strtol(mistr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", mistr);
  if(eptr == rstr)
    fatal(0, "cannot convert '%s'", mistr);
  if(maxiters > INT_MAX || maxiters <= 0)
    fatal(0, "cannot convert '%s': out of range", mistr);

  draw(width, height, x, y, radius, maxiters, path);
}

static void completed(Job *, void *) {
  // do nothing
}

void draw(int width, int height, arith_t x, arith_t y, arith_t radius,
	  int maxiters, const char *path) {
  const char *ext = strchr(path, '.');
  if(!ext)
    fatal(0, "cannot figure out extension of '%s'", ext);
  const char *fileType = NULL;
  if(!strcasecmp(ext, ".png"))
    fileType = "png";
  else if(!strcasecmp(ext, ".jpg") || !strcasecmp(ext, ".jpeg"))
    fileType = "jpeg";
  else if(!strcasecmp(ext, ".ppm"))
    fileType = "ppm";
  else
    fatal(0, "unknown file tyep '%s'", ext);
  MandelbrotJobFactory jf;
  IterBuffer *dest = FractalJob::recompute(x, y, radius, maxiters,
					   width, height,
					   completed,
					   NULL,
					   0, 0, &jf);
  Job::pollAll();
  // Convert to a pixbuf
  // TODO de-dupe with View::Completed
  Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, width, height);
  const int rowstride = pixbuf->get_rowstride();
  guint8 *pixels = pixbuf->get_pixels();
  for(int py = 0; py < height; ++py) {
    const count_t *datarow = &dest->data[py * width];
    guchar *pixelrow = pixels + py * rowstride;
    for(int px = 0; px < width; ++px) {
      const count_t count = *datarow++;
      if(count < maxiters) {
	*pixelrow++ = red(count, maxiters);
	*pixelrow++ = green(count, maxiters);
	*pixelrow++ = blue(count, maxiters);
      } else {
	*pixelrow++ = 0;
	*pixelrow++ = 0;
	*pixelrow++ = 0;
      }
    }
  }
  // Write to a file
  if(!strcmp(fileType, "ppm")) {
    FILE *fp = fopen(path, "wb");
    if(!fp)
      fatal(errno, "opening %s", path);
    if(fprintf(fp, "P6\n%d %d 255\n", width, height) < 0)
      fatal(errno, "writing %s", path);
    for(int py = 0; py < height; ++py) {
      guchar *pixelrow = pixels + py * rowstride;
      fwrite(pixelrow, 3, width, fp);
      if(ferror(fp))
	fatal(errno, "writing %s", path);
    }
    if(fclose(fp) < 0)
      fatal(errno, "closing %s", path);
  } else
    pixbuf->save(path, fileType);
}

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
