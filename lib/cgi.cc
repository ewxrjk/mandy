/* Copyright © 2015 Richard Kettlewell.
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
#include "cgi.h"
#include <cctype>
#include <stdexcept>
#include <arith.h>
#include <climits>

void parse_query(const char *query, const struct param_def *params) {
  while(*query) {
    const char *ptr = strchr(query, '&');
    if(ptr) {
      parse_query_one(query, ptr, params);
      query = ptr + 1;
    } else {
      parse_query_one(query, query + strlen(query), params);
      break;
    }
  }
}

void parse_query_one(const char *start, const char *end,
                     const struct param_def *params) {
  char name[64], value[1024];
  start = urldecode(start, end, name, sizeof name, 1);
  if(start >= end)
    throw std::runtime_error("malformed parameter");
  urldecode(start, end, value, sizeof value, 0);
  while(params->name) {
    if(!strcmp(params->name, name)) {
      params->handler(params->target, value);
      return;
    }
    ++params;
  }
  throw std::runtime_error("unrecognized parameter");
}

const char *urldecode(const char *start, const char *end, char *v, size_t vsize,
                      int name) {
  size_t pos = 0;
  --vsize; // leave room for the final 0
  while(start < end) {
    char ch = *start++;
    switch(ch) {
    default: break;
    case '=':
      if(name) {
        v[pos] = 0;
        return start;
      } else
        break;
    case '+': ch = ' '; break;
    case '%':
      if(end - start < 2)
        throw std::runtime_error("truncated urlencode");
      ch = 16 * unhex(start[0]) + unhex(start[1]);
      start += 2;
      break;
    }
    if(pos < vsize)
      v[pos++] = ch;
    else
      throw std::runtime_error("excessively long parameter");
  }
  v[pos] = 0;
  return start;
}

int unhex(char ch) {
  if(ch >= '0') {
    if(ch <= '9')
      return ch - '0';
    ch |= 0x20;
    if(ch >= 'a' and ch <= 'f')
      return ch - ('a' - 10);
  }
  throw std::runtime_error("invalid hex");
}

void param_string(void *target, const char *value) {
  *(std::string *)target = value;
}

void param_int(void *target, const char *value) {
  char *e;
  long n;
  errno = 0;
  n = strtol(value, &e, 0);
  if(errno)
    throw std::runtime_error(strerror(errno));
  if(e == value || *e)
    throw std::runtime_error("malformed integer");
  if(n < INT_MIN || n > INT_MAX)
    throw std::runtime_error("integer out of range");
  *(int *)target = n;
}

void param_int_positive(void *target, const char *value) {
  param_int(target, value);
  if(*(int *)target <= 0)
    throw std::runtime_error("integer must be positive");
}

void param_arith(void *target, const char *value) {
  arith_t n;
  char *e;
  int rc = arith_traits<arith_t>::fromString(n, value, &e);
  if(rc)
    throw std::runtime_error(strerror(rc));
  if(e == value || *e)
    throw std::runtime_error("malformed value");
  *(arith_t *)target = n;
}

void param_arith_positive(void *target, const char *value) {
  param_arith(target, value);
  if(*(arith_t *)target <= 0)
    throw std::runtime_error("value must be positive");
}

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
