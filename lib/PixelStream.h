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
#ifndef PIXELSTREAM_H
#define PIXELSTREAM_H

class PixelStream {
public:
  // Return the next pixel. Return false if there are no more.
  // px/py are always set to a valid pixel even if the return is false.
  virtual bool next(int &px, int &py) = 0;

  inline bool morepixels(int count, int *px, int *py) {
    bool more = false;
    for(int i = 0; i < count; i++)
      more |= next(px[i], py[i]);
    return more;
  }
};

class PixelStreamRectangle: public PixelStream {
public:
  PixelStreamRectangle(int x, int y, int w, int h):
      m_min_x(x), m_min_y(y), m_limit_x(x + w), m_limit_y(y + h), m_px(x),
      m_py(y) {}
  bool next(int &px, int &py) override {
    if(m_py >= m_limit_y) {
      px = m_min_x;
      py = m_min_y;
      return false;
    }
    px = m_px;
    py = m_py;
    m_px++;
    if(m_px >= m_limit_x) {
      m_px = m_min_x;
      m_py++;
    }
    return true;
  }

private:
  int m_min_x, m_min_y, m_limit_x, m_limit_y;
  int m_px, m_py;
};

class PixelStreamEdge: public PixelStream {
public:
  PixelStreamEdge(int x, int y, int w, int h):
      m_min_x(x), m_min_y(y), m_limit_x(x + w - 1), m_limit_y(y + h - 1),
      m_px(x), m_py(y) {}
  bool next(int &px, int &py) override {
    if(m_edge >= 4) {
      px = m_min_x;
      py = m_min_y;
      return false;
    }
    px = m_px;
    py = m_py;
    switch(m_edge) {
    case 0:
      m_px++;
      if(m_px >= m_limit_x)
        m_edge++;
      break;
    case 1:
      m_py++;
      if(m_py >= m_limit_y)
        m_edge++;
      break;
    case 2:
      m_px--;
      if(m_px <= m_min_x)
        m_edge++;
      break;
    case 3:
      m_py--;
      if(m_py <= m_min_y)
        m_edge++;
      break;
    }
    return true;
  }

private:
  int m_min_x, m_min_y, m_limit_x, m_limit_y;
  int m_px, m_py, m_edge = 0;
};
#endif /* PIXELSTREAM_H */

/*
Local Variables
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
