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

#if USE_PTHREADS
mutex_t *LockCreate() {
  int rc;
  mutex_t *m = new mutex_t();
  if((rc = pthread_mutex_init(m, NULL)))
    fatal(rc, "pthread_mutex_init");
  return m;
}

void LockAcquire(mutex_t *m) {
  int rc;

  if((rc = pthread_mutex_lock(m)))
    fatal(rc, "pthread_mutex_lock");
}

void LockRelease(mutex_t *m) {
  int rc;

  if((rc = pthread_mutex_unlock(m)))
    fatal(rc, "pthread_mutex_unlock");
}

cond_t *CondCreate() {
  int rc;
  cond_t *c = new cond_t();
  if((rc = pthread_cond_init(c, NULL)))
    fatal(rc, "pthread_cond_init");
  return c;
}

void CondWait(cond_t *c, mutex_t *m) {
  int rc;

  if((rc = pthread_cond_wait(c, m)))
    fatal(rc, "pthread_cond_wait");
}

void CondSignal(cond_t *c) {
  int rc;

  if((rc = pthread_cond_signal(c)))
    fatal(rc, "pthread_cond_signal");
}

void CondBroadcast(cond_t *c) {
  int rc;

  if((rc = pthread_cond_broadcast(c)))
    fatal(rc, "pthread_cond_broadcast");
}

void ThreadCreate(threadid_t &id, void *(*threadfn)(void *arg), void *arg) {
  int rc;

  if((rc = pthread_create(&id, NULL, threadfn, arg)))
    fatal(rc, "pthread_create");
}

void ThreadJoin(threadid_t &id) {
  int rc;

  if((rc = pthread_join(id, NULL)))
    fatal(rc, "pthread_join");
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
