#include <stdio.h>

#include "bitree.h"

#define TEST_NUM	3

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

	if (bitree_ins_left(tree, NULL, (const void *) &data[1]) != 0)
		exit(EXIT_FAILURE);

	bitree_ins_left(tree, tree->root, (const void *) &data[0]);
	bitree_ins_right(tree, tree->root, (const void *) &data[2]);
}

int visit(struct bitree_node *node)
{
	if (!node || !node->data)
		return -1;

	printf("data: %d\n", *(int *) node->data);

	return 0;
}

int main(int argc, char *argv[])
{
	struct bitree extree, *tree = &extree;
	int cnt;

	bitree_init(tree, free);

	build123(tree);

	cnt = preorder_traversal(tree->root, visit);
	printf("preorder: total %d nodes traversed\n", cnt);

	cnt = inorder_traversal(tree->root, visit);
	printf("inorder: total %d nodes traversed\n", cnt);

	cnt = postorder_traversal(tree->root, visit);
	printf("postorder: total %d nodes traversed\n", cnt);

	cnt = levelorder_traversal(tree->root, visit);
	printf("levelorder: total %d nodes traversed\n", cnt);

	exit(EXIT_SUCCESS);
}
