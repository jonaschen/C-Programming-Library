#ifndef __BST_H
#define __BST_H

#define	AVL_LFT_HEAVY	-1
#define AVL_BALANCED	0
#define AVL_RGT_HEAVY	1

struct bistree_node {
	void	*data;
	int	factor;
	struct bistree_node *left;
	struct bistree_node *right;
};

struct bistree {
	int	size;

	void	(*free_data)(void *data);
 
	struct bistree_node *root;
};

void bistree_init(struct bistree *tree,
		void (*compare)(const void *key1, const void *key2),
		void (*free_data)(void *data));

void bistree_destroy(struct bistree *tree);

int bistree_remove(struct bistree *tree, const void *data);
int bistree_lookup(const struct bistree *tree, void **data);

#define bistree_size(tree)	((tree)->size)

#endif
