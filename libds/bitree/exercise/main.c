#include <stdio.h>

#include "bitree.h"

#define TEST_NUM	5

/* 
 *	TODO: need a more general rule to build nodes for
 *	      the example tree.
 */
void build123(struct bitree *tree)
{
	int i, n = TEST_NUM, *data;

	data = (int *) malloc(n * sizeof(int));
	if (!data)
		exit(EXIT_FAILURE);

	for (i = 0; i < n; i++)
		data[i] = i + 1;

	if (bitree_ins_left(tree, NULL, (const void *) &data[3]) != 0)
		exit(EXIT_FAILURE);

	bitree_ins_left(tree, tree->root, (const void *) &data[1]);
	bitree_ins_right(tree, tree->root, (const void *) &data[4]);

	bitree_ins_left(tree, tree->root->left, (const void *) &data[0]);
	bitree_ins_right(tree, tree->root->left, (const void *) &data[2]);
}

int visit(struct bitree_node *node)
{
	if (!node || !node->data)
		return -1;

	printf("data: %d\n", *(int *) node->data);

	return 0;
}

int min_value(struct bitree *tree)
{
	struct bitree_node *node = tree->root;

	if (!tree || !tree->root)
		return 0;

	while (node->left)
		node = node->left;

	return *((int *) node->data);
}

int main(int argc, char *argv[])
{
	struct bitree extree, *tree = &extree;
	int cnt, depth, min;

	bitree_init(tree, free);

	build123(tree);

	cnt = preorder_traversal(tree->root, visit);
	printf("preorder: total %d nodes traversed\n", cnt);

	cnt = inorder_traversal(tree->root, visit);
	printf("inorder: total %d nodes traversed\n", cnt);

	cnt = postorder_traversal(tree->root, visit);
	printf("postorder: total %d nodes traversed\n", cnt);

	cnt = levelorder_traversal(tree->root, visit, &depth);
	printf("levelorder: total %d nodes traversed, max depth:%d\n", cnt, depth);

	min = min_value(tree);
	printf("min valud: %d\n", min);

	exit(EXIT_SUCCESS);
}
