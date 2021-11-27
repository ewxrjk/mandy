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
  arith_t m_x = 0, m_y = 0, m_radius = 2;
  arith_type m_arith = ARITH_DEFAULT;
  int m_maxiters = 255, m_seconds = 10, m_fps = 25;
  int m_bitrate = 8 * 1024 * 1024;
  std::string m_codec = "libx264";
  std::string m_ffmpeg = ffmpegDefault();
  std::string m_path = "mandy.mp4";

  RealControl m_x_control, m_y_control, m_radius_control;
  IntegerControl m_maxiters_control, m_seconds_control, m_fps_control,
      m_bitrate_control;
  StringControl m_codec_control;
  FileSelectionControl m_ffmpeg_control;
  StringControl m_path_control; // TODO use a file chooser

  MovieControls(MovieWindow *window):
      m_window(window),
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
      m_ffmpeg_control(this, &m_ffmpeg, "Select Encoder"),
      m_path_control(this, &m_path) {

    m_x_control.Attach(0, 0, "X center");
    m_y_control.Attach(0, 1, "Y center");
    m_radius_control.Attach(0, 2, "Radius");
    m_maxiters_control.Attach(0, 3, "Iterations");

    m_seconds_control.Attach(1, 0, "Length (s)");
    m_fps_control.Attach(1, 1, "Frames/s");
    m_bitrate_control.Attach(1, 2, "Bits/s");
    m_codec_control.Attach(1, 3, "Codec");

    m_ffmpeg_control.Attach(0, 4, "Encoder", 2);
    m_path_control.Attach(0, 5, "Filename", 2);
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
    Progress(MovieWindow *window, const std::string &message,
             bool completed = false):
        m_window(window),
        m_message(message), m_completed(completed) {}
    MovieWindow *m_window;
    std::string m_message; // progress message
    bool m_completed;      // set on completion
  };

  class RenderMovieThread: public RenderMovie {
  public:
    RenderMovieThread(MovieWindow *mw_): mw(mw_) {}
    MovieWindow *mw = nullptr;

    void Progress(const std::string &message, bool completed) {
      auto *p = new MovieWindow::Progress(mw, message, completed);
      p->submit(&MovieWindow::progress_callback, NULL);
    }
  };

  MovieWindow():
      controls(this), buttons(false, 0), render("Render"),
      cancel(Gtk::Stock::CANCEL), vbox(false, 0), working(false) {
    controls.UpdateDisplay();
    frame.add(controls);
    // Action buttons & progress report
    render.signal_clicked().connect(sigc::mem_fun(*this, &MovieWindow::Render));
    buttons.pack_start(render, false, false, 1);
    cancel.signal_clicked().connect(sigc::mem_fun(*this, &MovieWindow::Cancel));
    buttons.pack_start(cancel, false, false, 1);
    report.set_editable(false);
    buttons.pack_start(report, true, true, 1);
    // Put them together in a vbox
    vbox.pack_start(frame, false, false, 1);
    vbox.pack_end(buttons, true, true, 0);
    add(vbox);
  }

  ~MovieWindow() {}

  bool configurationValid() {
    bool valid = controls.allDisplaysValid();
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
    controls.SetSensitivity(false);
    working = true;
    ThreadCreate(thread, MovieWindow::worker, this);
  }

  void Cancel() {
    if(working) {
      return; // TODO cancel movie renderer
    }
    delete this;
  }

  // Worker thread that renders the movie
  void worker() {
    RenderMovieThread rmt(this);
    rmt.sx = controls.m_x;
    rmt.sy = controls.m_y;
    rmt.sr = 2;
    rmt.ex = controls.m_x;
    rmt.ey = controls.m_y;
    rmt.er = controls.m_radius;
    rmt.maxiters = controls.m_maxiters;
    rmt.width = 1280;
    rmt.height = 720; // TODO configurable
    rmt.seconds = controls.m_seconds;
    rmt.fps = controls.m_fps;
    rmt.bitrate = controls.m_bitrate;
    rmt.arith = controls.m_arith;
    rmt.ffmpeg = controls.m_ffmpeg;
    rmt.codec = controls.m_codec;
    rmt.path = controls.m_path;
    rmt.Render();
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

void MovieControls::controlChanged(Control *c) {
  m_window->controlChanged();
  if(c == &m_ffmpeg_control) {
    c->UpdateUnderlying();
  }
}

void Movie(arith_t x, arith_t y, arith_t radius, int maxiters,
           arith_type arith) {
  MovieWindow *w = new MovieWindow();
  w->controls.m_x = x;
  w->controls.m_y = y;
  w->controls.m_radius = radius;
  w->controls.m_maxiters = maxiters;
  w->controls.m_arith = arith;
  w->controls.UpdateDisplay();
  w->show_all();
}
}; // namespace mmui
