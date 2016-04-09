#include <string.h>

#include "exercise.h"

struct bitree *build(int n)
{
	int i, *data;
	struct bitree *tree = NULL;
	struct bitree_node *position = NULL;

	tree = (struct bitree *) malloc(sizeof(struct bitree));
	if (!tree)
		return NULL;

	bitree_init(tree, NULL);

	data = (int *) malloc(n * sizeof(int));
	if (!data) {
		goto fail;
	}

	for (i = 0; i <= n/2; i++) {
		data[i] = i + 1;
		if (bitree_ins_left(tree, position, (const void *) &data[i]) != 0)
			goto fail;

		if (!position) {
			if (bitree_root(tree) == NULL)
				goto fail;
			position = bitree_root(tree);
		} else {
			position = position->left;
		}
	}

	position = bitree_root(tree);
	for (i = n/2 + 1; i < n; i++) {
		data[i] = i + 1;
		if (bitree_ins_right(tree, position, (const void *) &data[i]) != 0)
			goto fail;

		position = position->right;
	}

	return tree;

fail:
	free(tree);
	return NULL;
}
