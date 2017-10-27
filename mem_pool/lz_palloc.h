/**
 * @file lz_palloc.h
 * @date 2016/04/07 14:36:16
 * @author lissa,lissaagainster@yahoo.com
 * @copyright lissa
 * @brief Revision from nginx-1.9.9, this file changes from
 * nginx-1.9.9/src/core/ngx_palloc.h
 */

#ifndef __LZ_PALLOC_H
#define __LZ_PALLOC_H

#include "lz_alloc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * LZ_MAX_ALLOC_FROM_POOL should be (lz_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define LZ_MAX_ALLOC_FROM_POOL (getpagesize() - 1)
#define LZ_DEFAULT_POOL_SIZE (16 * 1024)
#define LZ_POOL_ALIGNMENT 16
#define LZ_ALIGNMENT sizeof(unsigned long)

#define lz_align(d, a) (((d) + (a - 1)) & ~(a - 1))
#define lz_align_ptr(p, a)                                                    \
  (u_char*) (((uintptr_t)(p) + ((uintptr_t)a - 1)) & ~((uintptr_t)a - 1))

#define LZ_MIN_POOL_SIZE                                                      \
  lz_align((sizeof(lz_pool_t) + 2 * sizeof(lz_pool_large_t)),                 \
           LZ_POOL_ALIGNMENT)

#define lz_close_file close
#define lz_delete_file(name) unlink((const char*) name)
#define lz_memzero(p, s) memset(p, 0, s)

#define LZ_DEBUG 1

#define LZ_OK 0
#define LZ_FILE_ERROR -1
#define LZ_DECLINED -5

struct lz_pool_s;
typedef struct lz_pool_s lz_pool_t;
typedef void (*lz_pool_cleanup_pt)(void* data);
typedef struct lz_pool_cleanup_s lz_pool_cleanup_t;

struct lz_pool_cleanup_s {
  lz_pool_cleanup_pt  handler;
  void                *data;
  lz_pool_cleanup_t   *next;
};

typedef struct lz_pool_large_s  lz_pool_large_t;
struct lz_pool_large_s {
  lz_pool_large_t     *next;
  void                *alloc;
};

typedef struct {
  u_char              *last;
  u_char              *end;
  lz_pool_t           *next;
  uintptr_t           failed;
} lz_pool_data_t;

struct lz_pool_s {
  lz_pool_data_t      d;
  size_t              max;
  lz_pool_t           *current;
  void                *chain;
  lz_pool_large_t     *large;
  lz_pool_cleanup_t   *cleanup;
  lz_log_t            *log;
};

typedef struct {
  int                 fd;
  u_char              *name;
  lz_log_t            *log;
} lz_pool_cleanup_file_t;

void* lz_alloc(size_t size, lz_log_t* log);
void* lz_calloc(size_t size, lz_log_t* log);
lz_pool_t* lz_create_pool(size_t size, lz_log_t* log);
void lz_destroy_pool(lz_pool_t* pool);
void lz_reset_pool(lz_pool_t* pool);

void* lz_palloc(lz_pool_t* pool, size_t size);
void* lz_pnalloc(lz_pool_t* pool, size_t size);
void* lz_pcalloc(lz_pool_t* pool, size_t size);
void* lz_pmemalign(lz_pool_t* pool, size_t size, size_t alignment);
intptr_t lz_pfree(lz_pool_t* pool, void* p);
lz_pool_cleanup_t* lz_pool_cleanup_add(lz_pool_t* p, size_t size);
void lz_pool_run_cleanup_file(lz_pool_t* p, int fd);
void lz_pool_cleanup_file(void* data);
void lz_pool_delete_file(void* data);

#ifdef __cplusplus
}
#endif

#endif /* __LZ_PALLOC_H */
