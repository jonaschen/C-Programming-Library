#ifndef __BITREE_H
#define __BITREE_H

#include <stdlib.h>

struct bitree_node {
	void *data;

	struct bitree_node *left;
	struct bitree_node *right;
};

struct bitree {
	int	size;

	int	(*visit)(struct bitree_node *node);
	void	(*free_data)(void *data);

	struct bitree_node *root;
};

void bitree_init(struct bitree *tree,
		 void (*free_data)(void *data),
		 int (*visit)(struct bitree_node *node));

#define bitree_size(tree)	((tree)->size)
#define bitree_root(tree)	((tree)->root)
#define bitree_data(node)	((node)->data)
#define bitree_left(node)	((node)->left)
#define bitree_right(node)	((node)->right)

#endif
