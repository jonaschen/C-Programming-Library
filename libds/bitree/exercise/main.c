#include <stdio.h>

#include "stack.h"
#include "bitree.h"
#include "exercise.h"

int visit(struct bitree_node *node)
{
	if (!node || !node->data)
		return -1;

	printf("data: %d\n", *(int *) node->data);

	return 0;
}

void print_path_sum(struct stack_t *paths)
{
	struct stack_elem *elem;
	struct bitree_node *node;
	struct stack_t path, *s = &path;
	int sum = 0;
	int data, *pdata;

	stack_init(s, NULL);

	for (elem = paths->top; elem; elem = elem->next) {
		node = (struct bitree_node *) elem->data;
		data = *(int *) node->data;
		sum += data;
		stack_push(s, node->data);
	}
	while (!stack_is_empty(s)) {
		stack_pop(s, (void **) &pdata);
		if (pdata)
			printf("%d, ", *pdata);
	}
	printf("\n sum:%d\n", sum);
}

int print_paths(struct bitree_node *root)
{
	struct bitree_node *node = root, *dummy;
	struct bitree_node *last_visited = NULL, *peek;
	struct stack_t parent_stack, *parent = &parent_stack;
	struct stack_t path_stack, *paths = &path_stack;
	int cnt = 0;

	stack_init(parent, NULL);
	stack_init(paths, NULL);

	while (!stack_is_empty(parent) || node) {
		if (node) {
			stack_push(parent, (const void *) node);
			stack_push(paths, (const void *) node);
			node = node->left;
		} else {
			peek = (struct bitree_node *) stack_peek(parent);
			if (peek->right && last_visited != peek->right) {
				node = peek->right;
			} else {
				if (!peek->right && (peek->left != last_visited || !peek->left)) {
					printf("path %d:", ++cnt);
					print_path_sum(paths);
				}
				stack_pop(parent, (void **) &last_visited);
				stack_pop(paths, (void **) &dummy);
			}
		}
	}

	return cnt;
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

void mirror(struct bitree_node *root)
{
	struct bitree_node *temp;

	if (!root)
		return;

	if (root->left)
		mirror(root->left);

	if (root->right)
		mirror(root->right);

	temp = root->right;
	root->right = root->left;
	root->left = temp;

	return;
}

void double_tree_node(struct bitree *tree, struct bitree_node *node)
{
	int *data;
	struct bitree_node *dup;

	if (!node)
		return;

	data = (int *) malloc(sizeof(int));
	*data = *(int *) node->data;
	dup = bitree_alloc_node(data);

	dup->left = node->left;
	node->left = dup;
	tree->size++;
}

void double_tree(struct bitree *tree)
{
	struct bitree_node *node = tree->root;
	struct bitree_node *last_visited = NULL, *peek;
	struct stack_t parent_stack, *parent = &parent_stack;

	if (tree == NULL)
		return;

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
				double_tree_node(tree, peek);
				stack_pop(parent, (void **) &last_visited);
			}
		}
	}
}


#define DEFAULT_TREE_SIZE	5

int main(int argc, char *argv[])
{
	struct bitree *tree;
	int cnt, depth, min;

	tree = build(DEFAULT_TREE_SIZE);
	if (!tree) {
		fprintf(stderr, "build tree error\n");
		exit(EXIT_FAILURE);
	}

//	mirror(tree->root);

	cnt = preorder_traversal(tree->root, visit);
	printf("preorder: total %d nodes traversed\n", cnt);

	cnt = inorder_traversal(tree->root, visit);
	printf("inorder: total %d nodes traversed\n", cnt);

	cnt = postorder_traversal(tree->root, visit);
	printf("postorder: total %d nodes traversed\n", cnt);

	cnt = levelorder_traversal(tree->root, visit, &depth);
	printf("levelorder: total %d nodes traversed, max depth:%d\n", cnt, depth);

//	printf("\ndoing mirror to the tree\n");
//	mirror(tree->root);
//
//
//	cnt = preorder_traversal(tree->root, visit);
//	printf("preorder: total %d nodes traversed\n", cnt);
//
//	cnt = inorder_traversal(tree->root, visit);
//	printf("inorder: total %d nodes traversed\n", cnt);
//
//	cnt = postorder_traversal(tree->root, visit);
//	printf("postorder: total %d nodes traversed\n", cnt);
//
//	cnt = levelorder_traversal(tree->root, visit, &depth);
//	printf("levelorder: total %d nodes traversed, max depth:%d\n", cnt, depth);
//	
//	min = min_value(tree);
//	printf("min value: %d\n", min);
//
//	double_tree(tree);
//	cnt = levelorder_traversal(tree->root, visit, &depth);
//	printf("levelorder: total %d nodes traversed, max depth:%d\n", cnt, depth);
//
//	cnt = print_paths(tree->root);
//	printf("total %d paths\n", cnt);


exit:
	free(bitree_root(tree)->data);
	bitree_destroy(tree);
	free(tree);

	exit(EXIT_SUCCESS);
}
