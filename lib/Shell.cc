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
#include "Shell.h"
#include <sstream>
#include <cstdlib>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif

std::string shellQuote(const std::string &s) {
  if(s.size()) {
    // TODO windows quoting rules!
    std::stringstream ss;
    for(size_t n = 0; n < s.size(); ++n) {
      char c = s.at(n);
      if((c & 128)
	 || (c >= 'a' && c <= 'z')
	 || (c >= 'A' && c <= 'Z')
	 || (c >= '0' && c <= '9')
	 || c == '-' || c == '_' || c == '/' || c == '.')
	ss << c;
      else
	ss << '\\' << c;
    }
    return ss.str();
  } else
    return "\"\"";
}

bool isOnPath(const std::string &name) {
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
    if(access(candidate.c_str(), X_OK) == 0) // TODO windows
      return true;
    if(sep)
      path = sep + 1;
    else
      break;
  }
  return false;
}
