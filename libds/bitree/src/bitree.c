#include "bitree.h"

void bitree_init(struct bitree *tree, void (*free_data)(void *data))
{
	if (!tree)
		return;

	tree->size = 0;
	tree->root = NULL;
	tree->free_data = free_data;
}

void bitree_destroy(struct bitree *tree)
{
	struct bitree_node *root;

	if (!tree || bitree_is_empty(tree))
		return;

	bitree_rem_left(tree, NULL);

	memset(tree, 0, sizeof(struct bitree));

	return;
}

void bitree_rem_left(struct bitree *tree, struct bitree_node *node)
{
	struct bitree_node **left;
	int cnt;

	if (!tree || bitree_is_empty(tree))
		return;

	if (node)
		left = &node->left;
	else
		left = &tree->root;

	if (*left == NULL)
		return; 

	if ((*left)->left)
		bitree_rem_left(tree, *left);
	if ((*left)->right)
		bitree_rem_right(tree, *left);

	if (tree->free_data != NULL) {
		tree->free_data((*left)->data);
	}

	free(*left);
	*left = NULL;

	tree->size--;

	return;
}

void bitree_rem_right(struct bitree *tree, struct bitree_node *node)
{
	struct bitree_node **right;
	int cnt;

	if (!tree || bitree_is_empty(tree))
		return;

	if (node)
		right = &node->right;
	else
		right = &tree->root;

	if (*right == NULL)
		return;

	if ((*right)->left)
		bitree_rem_left(tree, *right);
	if ((*right)->right)
		bitree_rem_right(tree, *right);

	if (tree->free_data != NULL) {
		tree->free_data((*right)->data);
	}

	free(*right);
	*right = NULL;

	tree->size--;

	return;
}

struct bitree_node *bitree_alloc_node(void *data)
{
	struct bitree_node *new;

	new = (struct bitree_node *) malloc(sizeof(struct bitree_node));
	if (!new)
		return NULL;

	new->data = data;
	new->left = new->right = NULL;

	return new;
}

int bitree_ins_left(struct bitree *tree, struct bitree_node *node, const void *data)
{
	struct bitree_node *new;

	if (!tree)
		return -1;

	new = bitree_alloc_node((void *) data);
	if (!new)
		return -1;

	if (node) {
		new->left = node->left;
		node->left = new;
	} else {
		if (bitree_is_empty(tree))
			tree->root = new;
		else {
			free(new);
			return -1;
		}
	}

	tree->size++;

	return 0;
}

int bitree_ins_right(struct bitree *tree, struct bitree_node *node, const void *data)
{
	struct bitree_node *new;

	if (!tree)
		return -1;

	new = bitree_alloc_node((void *) data);
	if (!new)
		return -1;

	if (node) {
		new->right = node->right;
		node->right = new;
	} else {
		if (bitree_is_empty(tree))
			tree->root = new;
		else {
			free(new);
			return -1;
		}
	}

	tree->size++;

	return 0;
}

int bitree_merge(struct bitree *merge, struct bitree *left, struct bitree *right, const void *data)
{
	struct bitree_node *new;

	if (!merge || (!right && !left))
		return -1;

	new = bitree_alloc_node((void *) data);
	if (!new)
		return -1;

	merge->root = new;
	merge->size = 1;

	if (left) {
		new->left = left->root;
		merge->size += left->size;
	}

	if (right) {
		new->right = right->root;
		merge->size += right->size;
	}

	return 0;
}

