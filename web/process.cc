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
#include "mfcgi.h"
#include "MandelbrotJob.h"
#include "Color.h"

void process_query(const char *query, GdkPixbufSaveFunc writer, gpointer wdata,
                   bool header) {
  IterBuffer *ib = NULL;
  GdkPixbuf *pixbuf = NULL;

  try {
    int t = arith_double;
    int w = 64, h = 64, m = 255;
    arith_t x = -2, y = -1.25, s = 2.5;
    const struct param_def params[] = {
        {"w", param_int_positive, &w}, {"h", param_int_positive, &h},
        {"m", param_int_positive, &m}, {"x", param_arith, &x},
        {"y", param_arith, &y},        {"s", param_arith_positive, &s},
        {"t", param_int, &t},          {0, 0, 0},
    };
    parse_query(query, params);
    if(t < 0 || t >= arith_limit)
      throw std::runtime_error("invalid arithmetic type");
    // TODO bounds checks
    if(m > 255)
      throw std::runtime_error("excessive iteration count");
    if(w > 256 || h > 256)
      throw std::runtime_error("excessive pixel size");

    ib = new IterBuffer(w, h);
    MandelbrotJob mj;
    mj.dest = ib->acquire();
    mj.xleft = x;
    mj.ybottom = y;
    mj.xsize = s;
    mj.maxiters = m;
    mj.x = mj.y = 0;
    mj.w = w;
    mj.h = h;
    mj.arith = static_cast<arith_type>(t);
    mj.work();

    if(!(pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h)))
      throw std::runtime_error("insufficient memory");

    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    for(int yy = 0; yy < h; ++yy) {
      for(int xx = 0; xx < w; ++xx) {
        count_t c = ib->pixel(xx, yy);
        int r, g, b;
        if(c < m) {
          r = red(c, m);
          g = green(c, m);
          b = blue(c, m);
        } else {
          r = g = b = 0;
        }
        pixels[3 * xx + 0] = r;
        pixels[3 * xx + 1] = g;
        pixels[3 * xx + 2] = b;
      }
      pixels += gdk_pixbuf_get_rowstride(pixbuf);
    }

    if(header) {
      static const char http_header[] =
          "Content-Type: image/png\r\n"
          "Cache-Control: s-maxage=3600,max-age=3600\r\n" // TODO could ask for
                                                          // more
          "\r\n";
      writer(http_header, strlen(http_header), NULL, wdata);
    }
    gboolean rc = gdk_pixbuf_save_to_callback(
        pixbuf, writer, wdata, "png", NULL, "compression", "9", (char *)NULL);
    if(!rc)
      throw std::runtime_error("gdk_pixbuf_save_to_callback failed");
    ib->release();
    ib = NULL;
    g_object_unref(pixbuf);
    pixbuf = NULL;

  } catch(std::runtime_error &e) {
    if(ib)
      ib->release();
    if(pixbuf)
      g_object_unref(pixbuf);
    throw;
  }
}
