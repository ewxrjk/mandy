/* Copyright © 2010, 2015 Richard Kettlewell.
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
#ifndef ITERBUFFER_H
#define ITERBUFFER_H

/* A buffer for iteration counts.  This buffer is reference-counted so that it
 * can conveniently be shared between threads.  Moreover the reference-counting
 * is thread-safe, so acquire/release may be used without reference to
 * locking. */
class IterBuffer {
  ATOMIC_TYPE refs;
  void finished();
  ~IterBuffer();

  // The buffer size. xw is the width rounded up to a multiple of 8
  // (for the benefit of SIMD implementations.)
  int xw, w, h;
  // The actual data.
  count_t *data;

public:
  // Construct a new IterBuffer with a given size.  The initial refcount is 1.
  IterBuffer(int w, int h);
  // Acquire a reference.
  IterBuffer *acquire() {
    ATOMIC_INC(refs);
    return this;
  }
  // Release a reference.
  void release() {
    if(ATOMIC_DEC(refs) == 0)
      finished();
  }

  inline count_t &pixel(int x, int y) {
    return data[y * xw + x];
  }

  inline int width() const {
    return w;
  }
  inline int height() const {
    return h;
  }

  void clear() {
    memset(data, 0xFF, xw * h * sizeof(count_t));
  }
};

#endif /* ITERBUFFER_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
