#include "thread.h"

int start_on_linux(thread_t* t) {
  thread_platform_on_linux_t* platform;
  platform = (thread_platform_on_linux_t*)t->platform;
  t->state = RUNNABLE;
  pthread_cond_signal(&platform->cond);
  return 0;
}

int suspend_on_linux(thread_t* t) {
  return 0;
}

int destroy_on_linux(thread_t* t) {
  thread_platform_on_linux_t* platform;
  platform = (thread_platform_on_linux_t*)t->platform;
  t->state = TERMINATED;
  t->call = 0;
  pthread_cond_signal(&platform->cond);
  return 0;
}

int join_on_linux(thread_t* t) {
  thread_platform_on_linux_t* platform = t->platform;
  pthread_cond_wait(&platform->cond_join, &platform->mutex_join);
  return 0;
}

thread_operation_t thread_operation_on_linux = {
  .start = start_on_linux,
  .suspend = suspend_on_linux,
  .destroy = destroy_on_linux,
  .join = join_on_linux
};

