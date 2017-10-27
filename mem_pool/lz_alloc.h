/**
 * @file lz_alloc.h
 * @date 2016/04/06 20:29:44
 * @author lissa,lissaagainster@yahoo.com
 * @copyright lissa
 * @brief Revision from nginx-1.9.9 memory pool, this file changes from
 * nginx-1.9.9/src/os/unix/ngx_alloc.h
 */
#ifndef _LZ_ALLOC_H_
#define _LZ_ALLOC_H_

#include <stdint.h> /** uintptr_t, intpte_t */
#include <string.h> /** memset */
#include <stdlib.h> /** malloc */
#include <errno.h>  /** errno */
#include <unistd.h> /** getpagesize */

#include "lz_log_junction.h" /** log */
#ifdef __cplusplus
extern "C" {
#endif

void* lz_alloc(size_t size, lz_log_t* log);
void* lz_calloc(size_t size, lz_log_t* log);

#define lz_free free
/**
 * Linux has memalign() or posix_memalign()
 * Solaris has memalign()
 * FreeBSD 7.0 has posix_memalign(), besides, early version's malloc()
 * aligns allocations bigger than page size at the page boundary
 */

#if (LZ_HAVE_POSIX_MEMALIGN || LZ_HAVE_MEMALIGN)
void* lz_memalign(size_t alignment, size_t size, lz_log_t* log);
#else
#define lz_memalign(alignment, size, log) lz_alloc(size, log)
#endif /** LZ_HAVE_POSIX_MEMALIGN, LZ_HAVE_MEMALIGN */

extern uintptr_t lz_pagesize;
extern uintptr_t lz_pagesize_shift;
extern uintptr_t lz_cacheline_size;

#ifdef __cplusplus
}
#endif
#endif /** _LZ_ALLOC_H_ */
