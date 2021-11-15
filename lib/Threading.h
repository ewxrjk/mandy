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
#ifndef THREADING_H
#define THREADING_H

#include <pthread.h>

typedef pthread_cond_t cond_t;
typedef pthread_mutex_t mutex_t;
typedef pthread_t threadid_t;

#define COND_INIT = PTHREAD_COND_INITIALIZER
#define MUTEX_INIT = PTHREAD_MUTEX_INITIALIZER

mutex_t *LockCreate();
void LockAcquire(mutex_t *m);
void LockRelease(mutex_t *m);
cond_t *CondCreate();
void CondWait(cond_t *c, mutex_t *m);
void CondSignal(cond_t *c);
void CondBroadcast(cond_t *c);
void ThreadCreate(threadid_t &id, void *(*threadfn)(void *arg),
                  void *arg = NULL);
void ThreadJoin(threadid_t &id);

#endif /* THREADING_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
