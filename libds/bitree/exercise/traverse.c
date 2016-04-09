#include "traverse.h"

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
