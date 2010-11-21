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
#ifndef THREADING_GLIB_H
#define THREADING_GLIB_H

#if USE_GTHREADS
inline void ThreadInit() {
  Glib::thread_init();
}

inline mutex_t *LockCreate() {
  return new mutex_t();
}

inline void LockAcquire(mutex_t *m) {
  m->lock();
}

inline void LockRelease(mutex_t *m) {
  m->unlock();
}

inline cond_t *CondCreate() {
  return new cond_t();
}

inline void CondWait(cond_t *c, mutex_t *m) {
  c->wait(*m);
}

inline void CondSignal(cond_t *c) {
  c->signal();
}

inline void CondBroadcast(cond_t *c) {
  c->broadcast();
}
#endif
#endif /* THREADING_GLIB_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
