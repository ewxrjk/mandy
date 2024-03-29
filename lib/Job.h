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
#ifndef JOB_H
#define JOB_H

#include "IterBuffer.h"
#include <list>
#include <set>
#include <vector>
#include "Threading.h"

/* Base class for jobs passed to worker threads.  Agnostic about what the job
 * actually does.  A key point is that the number of threads matches the number
 * of cores, the idea being that all the work gets done with a minimum of
 * context switching. */
class Job {
public:
  void (*completion_callback)(Job *, void *); // called upon completion
  void *completion_data;                      // passed to callback

private:
  static std::list<Job *> queue;          // job queue
  static std::set<Job *> working;         // jobs being processed
  static std::list<Job *> completed;      // completed jobs
  static cond_t *queued_cond;             // signaled when a job is queued
  static cond_t *completed_cond;          // signaled when a job is completed
  static mutex_t *lock;                   // lock protecting jobs
  static std::vector<threadid_t> workers; // worker thread IDs
  static bool shutdown;                   // shutdown flag
  static void *worker(void *);            // work thread
  static void dequeue();
  static bool dequeue(void *completion_data);

public:
  virtual ~Job();

  // Override in derived class to define what the job does
  virtual void work();

  // Submit the job.  It will be run at some point in a background thread
  // unless cancel() is called before it reaches the head of the queue.
  //
  // completion_callback() will be called from inside poll().  No locks will be
  // held while it's being called, making it safe to invoke Job::submit(),
  // Job::cancel() and even Job::poll().  After the completion callback returns
  // the job will be deleted.
  void submit(void (*completion_callback)(Job *, void *), void *completion_data = NULL);

  // Cancel outstanding jobs with matching completion_data.
  // Jobs already in progress will still complete.
  static void cancel(void *completion_data);

  // Wait for up to max jobs, and execute their completion callbacks in this
  // thread.  This must only be used from the main thread.
  static bool poll(int max = 16);

  // Wait for all jobs with matching completion_data and execute their
  // completion callbacks in this thread.  This can be used from any thread
  // that the matched jobs will accept being run in.
  static void poll(void *completion_data);

  static bool pending();                            // any work left?
  static bool pending(void *completion_data);       // any work left with matching
                                                    // completion_data?
  static bool pendingLocked(void *completion_data); // any work left with
                                                    // matching
                                                    // completion_data?
                                                    // (Caller must hold lock)

  static void init(int nthreads = -1); // initialize thread pool
  static void destroy();               // destroy thread pool

  template <class T> static bool find_jobs(const T &collection, void *completion_data) {
    for(typename T::const_iterator it = collection.begin(); it != collection.end(); ++it) {
      const Job *j = *it;
      if(j->completion_data == completion_data)
        return true;
    }
    return false;
  }
};

#endif /* JOB_H */

/*
Local Variables:
mode:c++
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
