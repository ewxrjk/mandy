/* Copyright © Richard Kettlewell.
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

void draw(const char *wstr, const char *hstr, const char *xstr,
          const char *ystr, const char *rstr, const char *mistr,
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

  FILE *fp;
  if(!(fp = fopen(path, "wb")))
    fatal(errno, "opening %s", path);
  if(draw(width, height, x, y, radius, maxiters, ARITH_DEFAULT, fp))
    fatal(errno, "writing %s", path);
  if(fclose(fp) < 0)
    fatal(errno, "writing %s", path);
}

static void completed(Job *, void *) {
  // do nothing
}

int draw(int width, int height, arith_t x, arith_t y, arith_t radius,
         int maxiters, arith_type arith, FILE *fp, const char *fileType) {
  MandelbrotJobFactory jf;
  IterBuffer *dest = FractalJob::recompute(
      x, y, radius, maxiters, width, height, arith, completed, &jf, 0, 0, &jf);
  Job::poll(&jf);
  // Write to a file
  if(!strcmp(fileType, "ppm")) {
    /* PPMs can be written directly */
    if(fprintf(fp, "P6\n%d %d 255\n", width, height) < 0) {
      perror("write error");
      return -1;
    }
    for(int py = 0; py < height; ++py) {
      const count_t *datarow = &dest->pixel(0, py);
      for(int px = 0; px < width; ++px) {
        const count_t count = *datarow++;
        int r, g, b;
        if(count < maxiters) {
          r = red(count, maxiters);
          g = green(count, maxiters);
          b = blue(count, maxiters);
        } else
          r = g = b = 0;
        if(fprintf(fp, "%c%c%c", r, g, b) < 0) {
          perror("write error");
          return -1;
        }
      }
    }
  } else {
    // Convert to a pixbuf
    // TODO de-dupe with View::Completed
    Glib::RefPtr<Gdk::Pixbuf> pixbuf =
        Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, width, height);
    const int rowstride = pixbuf->get_rowstride();
    guint8 *pixels = pixbuf->get_pixels();
    for(int py = 0; py < height; ++py) {
      const count_t *datarow = &dest->pixel(0, py);
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
    gchar *buffer;
    gsize buffer_size;
    pixbuf->save_to_buffer(buffer, buffer_size, fileType);
    fwrite(buffer, 1, buffer_size, fp);
    if(ferror(fp)) {
      perror("write error");
      return -1;
    }
    g_free(buffer);
  }
  dest->release();
  return 0;
}

static std::string get_default(const char *name,
                               const std::string &default_value) {
  const char *value = getenv(name);
  return value ? value : default_value;
}

