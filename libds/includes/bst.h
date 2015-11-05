#ifndef __BST_H
#define __BST_H

struct bistree_node {
};

struct bistree {
};

void bistree_init(struct bistree *tree,
		void (*compare)(const void *key1, const void *key2),
		void (*free_data)(void *data));

void bistree_destroy(struct bistree *tree);

int bistree_remove(struct bistree *tree, const void *data);
int bistree_lookup(const struct bistree *tree, void **data);

int bistree_size(const struct bistree *tree);

#endif
