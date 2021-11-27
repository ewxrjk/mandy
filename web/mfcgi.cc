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
#include <fcgiapp.h>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>

static const struct option options[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {"test", required_argument, NULL, 't'},
    {"count", required_argument, NULL, 'n'},
    {"no-header", required_argument, NULL, 'H'},
    {NULL, 0, NULL, 0}};

static void process_request(FCGX_Stream *out, FCGX_ParamArray envp,
                            bool header);
static std::string format(int rc);
static gboolean pixbuf_save_callback(const gchar *buf, gsize count,
                                     GError **error, gpointer data);
static gboolean stdio_save_callback(const gchar *buf, gsize count, GError **,
                                    gpointer data);
static void test(const char *query, bool header);

int main(int argc, char **argv) {
  int n, rc;
  bool header = true;
  FCGX_Stream *in;
  FCGX_Stream *out;
  FCGX_Stream *err;
  FCGX_ParamArray envp;
  int count = 1;

#ifndef GLIB_VERSION_2_36 // deprecation warning from then on
  g_type_init();
#endif

  try {
    while((n = getopt_long(argc, argv, "hVt:Hn:", options, NULL)) >= 0) {
      switch(n) {
      case 'h':
        printf("Usage:\n"
               "  mfcgi [OPTIONS] [PATH]\n"
               "Options:\n"
               "  --help, -h        Display help message\n"
               "  --version, -V     Display version number\n");
        return 0;
      case 'V': puts(PACKAGE_VERSION); return 0;
      case 'n': count = atoi(optarg); break;
      case 't':
        for(n = 1; n <= count; ++n)
          test(optarg, header);
        return 0;
      case 'H': header = false; break;
      default: return 1;
      }
    }
    if((rc = FCGX_Init()) != 0)
      throw std::runtime_error("FCGX_Init failed: " + format(rc));
    if(optind < argc) {
      if(FCGX_OpenSocket(argv[optind], SOMAXCONN) == -1)
        throw std::runtime_error("FCGX_OpenSocket failed: " + format(rc));
      ++optind;
    }
    if(optind != argc)
      throw std::runtime_error("invalid command line syntax");
    while(FCGX_Accept(&in, &out, &err, &envp) == 0) {
      try {
        process_request(out, envp, header);
      } catch(std::runtime_error &e) {
        if(header)
          FCGX_FPrintF(out, "Content-Type: text/plain\r\nStatus: 500\r\n\r\n");
        FCGX_FPrintF(out, "Sorry, something broke.\n");
        FCGX_FPrintF(err, "%s\n", e.what());
      }
      FCGX_Finish();
    }
    throw std::runtime_error("FCGX_Accept failed: " + format(rc));
  } catch(std::runtime_error &e) {
    fprintf(stderr, "ERROR: %s\n", e.what());
    return 1;
  }
  return 0;
}

static void process_request(FCGX_Stream *out, FCGX_ParamArray envp,
                            bool header) {
  while(*envp) {
    static const char prefix[] = "QUERY_STRING=";
    if(!strncmp(*envp, prefix, strlen(prefix))) {
      process_query(*envp + strlen(prefix), pixbuf_save_callback, out, header);
      return;
    }
    ++envp;
  }
  FCGX_FPrintF(out, "No query string found\n");
}

static gboolean pixbuf_save_callback(const gchar *buf, gsize count, GError **,
                                     gpointer data) {
  FCGX_Stream *out = (FCGX_Stream *)data;
  FCGX_PutStr(buf, count, out);
  return TRUE;
}

static void test(const char *query, bool header) {
  process_query(query, stdio_save_callback, stdout, header);
  fflush(stdout);
}

static gboolean stdio_save_callback(const gchar *buf, gsize count, GError **,
                                    gpointer data) {
  FILE *out = (FILE *)data;
  fwrite(buf, 1, count, out);
  return TRUE;
}

static std::string format(int rc) {
  char buffer[64];
  switch(rc) {
  case FCGX_UNSUPPORTED_VERSION: return "FCGX_UNSUPPORTED_VERSION";
  case FCGX_PROTOCOL_ERROR: return "FCGX_PROTOCOL_ERROR";
  case FCGX_PARAMS_ERROR: return "FCGX_PARAMS_ERROR";
  case FCGX_CALL_SEQ_ERROR: return "FCGX_CALL_SEQ_ERROR";
  default: snprintf(buffer, sizeof buffer, "%d", rc); return buffer;
  }
}
