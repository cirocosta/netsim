#ifndef NETSIM__THREAD_H
#define NETSIM__THREAD_H

#include <pthread.h>
#include "netsim/common.h"

static void* test2_thread(void* arg)
{
  LOGERR("LOL!");
  pthread_exit(NULL);
}

typedef struct ns_thread_t {
  pthread_t tid;
  void* init_data;
  void* res_data;
} ns_thread_t;

inline static void fs_thread_join(pthread_t thread, void** retval)
{
  int err;

  if ((err = pthread_join(thread, retval))) {
    errno = err;
    PASSERT(err, "pthread_join:");
  }
}

inline static void fs_thread_create(pthread_t* tid, const pthread_attr_t* attr,
                                    void* (*start_routine)(void*), void* arg)
{
  int err;

  if ((err = pthread_create(tid, attr, start_routine, arg))) {
    errno = err;
    PASSERT(err, "pthread_join:");
  }
}

#endif
