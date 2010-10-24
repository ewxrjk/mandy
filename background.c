#include "mand.h"
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static double goalx, goaly, goalxsize;
static int goalw, goalh;
static int goalmet = 1;
static int *goaliters;

static pthread_t controller_id;
static int ncores;
static struct threadinfo {
  pthread_t id;
  double x, y, xsize;
  int xpixels, ypixels;
  int *results;
} *threads;

// controller() broadcast to worker()s to indicate new work to do, or to quit
static pthread_cond_t work_available = PTHREAD_COND_INITIALIZER;

// worker()s signal controller() to indicate work has completed
static pthread_cond_t work_done = PTHREAD_COND_INITIALIZER;

// compute() signals controller() to indicate goal has changed
static pthread_cond_t goal_changed = PTHREAD_COND_INITIALIZER;

// Everything is protected by one big lock
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Set to tell all threads to just quit already
static int quit_threads;

/* Compute the iteration count for one point */
static int calc(double cx, double cy) {
  // let c = cx + icy
  // let z = zx + izy
  //
  // then z^2 + c = zx^2 - zy^2 + cx + i(2zxzy+cy)
  int iterations = 0;
  double zx = 0, zy = 0;
  while(iterations < maxiter && (zx * zx + zy * zy < 4.0)) {
    double nzx = zx * zx - zy * zy + cx;
    double nzy = 2 * zx * zy  + cy;
    zx = nzx;
    zy = nzy;
    ++iterations;
  }
  return iterations;
}

/* Do a rectangular portion of the computation */
static void *worker(void *arg) {
  struct threadinfo *me = arg;
  int rc;

  if((rc = pthread_mutex_lock(&lock)))
    fatal(rc, "pthread_mutex_lock");
  while(!quit_threads) {
    if(!me->results) {
      if((rc = pthread_cond_wait(&work_available, &lock)))
	fatal(rc, "pthread_cond_wait");
      continue;
    }
    int *results = me->results;
    if((rc = pthread_mutex_unlock(&lock)))
      fatal(rc, "pthread_mutex_unlock");
    for(int py = 0; py < me->ypixels; ++py)
      for(int px = 0; px < me->xpixels; ++px)
	*results++ = calc(me->x + px * me->xsize / me->xpixels,
			  me->y + py * me->xsize / me->xpixels);
    if((rc = pthread_mutex_lock(&lock)))
      fatal(rc, "pthread_mutex_lock");
    me->results = 0;
    if((rc = pthread_cond_broadcast(&work_done)))
      fatal(rc, "pthread_cond_broadcast");
  }
  if((rc = pthread_mutex_unlock(&lock)))
    fatal(rc, "pthread_mutex_unlock");
  return NULL;
}

/* Compute values for [x,x+xsize) x [y, y+xsize*ypixels/xpixels) in
 * xpixels * ypixels.  lock must be held. */
static void mand(double x, double y, double xsize, int xpixels, int ypixels,
	  int *results) {
  int rc;
  // Fill in the per-thread requests
  int ychunk = ypixels / ncores;
  int extra = ypixels % ncores != 0;
  for(int n = 0; n < ncores; ++n) {
    threads[n].x = x;
    threads[n].y = y + n * ychunk * xsize / xpixels;
    threads[n].xsize = xsize;
    threads[n].xpixels = xpixels;
    threads[n].ypixels = ychunk + (n == ncores - 1 && extra);
    threads[n].results = results + xpixels * n * ychunk;
  }
  // Set them going
  if((rc = pthread_cond_broadcast(&work_available)))
    fatal(rc, "pthread_cond_broadcast");
  // Wait for them to finish
  for(;;) {
    int finished = 0;
    for(int n = 0; n < ncores; ++n)
      if(threads[n].results == NULL)
	++finished;
    if(finished >= ncores)
      break;
    if((rc = pthread_cond_wait(&work_done, &lock)))
      fatal(rc, "pthread_cond_wait");
  }
}

