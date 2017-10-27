/**
 * Revision from nginx-1.9.9/src/core/ngx_palloc.c
 */
#include "lz_palloc.h"
static void* lz_palloc_block(lz_pool_t *pool, size_t size);
static void* lz_palloc_large(lz_pool_t *pool, size_t size);

lz_pool_t* lz_create_pool(size_t size, lz_log_t* log) {
  lz_pool_t* p;
  p = lz_memalign(LZ_POOL_ALIGNMENT, size, log);
  if (p == NULL) {
    return NULL;
  }
  p->d.last = (u_char *) p + sizeof(lz_pool_t);
  p->d.end = (u_char *) p + size;
  p->d.next = NULL;
  p->d.failed = 0;
  size = size - sizeof(lz_pool_t);
  p->max = (size < LZ_MAX_ALLOC_FROM_POOL) ? size : LZ_MAX_ALLOC_FROM_POOL;
  p->current = p;
  p->chain = NULL;
  p->large = NULL;
  p->cleanup = NULL;
  p->log = log;
  return p;
}

void lz_destroy_pool(lz_pool_t *pool) {
  lz_pool_t          *p, *n;
  lz_pool_large_t    *l;
  lz_pool_cleanup_t  *c;
  for (c = pool->cleanup; c; c = c->next) {
    if (c->handler) {
      pool->log->h->log_handler(LZ_LOG_DEBUG, pool->log, 0,
                                "run cleanup: %p", c);
      c->handler(c->data);
    }
  }
  for (l = pool->large; l; l = l->next) {
    pool->log->h->log_handler(LZ_LOG_DEBUG, pool->log, 0, "free: %p", l->alloc);
    if (l->alloc) {
      lz_free(l->alloc);
    }
  }
#if (LZ_DEBUG)
  /*
   * we could allocate the pool->log from this pool
   * so we cannot use this log while free()ing the pool
   */
  for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
    pool->log->h->log_handler(LZ_LOG_DEBUG, pool->log, 0,
                              "free: %p, unused: %uz", p, p->d.end - p->d.last); 
    if (n == NULL) {
      break;
    }
  }
#endif
  for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
    lz_free(p);
    if (n == NULL) {
      break;
    }
  }
}

void lz_reset_pool(lz_pool_t *pool) {
  lz_pool_t        *p;
  lz_pool_large_t  *l;
  for (l = pool->large; l; l = l->next) {
    if (l->alloc) {
      lz_free(l->alloc);
    }
  }
  for (p = pool; p; p = p->d.next) {
    p->d.last = (u_char *) p + sizeof(lz_pool_t);
    p->d.failed = 0;
  }
  pool->current = pool;
  pool->chain = NULL;
  pool->large = NULL;
}

void* lz_palloc(lz_pool_t *pool, size_t size) {
  u_char     *m;
  lz_pool_t  *p;
  if (size <= pool->max) {
    p = pool->current;
    do {
      m = lz_align_ptr(p->d.last, LZ_ALIGNMENT);
      if ((size_t) (p->d.end - m) >= size) {
        p->d.last = m + size;
        return m;
      }
      p = p->d.next;
    } while (p);
    return lz_palloc_block(pool, size);
  }
  return lz_palloc_large(pool, size);
}

void * lz_pnalloc(lz_pool_t *pool, size_t size) {
  u_char    *m;
  lz_pool_t  *p;
  if (size <= pool->max) {
    p = pool->current;
    do {
      m = p->d.last;
      if ((size_t) (p->d.end - m) >= size) {
        p->d.last = m + size;
        return m;
      }
      p = p->d.next;
    } while (p);
    return lz_palloc_block(pool, size);
  }
  return lz_palloc_large(pool, size);
}

static void * lz_palloc_block(lz_pool_t *pool, size_t size) {
  u_char    *m;
  size_t     psize;
  lz_pool_t  *p, *new;
  psize = (size_t) (pool->d.end - (u_char *) pool);
  m = lz_memalign(LZ_POOL_ALIGNMENT, psize, pool->log);
  if (m == NULL) {
    return NULL;
  }
  new = (lz_pool_t *) m;
  new->d.end = m + psize;
  new->d.next = NULL;
  new->d.failed = 0;
  m += sizeof(lz_pool_data_t);
  m = lz_align_ptr(m, LZ_ALIGNMENT);
  new->d.last = m + size;
  for (p = pool->current; p->d.next; p = p->d.next) {
    if (p->d.failed++ > 4) {
      pool->current = p->d.next;
    }
  }
  p->d.next = new;
  return m;
}

