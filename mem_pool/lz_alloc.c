/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#include "lz_alloc.h"

uintptr_t  lz_pagesize;
uintptr_t  lz_pagesize_shift;
uintptr_t  lz_cacheline_size;

void* lz_alloc(size_t size, lz_log_t* log) {
  void* p = malloc(size);
  if (p == NULL) {
    log->h->log_handler(LZ_LOG_EMERG, log, errno, "malloc(%zu) failed", size);
  }
  log->h->log_handler(LZ_LOG_DEBUG, log, 0, "malloc: %p:%uz", p, size);
  return p;
}

void* lz_calloc(size_t size, lz_log_t* log) {
  void* p = lz_alloc(size, log);
  if (p) {
    memset(p, 0, size);
  }
  return p;
}

#if (LZ_HAVE_POSIX_MEMALIGN)

void* lz_memalign(size_t alignment, size_t size, lz_log_t* log) {
  void  *p;
  int  err;
  err = posix_memalign(&p, alignment, size);
  if (err) {
    log->h->log_handler(LZ_LOG_EMERG, log, err,
                        "posix_memalign(%zu, %zu) failed", alignment, size);
    p = NULL;
  }
  log->h->log_handler(LZ_LOG_DEBUG, log, 0,
                      "posix_memalign: %p:%uz @%uz", p, size, alignment);
  return p;
}

#elif (LZ_HAVE_MEMALIGN)

void* lz_memalign(size_t alignment, size_t size, lz_log_t* log) {
  void* p = memalign(alignment, size);
  if (p == NULL) {
    log->h->log_handler(LZ_LOG_EMERG, log, errno,
                        "memalign(%zu, %zu) failed", alignment, size);
  }
  log->h->log_handler(LZ_LOG_DEBUG, log, 0,
                      "posix_memalign: %p:%uz @%uz", p, size, alignment);
  return p;
}

#endif
