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
#include "Job.h"
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

void Job::submit(void (*completion_callback_)(Job *, void *),
                 void *completion_data_) {
  completion_callback = completion_callback_;
  completion_data = completion_data_;
  LockAcquire(lock);
  queue.push_back(this);
  CondSignal(queued_cond);
  LockRelease(lock);
}

void Job::cancel(void *classId) {
  LockAcquire(lock);
  for(std::list<Job *>::iterator it = queue.begin();
      it != queue.end();
      ) {
    std::list<Job *>::iterator here = it;
    ++it;
    Job *j = *here;
    if(classId == NULL || j->classId == classId) {
      delete j;
      queue.erase(here);
    }
  }
  for(std::list<Job *>::iterator it = completed.begin();
      it != completed.end();
      ) {
    std::list<Job *>::iterator here = it;
    ++it;
    Job *j = *here;
    if(classId == NULL || j->classId == classId) {
      delete j;
      completed.erase(here);
    }
  }
  LockRelease(lock);
}

void Job::init(int nthreads) {
#if HAVE_SYSCONF && defined _SC_NPROCESSORS_ONLN
  if(nthreads == -1)
    nthreads = sysconf(_SC_NPROCESSORS_ONLN);
#endif
#if _WIN32
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  nthreads = sysinfo.dwNumberOfProcessors;
#endif
  if(nthreads == -1)
    nthreads = 1;
  queued_cond = CondCreate();
  completed_cond = CondCreate();
  lock = LockCreate();
  for(int n = 0; n < nthreads; ++n) {
    threadid_t id;
    ThreadCreate(id, worker);
    workers.push_back(id);
  }
}

void Job::destroy() {
  LockAcquire(lock);
  shutdown = true;
  CondBroadcast(queued_cond);
  LockRelease(lock);
  while(workers.size()) {
    ThreadJoin(workers.back());
    workers.pop_back();
  }
}

void Job::dequeue() {
  Job *j = completed.front();
  completed.pop_front();
  LockRelease(lock);
  j->completion_callback(j, j->completion_data);
  delete j;
  LockAcquire(lock);
}

bool Job::poll(int max) {
  LockAcquire(lock);
  while(!completed.empty() && max-- > 0)
    dequeue();
  bool nowEmpty = completed.empty();
  LockRelease(lock);
  return !nowEmpty;
}

void Job::pollAll() {
  LockAcquire(lock);
  while(!completed.empty() || !queue.empty() || working) {
    if(completed.empty()) {
      CondWait(completed_cond, lock);
      continue;
    }
    dequeue();
  }
  LockRelease(lock);
}

void *Job::worker(void *) {
  LockAcquire(lock);
  while(!shutdown) {
    if(queue.empty()) {
      CondWait(queued_cond, lock);
      continue;
    }
    Job *j = queue.front();
    queue.pop_front();
    ++working;
    LockRelease(lock);
    j->work();
    LockAcquire(lock);
    completed.push_back(j);
    --working;
    CondSignal(completed_cond);
  }
  LockRelease(lock);
  return NULL;
}

Job::~Job() {
}

bool Job::pending() {
  LockAcquire(lock);
  bool more = !completed.empty() || !queue.empty() || working;
  LockRelease(lock);
  return more;
}

std::list<Job *> Job::queue;
std::list<Job *> Job::completed;
cond_t *Job::queued_cond;
cond_t *Job::completed_cond;
mutex_t *Job::lock;
std::vector<threadid_t> Job::workers;
bool Job::shutdown;
int Job::working;

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
