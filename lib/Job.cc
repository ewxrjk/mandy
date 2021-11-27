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
#include "mandy.h"
#include "Job.h"
#include <unistd.h>

void Job::submit(void (*completion_callback_)(Job *, void *),
                 void *completion_data_) {
  completion_callback = completion_callback_;
  completion_data = completion_data_;
  LockAcquire(lock);
  queue.push_back(this);
  CondSignal(queued_cond);
  LockRelease(lock);
}

void Job::cancel(void *completion_data) {
  LockAcquire(lock);
  for(std::list<Job *>::iterator it = queue.begin(); it != queue.end();) {
    std::list<Job *>::iterator here = it;
    ++it;
    Job *j = *here;
    if(completion_data == NULL || j->completion_data == completion_data) {
      delete j;
      queue.erase(here);
    }
  }
  for(std::list<Job *>::iterator it = completed.begin();
      it != completed.end();) {
    std::list<Job *>::iterator here = it;
    ++it;
    Job *j = *here;
    if(completion_data == NULL || j->completion_data == completion_data) {
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

bool Job::dequeue(void *completion_data) {
  std::list<Job *>::iterator it;
  for(it = completed.begin(); it != completed.end(); ++it) {
    Job *j = *it;
    if(j->completion_data == completion_data)
      break;
  }
  if(it == completed.end())
    return false;
  Job *j = *it;
  completed.erase(it);
  LockRelease(lock);
  j->completion_callback(j, j->completion_data);
  delete j;
  LockAcquire(lock);
  return true;
}

bool Job::poll(int max) {
  LockAcquire(lock);
  while(!completed.empty() && max-- > 0)
    dequeue();
  bool nowEmpty = completed.empty();
  LockRelease(lock);
  return !nowEmpty;
}

void Job::poll(void *completion_data) {
  LockAcquire(lock);
  while(pendingLocked(completion_data)) {
    if(!dequeue(completion_data))
      CondWait(completed_cond, lock);
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
    working.insert(j);
    LockRelease(lock);
    j->work();
    LockAcquire(lock);
    working.erase(j);
    completed.push_back(j);
    CondSignal(completed_cond);
  }
  LockRelease(lock);
  return NULL;
}

Job::~Job() {}

bool Job::pending() {
  LockAcquire(lock);
  bool more = !completed.empty() || !queue.empty() || working.size();
  LockRelease(lock);
  return more;
}

bool Job::pending(void *completion_data) {
  LockAcquire(lock);
  bool more = pendingLocked(completion_data);
  LockRelease(lock);
  return more;
}

bool Job::pendingLocked(void *completion_data) {
  return (find_jobs(completed, completion_data)
          || find_jobs(queue, completion_data)
          || find_jobs(working, completion_data));
}

void Job::work() {}

std::list<Job *> Job::queue;
std::list<Job *> Job::completed;
std::set<Job *> Job::working;
cond_t *Job::queued_cond;
cond_t *Job::completed_cond;
mutex_t *Job::lock;
std::vector<threadid_t> Job::workers;
bool Job::shutdown;

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
