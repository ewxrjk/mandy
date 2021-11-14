/* Copyright © 2010 Richard Kettlewell.
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
#include "Threading.h"

#if USE_GTHREADS
void ThreadCreate(threadid_t &id, void *(*threadfn)(void *arg), void *arg) {
  id = Glib::Thread::create(
      sigc::bind(sigc::hide_return(sigc::ptr_fun(threadfn)), arg),
      true /*joinable*/);
}

void ThreadJoin(threadid_t &id) {
  id->join();
}
#endif

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