int dive(const char *wstr, const char *hstr, const char *sxstr,
         const char *systr, const char *srstr, const char *exstr,
         const char *eystr, const char *erstr, const char *mistr,
         const char *secstr, const char *path) {
  RenderMovie rm;
  char *eptr;
  int error;

  errno = 0;
  rm.width = strtol(wstr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", wstr);
  if(eptr == wstr)
    fatal(0, "cannot convert '%s'", wstr);
  if(rm.width > INT_MAX || rm.width <= 0)
    fatal(0, "cannot convert '%s': out of range", wstr);

  errno = 0;
  rm.height = strtol(hstr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", hstr);
  if(eptr == hstr)
    fatal(0, "cannot convert '%s'", hstr);
  if(rm.height > INT_MAX || rm.height <= 0)
    fatal(0, "cannot convert '%s': out of range", hstr);

  if((error = arith_traits<arith_t>::fromString(rm.sx, sxstr, &eptr)))
    fatal(error, "cannot convert '%s'", sxstr);
  if(eptr == sxstr)
    fatal(0, "cannot convert '%s'", sxstr);

  if((error = arith_traits<arith_t>::fromString(rm.sy, systr, &eptr)))
    fatal(error, "cannot convert '%s'", systr);
  if(eptr == systr)
    fatal(0, "cannot convert '%s'", systr);

  if((error = arith_traits<arith_t>::fromString(rm.sr, srstr, &eptr)))
    fatal(error, "cannot convert '%s'", srstr);
  if(eptr == srstr)
    fatal(0, "cannot convert '%s'", srstr);
  if(rm.sr <= arith_t(0))
    fatal(0, "cannot convert '%s': too small", erstr);

  if((error = arith_traits<arith_t>::fromString(rm.ex, exstr, &eptr)))
    fatal(error, "cannot convert '%s'", exstr);
  if(eptr == exstr)
    fatal(0, "cannot convert '%s'", exstr);

  if((error = arith_traits<arith_t>::fromString(rm.ey, eystr, &eptr)))
    fatal(error, "cannot convert '%s'", eystr);
  if(eptr == eystr)
    fatal(0, "cannot convert '%s'", eystr);

  if((error = arith_traits<arith_t>::fromString(rm.er, erstr, &eptr)))
    fatal(error, "cannot convert '%s'", erstr);
  if(eptr == erstr)
    fatal(0, "cannot convert '%s'", erstr);
  if(rm.er <= arith_t(0))
    fatal(0, "cannot convert '%s': too small", erstr);

  errno = 0;
  rm.maxiters = strtol(mistr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", mistr);
  if(eptr == mistr)
    fatal(0, "cannot convert '%s'", mistr);
  if(rm.maxiters > INT_MAX || rm.maxiters <= 0)
    fatal(0, "cannot convert '%s': out of range", mistr);

  errno = 0;
  rm.seconds = strtol(secstr, &eptr, 10);
  if(errno)
    fatal(errno, "cannot convert '%s'", secstr);
  if(eptr == secstr)
    fatal(0, "cannot convert '%s'", secstr);
  if(rm.seconds > INT_MAX || rm.seconds <= 0)
    fatal(0, "cannot convert '%s': out of range", secstr);

  rm.ffmpeg = get_default("FFMPEG", ffmpegDefault());
  rm.codec = get_default("CODEC", "libx264");
  rm.fps = atoi(get_default("FRAME_RATE", "25").c_str());
  rm.bitrate = atoi(get_default("BITRATE", "2097152").c_str());
  rm.path = path;

  return rm.Render();
}

int RenderMovie::Render(int *cancel) {
  const int frames = seconds * fps;
  double rk = pow(arith_traits<arith_t>::toDouble(er / sr), 1.0 / (frames - 1));
  std::stringstream command, pstream;
  // Construct the command
  std::string extras;
  if(codec.find("264") != std::string::npos) {
    // see e.g. https://bugzilla.mozilla.org/show_bug.cgi?id=1368063
    extras = "-pix_fmt yuv420p";
  }
  command << shellQuote(ffmpeg)
          << " -f image2pipe" // input format is concatenated image file demuxer
          << " -i pipe:0"     // input fil
          << " -vcodec " << shellQuote(codec) // output file codec
          << " -r " << fps                    // frame rate
          << " -b:v " << bitrate              // bit rate
          << " " << extras                    // extra options
          << " " << shellQuote(path);         // output file
  // Report the encoder command
  pstream << "Encoding with " << ffmpeg;
  Progress(pstream.str());
  // Run the encoder
  ::remove(path.c_str());
  fprintf(stderr, "%s\n", command.str().c_str());
  FILE *fp = popen(command.str().c_str(), "w");
  if(!fp) {
    perror("popen");
    Progress("Executing ffmpeg failed");
    return -1;
  }
  // TODO capture ffmpeg stderr and put it somewhere useful
  // (maybe the progress report should be a larger window)
  // Render PNGs to the pipe
  for(int frame = 0; frame < frames && (!cancel || !ATOMIC_GET(*cancel));
      ++frame) {
    std::stringstream pstream;
    pstream << "Frame " << frame << "/" << frames;
    Progress(pstream.str());
    arith_t radius = sr * pow(rk, frame);
    arith_t x = sx + arith_t(frame) * (ex - sx) / (frames - 1);
    arith_t y = sy + arith_t(frame) * (ey - sy) / (frames - 1);
    if(draw(width, height, x, y, radius, maxiters, arith, fp) < 0) {
      Progress("Encoding failed");
      pclose(fp);
      ::remove(path.c_str());
      return -1;
    }
  }
  // Finish
  int rc = pclose(fp);
  fprintf(stderr, "encoder: pclose: %d\n", rc);
  if(rc) {
    ::remove(path.c_str());
    Progress("Encoding failed", true);
    return -1;
  } else if(cancel && ATOMIC_GET(*cancel)) {
    Progress("Encoding cancelled", true);
    ::remove(path.c_str());
    return 0;
  } else {
    Progress("Encoding complete", true);
    return 0;
  }
}

void RenderMovie::Progress(const std::string &msg, bool) {
  fprintf(stderr, "%s\n", msg.c_str());
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
