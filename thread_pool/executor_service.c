#include "executor_service.h"
#include "thread_factory.h"
#include "lz_queue.h"
#include "future.h"
#include <stddef.h>
#include <malloc.h>

typedef struct thread_queue_s {
  thread_t*               thread;
  lz_queue_t              queue;
} thread_queue_t;

typedef struct thread_task_queue_s {
  task_t*                 task;
  lz_queue_t              queue;
} thread_task_queue_t;

typedef enum EXECUTOR_SERVICE_FIXED_SCHEDULE_CATEGORY {
  FIFO,
  LIFO
} SCHEDULE;

typedef struct executor_service_fixed_s {
  thread_factory_t        factory;
  thread_operation_t      op;
  thread_queue_t          idle;
  thread_queue_t          busy;
  thread_task_queue_t     task;
  int                     n_busy;
  int                     n_idle;
  int                     n_task;
  int                     n_limited;
  SCHEDULE                category;
  char                    first:1;
} executor_service_fixed_t;

executor_service_fixed_t executor_service_fixed;

future_t* submit_fixed(callable_pt task, void* argv);

void* thread_precall(void* v) {
  /*
  thread_queue_t* thread_queue = (thread_queue_t*)v;
  lz_queue_t* queue = &thread_queue->queue;
  thread_t* thread = thread_queue->thread;
  */
  return 0;
}

void* thread_postcall(void* v) {
  thread_task_queue_t* thread_task_queue = NULL;
  lz_queue_t* task_queue = NULL;
  callable_pt call = NULL;
  void* argv = NULL;

  thread_queue_t* thread_queue = (thread_queue_t*)v;
  lz_queue_t* queue = &thread_queue->queue;
  thread_t* thread = thread_queue->thread;

  //append completed task to idle_queue
  lz_queue_remove(queue);
  executor_service_fixed.n_busy--;

  if (thread->state == TERMINATED) {
    return 0;
  } else {
    thread->state = NEW;
  }

  lz_queue_insert_tail(&executor_service_fixed.idle.queue, queue);
  executor_service_fixed.n_idle++;

  //check task_queue
  task_queue = lz_queue_head(&executor_service_fixed.task.queue);
  if (lz_queue_empty(task_queue)) {
    return 0;
  }

  thread_task_queue = lz_queue_data(task_queue, thread_task_queue_t, queue);
  call = thread_task_queue->task->call;
  argv = thread_task_queue->task->argv;
  lz_queue_remove(task_queue);

  //resubmit the task_queue
  submit_fixed(call, argv);
  executor_service_fixed.op.start(thread);
  return 0;
}

int parse_executor_service_fixed_argv (int argc, char** argv) {
  return 0;
}

int init_fixed(int argc, char** argv) {
  int ret = parse_executor_service_fixed_argv(argc, argv);
  if (ret != 0) {
    return 0;
  }
  executor_service_fixed.factory = thread_factory_on_linux;
  executor_service_fixed.thread = thread_operation_on_linux;
  lz_queue_init(&executor_service_fixed.idle.queue);
  lz_queue_init(&executor_service_fixed.busy.queue);
  lz_queue_init(&executor_service_fixed.will.queue);
  executor_service_fixed.n_idle = 0;
  executor_service_fixed.n_busy = 0;
  executor_service_fixed.n_will = 0;
  executor_service_fixed.n_limited = 1;
  return 0;
}

task_t* submit_fixed(callable_pt call, void* argv) {
  lz_queue_t* queue = NULL;
  thread_t* thread = NULL;
  thread_queue_t* thread_queue = NULL;
  thread_task_queue_t* thread_task_queue = NULL;

  if (executor_service_fixed.first == 0) {
    init_fixed(0, 0);
    executor_service_fixed.first = 1;
  }
  if (executor_service_fixed.n_idle > 0) {
    queue = lz_queue_head(&executor_service_fixed.idle.queue);
    lz_queue_remove(queue);
    executor_service_fixed.n_idle--;

    lz_queue_insert_tail(&executor_service_fixed.busy.queue, queue);
    executor_service_fixed.n_busy++;

    thread_queue = lz_queue_data(queue, thread_queue_t, queue);
    thread = thread_queue->thread;

    thread->call = call;
    thread->argv = argv;
    thread->ret = 0;
    printf("designate idle thread %d\n", (int)thread->id);
  } else if (executor_service_fixed.n_idle <= 0 &&
      executor_service_fixed.n_busy < executor_service_fixed.n_limited) {
    thread =  executor_service_fixed.factory.new_thread();
    thread_queue = (thread_queue_t*)malloc(sizeof(thread_queue_t));

    thread_queue->thread = thread;

    thread->precall = thread_precall;
    thread->postcall = thread_postcall;
    thread->precall_argv = thread_queue;
    thread->postcall_argv = thread_queue;

    queue = &thread_queue->queue;
    lz_queue_insert_tail(&executor_service_fixed.busy.queue, queue);
    executor_service_fixed.n_busy++;
    printf("designate new thread %d\n", (int)thread->id);
  } else {
    printf("designate will thread\n");
    thread_task_queue = (thread_task_queue_t*)
      malloc(sizeof(thread_task_queue_t));
    task_t
    thread_task_queue->call = call;
    thread_task_queue->argv = argv;
    queue = &thread_task_queue->queue;
    lz_queue_insert_tail(&executor_service_fixed.task.queue, queue);
    executor_service_fixed.n_task++;

    thread = 0;
  }
  return (task_t*)thread;
}

void shutdown_fixed() {
  thread_queue_t*           thread_queue;
  thread_t*                 thread;
  lz_queue_t*               queue;
  lz_queue_t*               queue_sentinel;

  //destroy idle thread
  queue_sentinel = lz_queue_sentinel(&executor_service_fixed.idle.queue);
  queue = lz_queue_next(queue_sentinel);
  while(queue != queue_sentinel) {
    thread_queue = lz_queue_data(queue, thread_queue_t, queue);
    thread = thread_queue->thread;
    executor_service_fixed.thread.destroy(thread);
    printf("idle thread will exit %d\n", (int)thread->id);
    queue = lz_queue_next(queue);
  }

  //destroy busy thread
  queue_sentinel = lz_queue_sentinel(&executor_service_fixed.busy.queue);
  queue = lz_queue_next(queue_sentinel);
  while(queue != queue_sentinel) {
    thread_queue = lz_queue_data(queue, thread_queue_t, queue);
    thread = thread_queue->thread;
    executor_service_fixed.thread.destroy(thread);
    printf("busy thread will exit %d\n", (int)thread->id);
    queue = lz_queue_next(queue);
  }
}

executor_service_t executor_service = {
  .init = init_fixed,
  .submit = submit_fixed,
  .shutdown = shutdown_fixed
};
