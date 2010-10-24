#include "mand.h"
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static int ncores;
static struct threadinfo {
  pthread_t id;
  double x, y, size;
  int xpixels, ypixels;
  int *results;
} *threads;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int quit_threads;

static int calc(double cx, double cy) {
  // let c = cx + icy
  // let z = zx + izy
  //
  // then z^2 + c = zx^2 - zy^2 + cx + i(2zxzy+cy)
  int iterations = 0;
  double zx = 0, zy = 0;
  while(iterations < MAXITER && (zx * zx + zy * zy < 4.0)) {
    double nzx = zx * zx - zy * zy + cx;
    double nzy = 2 * zx * zy  + cy;
    zx = nzx;
    zy = nzy;
    ++iterations;
  }
  return iterations;
}

static void *worker(void *arg) {
  struct threadinfo *me = arg;
  int rc;

  if((rc = pthread_mutex_lock(&lock)))
    fatal(rc, "pthread_mutex_lock");
  while(!quit_threads) {
    if(!me->results) {
      if((rc = pthread_cond_wait(&cond, &lock)))
	fatal(rc, "pthread_cond_wait");
      continue;
    }
    int *results = me->results;
    if((rc = pthread_mutex_unlock(&lock)))
      fatal(rc, "pthread_mutex_unlock");
    for(int py = 0; py < me->ypixels; ++py)
      for(int px = 0; px < me->xpixels; ++px)
	*results++ = calc(me->x + px * me->size / me->xpixels,
			  me->y + py * me->size / me->xpixels);
    if((rc = pthread_mutex_lock(&lock)))
      fatal(rc, "pthread_mutex_lock");
    me->results = 0;
    if((rc = pthread_cond_broadcast(&cond)))
      fatal(rc, "pthread_cond_broadcast");
  }
  if((rc = pthread_mutex_unlock(&lock)))
    fatal(rc, "pthread_mutex_unlock");
  return NULL;
}

void init_threads(void) {
  ncores = sysconf(_SC_NPROCESSORS_ONLN);
  threads = malloc(ncores * sizeof *threads);
  memset(threads, 0, ncores * sizeof *threads);
  int rc;
  for(int n = 0; n < ncores; ++n)
    if((rc = pthread_create(&threads[n].id, NULL, worker, &threads[n])))
      fatal(rc, "pthread_create");
}

void destroy_threads(void) {
  int rc;
  if((rc = pthread_mutex_lock(&lock)))
    fatal(rc, "pthread_mutex_lock");
  quit_threads = 1;
  if((rc = pthread_cond_broadcast(&cond)))
    fatal(rc, "pthread_cond_broadcast");
  if((rc = pthread_mutex_unlock(&lock)))
    fatal(rc, "pthread_mutex_unlock");
  for(int n = 0; n < ncores; ++n) {
    if((rc = pthread_join(threads[n].id, NULL)))
      fatal(rc, "pthread_join");
  }
}

// Compute values for [x,x+size) x [y, y+size) in xpixels * ypixels
void mand(double x, double y, double size, int xpixels, int ypixels,
	  int *results) {
  int rc;
  if((rc = pthread_mutex_lock(&lock)))
    fatal(rc, "pthread_mutex_lock");
  // Fill in the per-thread requests
  int ychunk = ypixels / ncores;
  int extra = ypixels % ncores != 0;
  for(int n = 0; n < ncores; ++n) {
    threads[n].x = x;
    threads[n].y = y + n * ychunk * size / xpixels;
    threads[n].size = size;
    threads[n].xpixels = xpixels;
    threads[n].ypixels = ychunk + (n == ncores - 1 && extra);
    threads[n].results = results + xpixels * n * ychunk;
  }
  // Set them going
  if((rc = pthread_cond_broadcast(&cond)))
    fatal(rc, "pthread_cond_broadcast");
  // Wait for them to finish
  for(;;) {
    int finished = 0;
    for(int n = 0; n < ncores; ++n)
      if(threads[n].results == NULL)
	++finished;
    if(finished >= ncores)
      break;
    if((rc = pthread_cond_wait(&cond, &lock)))
      fatal(rc, "pthread_cond_wait");
  }
  if((rc = pthread_mutex_unlock(&lock)))
    fatal(rc, "pthread_mutex_unlock");
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
