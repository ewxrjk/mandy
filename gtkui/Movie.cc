/* Copyright © 2012 Richard Kettlewell.
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
#include "mmui.h"
#include "arith.h"
#include "Movie.h"
#include "Threading.h"
#include "Draw.h"
#include "Shell.h"
#include "Job.h"
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/stock.h>
#include <gtkmm/window.h>

namespace mmui {

  class MovieWindow;

  // Controls for movie rendering

  class MovieControls: public ControlContainer {
  public:
    MovieWindow *m_window;
    arith_t m_x, m_y, m_radius;
    int m_maxiters, m_seconds, m_fps;
    int m_bitrate;
    std::string m_codec;
    std::string m_ffmpeg;
    std::string m_path;

    Caption m_x_caption, m_y_caption, m_radius_caption, m_maxiters_caption,
      m_seconds_caption, m_fps_caption, m_bitrate_caption, m_codec_caption,
      m_ffmpeg_caption, m_path_caption;

    RealControl m_x_control, m_y_control, m_radius_control;
    IntegerControl m_maxiters_control, m_seconds_control, m_fps_control,
      m_bitrate_control;
    StringControl m_codec_control;		    // TODO use a drop-down?
    StringControl m_ffmpeg_control, m_path_control; // TODO use a file dialog

    MovieControls(MovieWindow *window):
      m_window(window),
      m_x(0), m_y(0), m_radius(2),
      m_maxiters(255),
      m_seconds(10), m_fps(25),
      m_bitrate(8 * 1024 * 1024),
      m_codec("libx264"),
      m_ffmpeg(isOnPath("avconv") ? "avconv" : "ffmpeg"),
      m_path("mandy.avi"),
      m_x_caption("X center"),
      m_y_caption("Y center"),
      m_radius_caption("Radius"),
      m_maxiters_caption("Iterations"),
      m_seconds_caption("Length (s)"),
      m_fps_caption("Frames/s"),
      m_bitrate_caption("Bits/s"),
      m_codec_caption("Codec"),
      m_ffmpeg_caption("Path to ffmpeg"),
      m_path_caption("Filename"),
      m_x_control(this, &m_x, -arith_traits<arith_t>::maximum(),
		  arith_traits<arith_t>::maximum()),
      m_y_control(this, &m_y, -arith_traits<arith_t>::maximum(),
		  arith_traits<arith_t>::maximum()),
      m_radius_control(this, &m_radius, 0, arith_traits<arith_t>::maximum()),
      m_maxiters_control(this, &m_maxiters, 1, INT_MAX - 1),
      m_seconds_control(this, &m_seconds, 1, INT_MAX - 1),
      m_fps_control(this, &m_fps, 1, INT_MAX - 1),
      m_bitrate_control(this, &m_bitrate, 1, INT_MAX - 1),
      m_codec_control(this, &m_codec),
      m_ffmpeg_control(this, &m_ffmpeg),
      m_path_control(this, &m_path) {
      attach(m_x_caption, 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_x_control, 1, 2, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_y_caption, 0, 1, 1, 2, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_y_control, 1, 2, 1, 2, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_radius_caption, 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_radius_control, 1, 2, 2, 3, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_maxiters_caption, 0, 1, 3, 4, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_maxiters_control, 1, 2, 3, 4, Gtk::FILL, Gtk::SHRINK, 1, 1);

      attach(m_seconds_caption, 2, 3, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_seconds_control, 3, 4, 0, 1, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_fps_caption, 2, 3, 1, 2, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_fps_control, 3, 4, 1, 2, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_bitrate_caption, 2, 3, 2, 3, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_bitrate_control, 3, 4, 2, 3, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_codec_caption, 2, 3, 3, 4, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_codec_control, 3, 4, 3, 4, Gtk::FILL, Gtk::SHRINK, 1, 1);

      attach(m_ffmpeg_caption, 0, 1, 4, 5, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_ffmpeg_control, 1, 4, 4, 5, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_path_caption, 0, 1, 5, 6, Gtk::FILL, Gtk::SHRINK, 1, 1);
      attach(m_path_control, 1, 4, 5, 6, Gtk::FILL, Gtk::SHRINK, 1, 1);
    }

    void controlChanged(Control *);
  };

  class MovieWindow: public Gtk::Window {
  public:
    MovieControls controls;
    Gtk::Frame frame;
    Gtk::HBox buttons;
    Gtk::Button render, cancel;
    Gtk::Entry report;
    Gtk::VBox vbox;

    bool working;
    threadid_t thread;

    // Progress report form the worker
    class Progress: public Job {
    public:
      Progress(MovieWindow *window,
	       const std::string &message,
	       bool completed = false):
	m_window(window), m_message(message), m_completed(completed) {
      }
      MovieWindow *m_window;
      std::string m_message;	// progress message
      bool m_completed;		// set on completion
    };

    MovieWindow(): controls(this),
		   buttons(false, 0),
		   render("Render"),
		   cancel(Gtk::Stock::CANCEL),
		   vbox(false, 0),
		   working(false) {
      controls.Update();
      frame.add(controls);
      // Action buttons & progress report
      render.signal_clicked().connect
	(sigc::mem_fun(*this, &MovieWindow::Render));
      buttons.pack_start(render, false, false, 1);
      cancel.signal_clicked().connect
	(sigc::mem_fun(*this, &MovieWindow::Cancel));
      buttons.pack_start(cancel, false, false, 1);
      report.set_editable(false);
      buttons.pack_start(report, true, true, 1);
      // Put them together in a vbox
      vbox.pack_start(frame, false, false, 1);
      vbox.pack_end(buttons, true, true, 0);
      add(vbox);
    }

    ~MovieWindow() {
    }

    bool configurationValid() {
      bool valid = controls.allValid();
      if(controls.m_path.size() == 0 || controls.m_ffmpeg.size() == 0)
	valid = false;
      return valid;
    }

    void controlChanged() {
      assert(!working);
      render.set_sensitive(configurationValid());
    }

    void Render() {
      assert(configurationValid());
      render.set_sensitive(false);
      controls.ContainerActivated(); // TODO a misnamed now?
      working = true;
      ThreadCreate(thread, MovieWindow::worker, this);
    }

    void Cancel() {
      if(working) {
	return;			// TODO cancel movie renderer
      }
      delete this;
    }

    void worker() {
      static unsigned serial;	// TODO synchronization
      const int frames = controls.m_seconds * controls.m_fps;
      const arith_t sx = controls.m_x;
      const arith_t sy = controls.m_y;
      const arith_t sr = 2;
      const arith_t ex = controls.m_x;
      const arith_t ey = controls.m_y;
      const arith_t er = controls.m_radius;
      const int maxiters = controls.m_maxiters;
      const int width = 1280;
      const int height = 720;	// TODO configurable
      std::stringstream tmp_pattern_stream;
      tmp_pattern_stream << "tmp_" << getpid() << "_" << serial++ << "_%d.ppm";
      const std::string tmp_pattern = tmp_pattern_stream.str();
      double rk = pow(arith_traits<arith_t>::toDouble(er / sr), 1.0/(frames - 1));
      for(int frame = 0; frame < frames; ++frame) {
	std::stringstream pstream;
	pstream << "Frame " << frame << "/" << frames;
	(new Progress(this, pstream.str()))
	  ->submit(&MovieWindow::progress_callback, NULL);
	arith_t radius = sr * pow(rk, frame);
	arith_t x = sx + arith_t(frame) * (ex - sx) / (frames - 1);
	arith_t y = sy + arith_t(frame) * (ey - sy) / (frames - 1);
	char tmp[1024];
	sprintf(tmp, tmp_pattern.c_str(), frame);
	draw(width, height, x, y, radius, maxiters, tmp);
      }
      std::stringstream command, pstream;
      command << shellQuote(controls.m_ffmpeg)
	      << " -f image2"
	      << " -i " << tmp_pattern
	      << " -vcodec " << shellQuote(controls.m_codec)
	      << " -r " << controls.m_fps
	      << " -b " << controls.m_bitrate
	      << " " << shellQuote(controls.m_path);
      pstream << "Encoding with " << controls.m_ffmpeg;
      (new Progress(this, pstream.str()))
	->submit(&MovieWindow::progress_callback, NULL);
      ::remove(controls.m_path.c_str());
      fprintf(stderr, "%s\n", command.str().c_str());
      int rc = system(command.str().c_str());
      // TODO capture ffmpeg stderr and put it somewhere useful
      // (maybe the progress report should be a larger window)
      for(int frame = 0; frame < frames; ++frame) {
	char tmp[1024];
	sprintf(tmp, tmp_pattern.c_str(), frame);
	::remove(tmp);
      }
      if(rc) {
	::remove(controls.m_path.c_str());
	(new Progress(this, "Encoding failed.", true))
	  ->submit(&MovieWindow::progress_callback, NULL);
      } else
	(new Progress(this, "Movie completed.", true))
	  ->submit(&MovieWindow::progress_callback, NULL);
    }

    void progress(const std::string &message, bool completed) {
      report.set_text(message);
      if(completed) {
	ThreadJoin(thread);
	working = false;
	render.set_sensitive(configurationValid());
      }
    }

    static void *worker(void *arg) {
      static_cast<MovieWindow *>(arg)->worker();
      return NULL;
    }

    static void progress_callback(Job *job, void *) {
      Progress *p = static_cast<Progress *>(job);
      p->m_window->progress(p->m_message, p->m_completed);
    }
  };

  void MovieControls::controlChanged(Control *) {
    m_window->controlChanged();
  }

  void Movie(arith_t x,
	     arith_t y,
	     arith_t radius,
             int maxiters) {
    MovieWindow *w = new MovieWindow();
    w->controls.m_x = x;
    w->controls.m_y = y;
    w->controls.m_radius = radius;
    w->controls.m_maxiters = maxiters;
    w->controls.Update();
    w->show_all();
  }

};