/* Controller thread.  When goalmet is set to 0, allocates a new iters[]
 * and fills it in (using the worker threads).
 */
static void *controller(void __attribute__((unused)) *data) {
  int rc;
  if((rc = pthread_mutex_lock(&lock)))
    fatal(rc, "pthread_mutex_lock");
  while(!quit_threads) {
    if(goalmet) {
      if((rc = pthread_cond_wait(&goal_changed, &lock)))
	fatal(rc, "pthread_cond_wait");
      continue;
    }
    double workx = goalx, worky = goaly, workxsize = goalxsize;
    int workw = goalw, workh = goalh;
    if(!(goaliters = malloc(workw * workh * sizeof(int))))
      fatal(errno, "malloc");
    mand(workx, worky, workxsize, workw, workh, goaliters);
    if(workx != goalx || worky != goaly || workxsize != goalxsize
       || workw != goalw || workh != goalh) {
      // The goal changed while we were thinking
      free(goaliters);
      goaliters = NULL;
      continue;
    }
    goalmet = 1;
    // We don't signal anything with the completion, the main thread
    // is expected to poll.
  }
  if((rc = pthread_mutex_unlock(&lock)))
    fatal(rc, "pthread_mutex_unlock");
  return NULL;
}

/* Initialize all threads */
void init_threads(void) {
  ncores = sysconf(_SC_NPROCESSORS_ONLN);
  if(!(threads = malloc(ncores * sizeof *threads)))
    fatal(errno, "malloc");
  memset(threads, 0, ncores * sizeof *threads);
  int rc;
  for(int n = 0; n < ncores; ++n)
    if((rc = pthread_create(&threads[n].id, NULL, worker, &threads[n])))
      fatal(rc, "pthread_create");
  if((rc = pthread_create(&controller_id, NULL, controller, NULL)))
    fatal(rc, "pthread_create");
}

/* Destroy all threads */
void destroy_threads(void) {
  int rc;
  if((rc = pthread_mutex_lock(&lock)))
    fatal(rc, "pthread_mutex_lock");
  quit_threads = 1;
  if((rc = pthread_cond_broadcast(&work_available)))
    fatal(rc, "pthread_cond_broadcast");
  if((rc = pthread_cond_signal(&goal_changed)))
    fatal(rc, "pthread_cond_broadcast");
  if((rc = pthread_mutex_unlock(&lock)))
    fatal(rc, "pthread_mutex_unlock");
  for(int n = 0; n < ncores; ++n) {
    if((rc = pthread_join(threads[n].id, NULL)))
      fatal(rc, "pthread_join");
  }
  if((rc = pthread_join(controller_id, NULL)))
    fatal(rc, "pthread_join");
}

/* Request a computation.  A non-NULL return means that the data is
 * available.  A NULL return means it is not, but will be later; you
 * must call again. */
int *compute(double x, double y, double xsize, int w, int h) {
  int *result = NULL, rc;

  if((rc = pthread_mutex_lock(&lock)))
    fatal(rc, "pthread_mutex_lock");
  if(goalx == x
     && goaly == y
     && goalxsize == xsize
     && goalw == w
     && goalh == h) {
    // Goal has not changed, but it might still be under construction
    if(goalmet) {
      result = goaliters;
      goaliters = NULL;		/* hand over ownership */
    }
  } else {
    // Goal has changed.
    if(goalmet) {
      // The last computation might never have been collected
      free(goaliters);
      goaliters = NULL;
    }
    goalx = x;
    goaly = y;
    goalxsize = xsize;
    goalw = w;
    goalh = h;
    goalmet = 0;
    if((rc = pthread_cond_signal(&goal_changed)))
      fatal(rc, "pthread_cond_signal");
  }
  if((rc = pthread_mutex_unlock(&lock)))
    fatal(rc, "pthread_mutex_unlock");
  return result;
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
