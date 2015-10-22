#include "stack.h"
#include "bitree.h"

void visit(struct bitree_node *node)
{
}

void preorder_traversal(struct bitree_node *root)
{
	struct bitree_node *node = root;
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
