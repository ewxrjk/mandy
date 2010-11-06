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
#include <unistd.h>

void Job::submit(void (*completion_callback_)(Job *, void *),
                 void *completion_data_) {
  int rc;
  completion_callback = completion_callback_;
  completion_data = completion_data_;
  acquireLock();
  queue.push_back(this);
  if((rc = pthread_cond_signal(&queued)))
    fatal(rc, "pthread_cond_signal");
  releaseLock();
}

void Job::cancel(void *classId) {
  acquireLock();
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
  releaseLock();
}

void Job::init() {
  int ncores = sysconf(_SC_NPROCESSORS_ONLN), rc;
  for(int n = 0; n < ncores; ++n) {
    pthread_t id;
    if((rc = pthread_create(&id, NULL, worker, NULL)))
      fatal(rc, "pthread_create");
    workers.push_back(id);
  }
}

void Job::destroy() {
  int rc;
  acquireLock();
  shutdown = true;
  if((rc = pthread_cond_broadcast(&queued)))
    fatal(rc, "pthread_cond_broadcast");
  releaseLock();
  while(workers.size()) {
    if((rc = pthread_join(workers.back(), NULL)))
      fatal(rc, "pthread_join");
    workers.pop_back();
  }
}

bool Job::poll(int max) {
  acquireLock();
  while(!completed.empty() && max-- > 0) {
    Job *j = completed.front();
    completed.pop_front();
    releaseLock();
    j->completion_callback(j, j->completion_data);
    delete j;
    acquireLock();
  }
  bool nowEmpty = completed.empty();
  releaseLock();
  return !nowEmpty;
}

void *Job::worker(void *) {
  int rc;
  acquireLock();
  while(!shutdown) {
    if(queue.empty()) {
      if((rc = pthread_cond_wait(&queued, &lock)))
	fatal(rc, "pthread_cond_wait");
      continue;
    }
    Job *j = queue.front();
    queue.pop_front();
    releaseLock();
    j->work();
    acquireLock();
    completed.push_back(j);
  }
  releaseLock();
  return NULL;
}

Job::~Job() {
}

bool Job::pending() {
  acquireLock();
  bool more = !completed.empty() || !queue.empty();
  releaseLock();
  return more;
}

std::list<Job *> Job::queue;
std::list<Job *> Job::completed;
pthread_cond_t Job::queued = PTHREAD_COND_INITIALIZER;
pthread_mutex_t Job::lock = PTHREAD_MUTEX_INITIALIZER;
std::vector<pthread_t> Job::workers;
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
