#ifndef __BITREE_H
#define __BITREE_H

#include "common.h"

struct bitree_node {
	void *data;

	struct bitree_node *left;
	struct bitree_node *right;
};

struct bitree {
	int	size;

	void	(*free_data)(void *data);
	int	(*compare)(const void *key1, const void *key2);

	struct bitree_node *root;
};

struct bitree_node *bitree_alloc_node(void *data);

void bitree_init(struct bitree *tree, void (*free_data)(void *data));
void bitree_destroy(struct bitree *tree);

void bitree_rem_left(struct bitree *tree, struct bitree_node *node);
void bitree_rem_right(struct bitree *tree, struct bitree_node *node);
int bitree_ins_left(struct bitree *tree, struct bitree_node *node, const void *data);
int bitree_ins_right(struct bitree *tree, struct bitree_node *node, const void *data);
int bitree_merge(struct bitree *merge, struct bitree *left, struct bitree *right, const void *data);

#define bitree_size(tree)	((tree)->size)
#define bitree_root(tree)	((tree)->root)
#define bitree_is_empty(tree)	(bitree_root(tree) == NULL)
#define bitree_data(node)	((node)->data)
#define bitree_left(node)	((node)->left)
#define bitree_right(node)	((node)->right)

#endif
