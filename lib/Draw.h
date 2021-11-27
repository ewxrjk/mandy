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
#ifndef DRAW_H
#define DRAW_H

#ifndef DEFAULT_FFMPEG
#define DEFAULT_FFMPEG "ffmpeg"
#endif

void draw(const char *wstr, const char *hstr, const char *xstr,
          const char *ystr, const char *rstr, const char *mistr,
          const char *path);

int dive(const char *wstr, const char *hstr, const char *sxstr,
         const char *systr, const char *srstr, const char *exstr,
         const char *eystr, const char *erstr, const char *mistr,
         const char *secstr, const char *path);

int draw(int width, int height, arith_t x, arith_t y, arith_t r, int maxiters,
         arith_type arith, FILE *fp, const char *fileType = "png");

class RenderMovie {
public:
  arith_t sx = 0;
  arith_t sy = 0;
  arith_t sr = 2;
  arith_t ex = 0;
  arith_t ey = 0;
  arith_t er = 0.125;
  int maxiters = 255;
  int width = 1280;
  int height = 720;
  int seconds = 10;
  int fps = 25;
  int bitrate = 8 * 1024 * 1024;
  arith_type arith = ARITH_DEFAULT;
  std::string ffmpeg = DEFAULT_FFMPEG;
  std::string codec = "libx264";
  std::string path = "mandy.mp4";

  int Render();

  virtual void Progress(const std::string &msg, bool completed = false);
};

#endif /* DRAW_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
