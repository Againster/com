#include <stdio.h>
#include <unistd.h>
#include "executor_service.h"
#include "thread_factory.h"

void* counter(void* x) {
  long i = 0;
  printf("%s over\n", __func__);
  return x;
}

int main () {
  void* i= (void*)0;
  thread_t* t = 0;
  for (; i < (void*)3; i++) {
    t = (thread_t*)executor_service.submit(counter, i);
    thread_operation_on_linux.start(t);
  }
  sleep(1);
  executor_service.shutdown();
  printf("main thread exit\n");
  sleep(10);
  return 0;
}
