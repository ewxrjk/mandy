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
#include "mandy.h"
#include <vector>
#include "Shell.h"
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

std::string shellQuote(const std::string &s) {
  if(s.size()) {
    std::stringstream ss;
    for(size_t n = 0; n < s.size(); ++n) {
      char c = s.at(n);
      if((c & 128) || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
         || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '/'
         || c == '.')
        ss << c;
      else
        ss << '\\' << c;
    }
    return ss.str();
  } else
    return "\"\"";
}

std::string findOnPath(const std::string &name) {
  const char *path = getenv("PATH");
  while(*path) {
    const char *sep = strchr(path, PATHSEP);
    std::string candidate;
    if(sep)
      candidate.assign(path, (size_t)(sep - path));
    else
      candidate.assign(path);
    candidate.append(DIRSEP);
    candidate.append(name);
    candidate.append(EXEEXT);
    if(access(candidate.c_str(), X_OK) == 0)
      return candidate;
    if(sep)
      path = sep + 1;
    else
      break;
  }
  return "";
}

std::string ffmpegDefault() {
  std::string path = findOnPath("avconv");
  if(!path.size())
    path = findOnPath("ffmpeg");
  return path;
}

bool Capture(const std::string &cmd, std::vector<std::string> &output) {
  output.clear();
  FILE *fp = popen(cmd.c_str(), "r");
  if(!fp)
    return false;
  char *line = 0;
  size_t len = 0;
  while(getline(&line, &len, fp) != -1)
    output.push_back(line);
  free(line);
  if(pclose(fp) != 0)
    return false;
  return true;
}
