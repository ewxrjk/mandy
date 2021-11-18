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
#ifndef DRAW_H
#define DRAW_H

void draw(const char *wstr, const char *hstr, const char *xstr,
          const char *ystr, const char *rstr, const char *mistr,
          const char *path);

int dive(const char *wstr, const char *hstr, const char *sxstr,
         const char *systr, const char *srstr, const char *exstr,
         const char *eystr, const char *erstr, const char *mistr,
         const char *frstr, const char *path);

int draw(int width, int height, arith_t x, arith_t y, arith_t r, int maxiters,
         arith_type arith, FILE *fp, const char *fileType = "png");

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
