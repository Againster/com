
#if __linux == 1
#include <pthread.h>
#include <malloc.h>
#include <signal.h>
#endif // __linux__

#include "thread_factory.h"

#if __linux__ == 1

void* start_routine_on_linux(void* v) {
  thread_t* t = (thread_t*)v;
  thread_platform_on_linux_t* platform = t->platform;
  while(1) {
    if (t->state == NEW)
      pthread_cond_wait(&platform->cond, &platform->mutex);

    if (t->precall)
      t->precall_ret = t->precall(t->precall_argv);

    if (t->call)
      t->ret = t->call(t->argv);

    if (t->postcall)
      t->postcall_ret = t->postcall(t->postcall_argv);

    //awake join thread
    pthread_cond_signal(&platform->cond_join);

    //switch over state
    if (t->state == TERMINATED)
      break;
  }
  printf("thread exit %d\n", (int)t->id);
  return NULL;
}

thread_t* new_thread_on_linux(){
  int ret = 0;
  thread_t* t = (thread_t*)malloc(sizeof(thread_t));
  thread_platform_on_linux_t* platform = (thread_platform_on_linux_t*)
    malloc(sizeof(thread_platform_on_linux_t));

  pthread_condattr_init(&platform->c_attr);
  pthread_cond_init(&platform->cond, &platform->c_attr);

  pthread_mutexattr_init(&platform->m_attr);
  pthread_mutex_init(&platform->mutex, &platform->m_attr);

  pthread_condattr_init(&platform->c_attr_join);
  pthread_cond_init(&platform->cond, &platform->c_attr_join);

  pthread_mutexattr_init(&platform->m_attr_join);
  pthread_mutex_init(&platform->mutex, &platform->m_attr_join);

  pthread_mutexattr_init(&platform->m_attr_state);
  pthread_mutex_init(&platform->mutex_state, &platform->m_attr_state);

  t->state = NEW;
  t->platform = platform;

  ret = pthread_create(&t->id, NULL, start_routine_on_linux, t);
  if (ret != 0) {
    printf("thread_factory create thread error");
    return NULL;
  }

  return t;
}

thread_factory_t thread_factory_on_linux = {
  .new_thread = new_thread_on_linux
};

#endif // __linux__
