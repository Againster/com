/**
 * @file lz_queue.h
 * @date 2016/07/27 16:11:50
 * @author lissa,lissaagainster@yahoo.com
 * @copyright lissa
 * @brief retrofit from nginx-1.9.9/src/core/ngx_queue.h & ngx_queue.c
 */

#ifndef _LZ_QUEUE_H_INCLUDED_
#define _LZ_QUEUE_H_INCLUDED_


typedef struct lz_queue_s  lz_queue_t;

struct lz_queue_s {
    lz_queue_t  *prev;
    lz_queue_t  *next;
};


#define lz_queue_init(q)                                                      \
    (q)->prev = q;                                                            \
    (q)->next = q


#define lz_queue_empty(h)                                                     \
    (h == (h)->prev)


#define lz_queue_insert_head(h, x)                                            \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x


#define lz_queue_insert_after   lz_queue_insert_head


#define lz_queue_insert_tail(h, x)                                            \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x


#define lz_queue_head(h)                                                      \
    (h)->next


#define lz_queue_last(h)                                                      \
    (h)->prev


#define lz_queue_sentinel(h)                                                  \
    (h)


#define lz_queue_next(q)                                                      \
    (q)->next


#define lz_queue_prev(q)                                                      \
    (q)->prev


#if (LZ_DEBUG)

#define lz_queue_remove(x)                                                    \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else

#define lz_queue_remove(x)                                                    \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif


#define lz_queue_split(h, q, n)                                               \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;


#define lz_queue_add(h, n)                                                    \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;


#define lz_queue_data(q, type, link)                                          \
    (type *) ((char *) q - offsetof(type, link))


lz_queue_t *lz_queue_middle(lz_queue_t *queue);
void lz_queue_sort(lz_queue_t *queue,
    int (*cmp)(const lz_queue_t *, const lz_queue_t *));


#endif /* _LZ_QUEUE_H_INCLUDED_ */
