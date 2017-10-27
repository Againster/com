
#include "lz_queue.h"

/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

lz_queue_t *
lz_queue_middle(lz_queue_t *queue)
{
    lz_queue_t  *middle, *next;
    middle = lz_queue_head(queue);
    if (middle == lz_queue_last(queue)) {
        return middle;
    }

    next = lz_queue_head(queue);
    for ( ;; ) {
        middle = lz_queue_next(middle);
        next = lz_queue_next(next);
        if (next == lz_queue_last(queue)) {
            return middle;
        }

        next = lz_queue_next(next);
        if (next == lz_queue_last(queue)) {
            return middle;
        }
    }
}

/* the stable insertion sort */
void
lz_queue_sort(lz_queue_t *queue,
    int (*cmp)(const lz_queue_t *, const lz_queue_t *))
{
    lz_queue_t  *q, *prev, *next;

    q = lz_queue_head(queue);
    if (q == lz_queue_last(queue)) {
        return;
    }

    for (q = lz_queue_next(q); q != lz_queue_sentinel(queue); q = next) {
        prev = lz_queue_prev(q);
        next = lz_queue_next(q);
        lz_queue_remove(q);

        do {
            if (cmp(prev, q) <= 0) {
                break;
            }
            prev = lz_queue_prev(prev);
        } while (prev != lz_queue_sentinel(queue));

        lz_queue_insert_after(prev, q);
    }
}
