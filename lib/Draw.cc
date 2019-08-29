/* Copyright © 2010, 2012 Richard Kettlewell.
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
#include "Shell.h"
#include <gdkmm/pixbuf.h>

#ifndef DEFAULT_FFMPEG
# define DEFAULT_FFMPEG "ffmpeg"
#endif

#define TMP_PATTERN "tmp%d.png"

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
  if(eptr == wstr)
    fatal(0, "cannot convert '%s'", wstr);
  if(width > INT_MAX || width <= 0)
    fatal(0, "cannot convert '%s': out of range", wstr);

  errno = 0;
  height = strtol(hstr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", hstr);
  if(eptr == hstr)
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
  if(eptr == mistr)
    fatal(0, "cannot convert '%s'", mistr);
  if(maxiters > INT_MAX || maxiters <= 0)
    fatal(0, "cannot convert '%s': out of range", mistr);

  draw(width, height, x, y, radius, maxiters, ARITH_DEFAULT, path);
}

static void completed(Job *, void *) {
  // do nothing
}

void draw(int width, int height, arith_t x, arith_t y, arith_t radius,
	  int maxiters, arith_type arith, const char *path) {
  const char *ext = strchr(path, '.');
  if(!ext)
    fatal(0, "cannot figure out extension of '%s'", path);
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
                                           arith,
					   completed,
					   &jf,
					   0, 0, &jf);
  Job::poll(&jf);
  // Write to a file
  if(!strcmp(fileType, "ppm")) {
    /* PPMs can be written directly */
    FILE *fp = fopen(path, "wb");
    if(!fp)
      fatal(errno, "opening %s", path);
    if(fprintf(fp, "P6\n%d %d 255\n", width, height) < 0)
      fatal(errno, "writing %s", path);
    for(int py = 0; py < height; ++py) {
      const count_t *datarow = &dest->data[py * width];
      for(int px = 0; px < width; ++px) {
        const count_t count = *datarow++;
        int r, g, b;
        if(count < maxiters) {
          r = red(count, maxiters);
          g = green(count, maxiters);
          b = blue(count, maxiters);
        } else
          r = g = b = 0;
        if(fprintf(fp, "%c%c%c", r, g, b) < 0)
          fatal(errno, "writing %s", path);
      }
    }
    if(fclose(fp) < 0)
      fatal(errno, "closing %s", path);
  } else {
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
    pixbuf->save(path, fileType);
  }
  dest->release();
}

static std::string get_default(const char *name, 
                               const std::string &default_value) {
  const char *value = getenv(name);
  return value ? value : default_value;
}

int dive(const char *wstr,
         const char *hstr,
         const char *sxstr,
         const char *systr,
         const char *srstr,
         const char *exstr,
         const char *eystr,
         const char *erstr,
         const char *mistr,
         const char *frstr,
         const char *path) {
  arith_t sx, sy, ex, ey, sr, er;
  long width, height, maxiters, frames;
  char *eptr;
  int error;

  errno = 0;
  width = strtol(wstr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", wstr);
  if(eptr == wstr)
    fatal(0, "cannot convert '%s'", wstr);
  if(width > INT_MAX || width <= 0)
    fatal(0, "cannot convert '%s': out of range", wstr);

  errno = 0;
  height = strtol(hstr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", hstr);
  if(eptr == hstr)
    fatal(0, "cannot convert '%s'", hstr);
  if(height > INT_MAX || height <= 0)
    fatal(0, "cannot convert '%s': out of range", hstr);

  if((error = arith_traits<arith_t>::fromString(sx, sxstr, &eptr)))
    fatal(error, "cannot convert '%s'", sxstr);
  if(eptr == sxstr)
    fatal(0, "cannot convert '%s'", sxstr);

  if((error = arith_traits<arith_t>::fromString(sy, systr, &eptr)))
    fatal(error, "cannot convert '%s'", systr);
  if(eptr == systr)
    fatal(0, "cannot convert '%s'", systr);

  if((error = arith_traits<arith_t>::fromString(sr, srstr, &eptr)))
    fatal(error, "cannot convert '%s'", srstr);
  if(eptr == srstr)
    fatal(0, "cannot convert '%s'", srstr);
  if(sr <= arith_t(0))
    fatal(0, "cannot convert '%s': too small", erstr);

  if((error = arith_traits<arith_t>::fromString(ex, exstr, &eptr)))
    fatal(error, "cannot convert '%s'", exstr);
  if(eptr == exstr)
    fatal(0, "cannot convert '%s'", exstr);

  if((error = arith_traits<arith_t>::fromString(ey, eystr, &eptr)))
    fatal(error, "cannot convert '%s'", eystr);
  if(eptr == eystr)
    fatal(0, "cannot convert '%s'", eystr);

  if((error = arith_traits<arith_t>::fromString(er, erstr, &eptr)))
    fatal(error, "cannot convert '%s'", erstr);
  if(eptr == erstr)
    fatal(0, "cannot convert '%s'", erstr);
  if(er <= arith_t(0))
    fatal(0, "cannot convert '%s': too small", erstr);

  errno = 0;
  maxiters = strtol(mistr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", mistr);
  if(eptr == mistr)
    fatal(0, "cannot convert '%s'", mistr);
  if(maxiters > INT_MAX || maxiters <= 0)
    fatal(0, "cannot convert '%s': out of range", mistr);

  errno = 0;
  frames = strtol(frstr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", frstr);
  if(eptr == frstr)
    fatal(0, "cannot convert '%s'", frstr);
  if(frames > INT_MAX || frames <= 0)
    fatal(0, "cannot convert '%s': out of range", frstr);

  double rk = pow(arith_traits<arith_t>::toDouble(er / sr), 1.0/(frames - 1));
  for(int frame = 0; frame < frames; ++frame) {
    arith_t radius = sr * pow(rk, frame);
    arith_t x = sx + arith_t(frame) * (ex - sx) / (frames - 1);
    arith_t y = sy + arith_t(frame) * (ey - sy) / (frames - 1);
    fprintf(stderr, "frame %d/%ld centre %s x %s radius %s\n",
            frame + 1, frames,
            arith_traits<arith_t>::toString(x).c_str(),
            arith_traits<arith_t>::toString(y).c_str(),
            arith_traits<arith_t>::toString(radius).c_str());
    char tmp[1024];
    sprintf(tmp, TMP_PATTERN, frame);
    draw(width, height, x, y, radius, maxiters, ARITH_DEFAULT, tmp);
  }
  std::string command;
  command += shellQuote(get_default("FFMPEG", ffmpegDefault()));
  command += " -f image2 -i " TMP_PATTERN;
  command += " -vcodec ";
  command += get_default("CODEC", "mpeg4"); 
  command += " -r ";
  command += get_default("FRAME_RATE", "25");
  command += " -b ";
  command += get_default("BITRATE", "2M");
  command += " ";
  command += shellQuote(path);
  fprintf(stderr, "encoding with: %s\n", command.c_str());
  remove(path);
  int rc = system(command.c_str());
  for(int frame = 0; frame < frames; ++frame) {
    char tmp[1024];
    sprintf(tmp, TMP_PATTERN, frame);
    remove(tmp);
  }
  return rc;
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
