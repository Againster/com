#ifndef THREAD_FACTORY_H
#define THREAD_FACTORY_H

#include "thread.h"

typedef thread_t* (*new_thread_pt)();

typedef struct thread_factory_s {
  new_thread_pt new_thread;
} thread_factory_t;

#if __linux__ == 1

extern thread_factory_t thread_factory_on_linux;

#endif // __linux__

#endif