static void * lz_palloc_large(lz_pool_t *pool, size_t size) {
  void        *p;
  uintptr_t     n;
  lz_pool_large_t  *large;
  p = lz_alloc(size, pool->log);
  if (p == NULL) {
    return NULL;
  }
  n = 0;
  for (large = pool->large; large; large = large->next) {
    if (large->alloc == NULL) {
      large->alloc = p;
      return p;
    }
    if (n++ > 3) {
      break;
    }
  }
  large = lz_palloc(pool, sizeof(lz_pool_large_t));
  if (large == NULL) {
    lz_free(p);
    return NULL;
  }
  large->alloc = p;
  large->next = pool->large;
  pool->large = large;
  return p;
}

void* lz_pmemalign(lz_pool_t *pool, size_t size, size_t alignment) {
  void             *p;
  lz_pool_large_t  *large;
  p = lz_memalign(alignment, size, pool->log);
  if (p == NULL) {
    return NULL;
  }
  large = lz_palloc(pool, sizeof(lz_pool_large_t));
  if (large == NULL) {
    lz_free(p);
    return NULL;
  }
  large->alloc = p;
  large->next = pool->large;
  pool->large = large;
  return p;
}

intptr_t lz_pfree(lz_pool_t *pool, void *p) {
  lz_pool_large_t  *l;
  for (l = pool->large; l; l = l->next) {
    if (p == l->alloc) {
      pool->log->h->log_handler(LZ_LOG_DEBUG, pool->log, 0,
                                "free: %p", l->alloc);
      lz_free(l->alloc);
      l->alloc = NULL;
      return LZ_OK;
    }
  }
  return LZ_DECLINED;
}

void * lz_pcalloc(lz_pool_t *pool, size_t size) {
  void *p;
  p = lz_palloc(pool, size);
  if (p) {
    lz_memzero(p, size);
  }
  return p;
}

lz_pool_cleanup_t* lz_pool_cleanup_add(lz_pool_t *p, size_t size) {
  lz_pool_cleanup_t  *c;
  c = lz_palloc(p, sizeof(lz_pool_cleanup_t));
  if (c == NULL) {
    return NULL;
  }
  if (size) {
    c->data = lz_palloc(p, size);
    if (c->data == NULL) {
      return NULL;
    }
  } else {
    c->data = NULL;
  }
  c->handler = NULL;
  c->next = p->cleanup;
  p->cleanup = c;
  p->log->h->log_handler(LZ_LOG_DEBUG, p->log, 0, "add cleanup: %p", c);
  return c;
}

void lz_pool_run_cleanup_file(lz_pool_t *p, int fd) {
  lz_pool_cleanup_t       *c;
  lz_pool_cleanup_file_t  *cf;
  for (c = p->cleanup; c; c = c->next) {
    if (c->handler == lz_pool_cleanup_file) {
      cf = c->data;
      if (cf->fd == fd) {
        c->handler(cf);
        c->handler = NULL;
        return;
      }
    }
  }
}

void lz_pool_cleanup_file(void *data) {
  lz_pool_cleanup_file_t  *c = data;
  c->log->h->log_handler(LZ_LOG_DEBUG, c->log, 0, "file cleanup: fd:%d", c->fd);
  if (lz_close_file(c->fd) == LZ_FILE_ERROR) {
    c->log->h->log_handler(LZ_LOG_DEBUG, c->log, 0, " \"%s\" failed", c->name);
  }
}

void lz_pool_delete_file(void *data) {
  lz_pool_cleanup_file_t  *c = data;
  int err;
  c->log->h->log_handler(LZ_LOG_DEBUG, c->log, 0,
                         "file cleanup: fd:%d %s", c->fd, c->name);
  if (lz_delete_file(c->name) == LZ_FILE_ERROR) {
    err = errno;
    if (err != ENOENT) {
      c->log->h->log_handler(LZ_LOG_CRIT, c->log, err,
                             " \"%s\" failed", c->name);
    }
  }
  if (lz_close_file(c->fd) == LZ_FILE_ERROR) {
    c->log->h->log_handler(LZ_LOG_ALERT, c->log, errno,
                           " \"%s\" failed", c->name);
  }
}

