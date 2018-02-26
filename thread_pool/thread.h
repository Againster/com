#ifndef THREAD_H
#define THREAD_H

#if __linux__ == 1
#include <pthread.h>
#include <signal.h>
#elif _WIN32 == 1
#endif // __linux__

#include "callable.h"

/**
 * NEW, A thread that has no yet started is in this state.
 * RUNNABLE, A thread executing in the system is in this state.
 * BLOCKED, A thread that is blocked waiting for a monitor lock is in this
 * state.
 * WAITING, A thread that 
 * TERMINATED
 */

typedef enum THREAD_STATE {
  NEW,
  RUNNABLE,
  BLOCKED,
  WAITING,
  TIMED_WAITING,
  TERMINATED
} THREAD_STATE;

typedef unsigned long thread_id_t;

typedef struct thread_s {
  thread_id_t           id;
  THREAD_STATE          state;
  callable_pt           call;
  void*                 argv;
  void*                 ret;
  void*                 platform;

  void*                 (*precall)(void*);
  void*                 precall_argv;
  void*                 precall_ret;
  void*                 (*postcall)(void*);
  void*                 postcall_argv;
  void*                 postcall_ret;
} thread_t;

typedef int (*thread_operation_pt)(thread_t* t);

typedef struct thread_operation_s {
  thread_operation_pt   start;
  thread_operation_pt   suspend;
  thread_operation_pt   destroy;
  thread_operation_pt   join;
} thread_operation_t;

#if __linux__ == 1
typedef struct thread_platform_on_linux_s {
  pthread_cond_t                       cond;
  pthread_condattr_t                   c_attr;
  pthread_mutex_t                      mutex;
  pthread_mutexattr_t                  m_attr;

  pthread_cond_t                       cond_join;
  pthread_condattr_t                   c_attr_join;
  pthread_mutex_t                      mutex_join;
  pthread_mutexattr_t                  m_attr_join;

  pthread_mutex_t                      mutex_state;
  pthread_mutexattr_t                  m_attr_state;
} thread_platform_on_linux_t;

extern thread_operation_t thread_operation_on_linux;
#endif // __linux__


#endif // THREAD_H
