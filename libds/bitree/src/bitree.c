#include "stack.h"
#include "bitree.h"
#include "queue.h"

void bitree_init(struct bitree *tree,
		 void (*free_data)(void *data),
		 int (*visit)(struct bitree_node *node))
{
	if (!tree)
		return;

	tree->size = 0;
	tree->root = NULL;
	tree->free_data = free_data;
	tree->visit = visit;
}

void preorder_traversal(struct bitree *tree)
{
	struct bitree_node *node = tree->root;
	int (*visit)(struct bitree_node *) = tree->visit;
	struct stack_t parent_stack, *parent = &parent_stack;

	stack_init(parent, NULL);

	while (stack_size(parent) || node) {
		if (node) {
			visit(node);
			if (node->right)
				stack_push(parent, (const void *) node->right);
			node = node->left;
		} else {
			stack_pop(parent, (void **) &node);
		}
	}
}

void inorder_traversal(struct bitree *tree)
{
	struct bitree_node *node = tree->root;
	int (*visit)(struct bitree_node *) = tree->visit;
	struct stack_t parent_stack, *parent = &parent_stack;

	stack_init(parent, NULL);

	while (stack_size(parent) || node) {
		if (node) {
			stack_push(parent, (const void *) node);
			node = node->left;
		} else {
			stack_pop(parent, (void **) &node);
			visit(node);
			node = node->right;
		}
	}
}

void postorder_traversal(struct bitree *tree)
{
	struct bitree_node *node = tree->root;
	int (*visit)(struct bitree_node *) = tree->visit;
	struct bitree_node *last_visited = NULL, *peek;
	struct stack_t parent_stack, *parent = &parent_stack;

	stack_init(parent, NULL);

	while (stack_size(parent) || node) {
		if (node) {
			stack_push(parent, (const void *) node);
			node = node->left;
		} else {
			peek = (struct bitree_node *) stack_peek(parent);
			if (peek->right && last_visited != peek->right) {
				node = peek->right;
			} else {
				visit(peek);
				stack_pop(parent, (void **) &last_visited);
			}
		}
	}
}

void levelorder_traversal(struct bitree *tree)
{
	struct bitree_node *root = tree->root;
	int (*visit)(struct bitree_node *) = tree->visit;
	struct queue_t node_queue, *q = &node_queue;
	struct bitree_node *node = NULL;

	queue_init(q, NULL);
	queue_enqueue(q, (const void *) root);

	while (queue_size(q)) {
		queue_dequeue(q, (void **) &node);
		visit(node);
		if (node->left)
			queue_enqueue(q, node->left);

		if (node->right)
			queue_enqueue(q, node->right);
	}
}
