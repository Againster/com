#ifndef EXECUTOR_SERVICE_H
#define EXECUTOR_SERVICE_H

#include "callable.h"
#include "future.h"

typedef int       (*init_pt)(int argc, char**argv);
typedef future_t* (*submit_pt)(callable_pt task, void* argv);
typedef void      (*shutdown_pt)();

typedef struct executor_service_s {
  init_pt init;
  submit_pt submit;
  shutdown_pt shutdown;
} executor_service_t;

extern executor_service_t executor_service;

#endif
