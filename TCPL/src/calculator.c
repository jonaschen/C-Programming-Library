#include <stdio.h>

#include "bitree.h"


int main(int argc, char *argv[])
{
	struct bitree *tree;

	tree = (struct bitree *) malloc(sizeof(struct bitree));
	if (!tree)
		return -1;

	bitree_init(tree, free);

	bitree_destroy(tree);

	return 0;
}
