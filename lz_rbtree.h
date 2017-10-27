/**
 * @file lz_rbtree.h
 * @date 2017/06/06 09:24:00
 * @author lissa,lissaagainster@yahoo.com
 * @copyright lissa
 * @brief retrofit from nginx-1.9.9/src/core/ngx_rbtree.h & ngx_rbtree.c
 */

#ifndef _LZ_RBTREE_H_
#define _LZ_RBTREE_H_

#include <stdint.h> /** for intptr_t and uintptr_t */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL (void*)0
#endif

typedef uintptr_t lz_rbtree_key_t;
typedef intptr_t lz_rbtree_key_int_t;
typedef uintptr_t lz_uint_t;
typedef struct lz_rbtree_node_s  lz_rbtree_node_t;


struct lz_rbtree_node_s {
    lz_rbtree_key_t       key;
    lz_rbtree_node_t     *left;
    lz_rbtree_node_t     *right;
    lz_rbtree_node_t     *parent;
    uint8_t               color;
    uint8_t               data;
};


typedef struct lz_rbtree_s  lz_rbtree_t;

typedef void (*lz_rbtree_insert_pt) (lz_rbtree_node_t *root,
    lz_rbtree_node_t *node, lz_rbtree_node_t *sentinel);

struct lz_rbtree_s {
    lz_rbtree_node_t     *root;
    lz_rbtree_node_t     *sentinel;
    lz_rbtree_insert_pt   insert;
};


#define lz_rbtree_init(tree, s, i)                                           \
    lz_rbtree_sentinel_init(s);                                              \
    (tree)->root = s;                                                         \
    (tree)->sentinel = s;                                                     \
    (tree)->insert = i


void lz_rbtree_insert(lz_rbtree_t *tree, lz_rbtree_node_t *node);
void lz_rbtree_delete(lz_rbtree_t *tree, lz_rbtree_node_t *node);
void lz_rbtree_insert_value(lz_rbtree_node_t *root, lz_rbtree_node_t *node,
    lz_rbtree_node_t *sentinel);
void lz_rbtree_insert_timer_value(lz_rbtree_node_t *root,
    lz_rbtree_node_t *node, lz_rbtree_node_t *sentinel);
lz_rbtree_node_t *lz_rbtree_next(lz_rbtree_t *tree,
    lz_rbtree_node_t *node);


#define lz_rbt_red(node)               ((node)->color = 1)
#define lz_rbt_black(node)             ((node)->color = 0)
#define lz_rbt_is_red(node)            ((node)->color)
#define lz_rbt_is_black(node)          (!lz_rbt_is_red(node))
#define lz_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define lz_rbtree_sentinel_init(node)  lz_rbt_black(node)


static lz_rbtree_node_t*
lz_rbtree_min(lz_rbtree_node_t *node, lz_rbtree_node_t *sentinel)
{
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}

#ifdef __cplusplus
}
#endif

#endif /* _LZ_RBTREE_H_ */
