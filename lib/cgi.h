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
#ifndef CGI_H
#define CGI_H

struct param_def {
  const char *name;
  void (*handler)(void *target, const char *value);
  void *target;
};

void parse_query(const char *query, const struct param_def *params);
void parse_query_one(const char *start, const char *end,
                     const struct param_def *params);
const char *urldecode(const char *start, const char *end, char *v, size_t vsize,
                      int name);
int unhex(char ch);
void param_string(void *target, const char *value);
void param_int(void *target, const char *value);
void param_int_positive(void *target, const char *value);
void param_arith(void *target, const char *value);
void param_arith_positive(void *target, const char *value);

#endif /* CGI_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
