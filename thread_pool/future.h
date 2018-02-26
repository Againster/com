#ifndef FUTURE_H
#define FUTURE_H

#include "callable.h"

typedef struct task_s {
  callable_pt call;
  void*       argv;
  void*       ret;
  char        preliminary:1;
  char        running:1;
  char        done:1;
  char        interrupted:1;
} task_t;

typedef int   (*cancel_pt)(int may_interrupt_if_running);
typedef void* (*get_pt)(unsigned long timeout);
typedef int   (*is_canceled_pt)();
typedef int   (*is_done_pt)();

typedef struct task_operation_s {
  cancel_pt       cancel;
  get_pt          get;
  is_canceled_pt  canceled;
  is_done_pt      done;
} task_operation_t;

extern task_operation_t task_operation;

#endif
