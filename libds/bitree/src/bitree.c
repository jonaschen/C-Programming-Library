#include "stack.h"
#include "bitree.h"
#include "queue.h"

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

	root = tree->root;
	if (bitree_left(root))
		bitree_rem_left(tree, NULL);

	if (bitree_right(root))
		bitree_rem_right(tree, NULL);

	bitree_free_node(root);

	memset(tree, 0, sizeof(struct bitree));
}

int preorder_traversal(struct bitree_node *root, int (*visit)(struct bitree_node *))
{
	struct bitree_node *node = root;
	struct stack_t parent_stack, *parent = &parent_stack;
	int cnt = 0;

	if (visit == NULL)
		return -1;

	stack_init(parent, NULL);

	while (!stack_is_empty(parent) || node) {
		if (node) {
			visit(node);
			cnt++;
			if (node->right)
				stack_push(parent, (const void *) node->right);
			node = node->left;
		} else {
			stack_pop(parent, (void **) &node);
		}
	}

	return cnt;
}

int inorder_traversal(struct bitree_node *root, int (*visit)(struct bitree_node *))
{
	struct bitree_node *node = root;
	struct stack_t parent_stack, *parent = &parent_stack;
	int cnt = 0;

	if (visit == NULL)
		return -1;

	stack_init(parent, NULL);

	while (!stack_is_empty(parent) || node) {
		if (node) {
			stack_push(parent, (const void *) node);
			node = node->left;
		} else {
			stack_pop(parent, (void **) &node);
			visit(node);
			cnt++;
			node = node->right;
		}
	}

	return cnt;
}

int postorder_traversal(struct bitree_node *root, int (*visit)(struct bitree_node *))
{
	struct bitree_node *node = root;
	struct bitree_node *last_visited = NULL, *peek;
	struct stack_t parent_stack, *parent = &parent_stack;
	int cnt = 0;

	if (visit == NULL)
		return -1;

	stack_init(parent, NULL);

	while (!stack_is_empty(parent) || node) {
		if (node) {
			stack_push(parent, (const void *) node);
			node = node->left;
		} else {
			peek = (struct bitree_node *) stack_peek(parent);
			if (peek->right && last_visited != peek->right) {
				node = peek->right;
			} else {
				visit(peek);
				cnt++;
				stack_pop(parent, (void **) &last_visited);
			}
		}
	}

	return cnt;
}

int levelorder_traversal(struct bitree_node *root, int (*visit)(struct bitree_node *), int *depth)
{
	struct queue_t node_queue, *q = &node_queue;
	struct bitree_node *node = NULL;
	int cnt = 0, level = 0;
	struct bitree_node *most_right = root, *next_level = root;

	if (visit == NULL || !root)
		return -1;

	level = 0;

	queue_init(q, NULL);
	queue_enqueue(q, (const void *) root);

	while (queue_size(q)) {
		queue_dequeue(q, (void **) &node);
		visit(node);
		cnt++;
		if (node->left) {
			queue_enqueue(q, node->left);
			if (node == most_right)
				next_level = node->left;
		}

		if (node->right) {
			queue_enqueue(q, node->right);
			if (node == most_right)
				next_level = node->right;
		}

		if (most_right != next_level) {
			level++;
			most_right = next_level;
		}
	}

	if (depth)
		*depth = level;

	return cnt;
}

void bitree_rem_left(struct bitree *tree, struct bitree_node *node)
{
	struct bitree_node *left;
	int cnt;

	if (!tree || bitree_is_empty(tree))
		return;

	if (node)
		left = node->left;
	else
		left = tree->root->left;

	cnt = postorder_traversal(left, bitree_free_node);
	if (cnt < 0) {
		/* TODO: error handling */
	} else {
		tree->size -= cnt;
		if (node)
			node->left = NULL;
		else
			tree->root->left = NULL;
	}
}

void bitree_rem_right(struct bitree *tree, struct bitree_node *node)
{
	struct bitree_node *right;
	int cnt;

	if (!tree || bitree_is_empty(tree))
		return;

	if (node)
		right = node->right;
	else
		right = tree->root->right;

	cnt = postorder_traversal(right, bitree_free_node);
	if (cnt < 0) {
		/* TODO: error handling */
	} else {
		tree->size -= cnt;
		if (node)
			node->right = NULL;
		else
			tree->root->right = NULL;
	}
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
