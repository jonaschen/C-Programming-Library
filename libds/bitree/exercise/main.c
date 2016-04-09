#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"
#include "stack.h"
#include "bitree.h"
#include "exercise.h"

#define TRUE	1
#define FALSE	0

static struct dlist *visit_list = NULL;

static int dump_visit_list(int destroy)
{
	struct dlist *list = visit_list;
	struct dlist_node *node;
	int i = 0;

	if (!list)
		return;

	for (node = dlist_head(list); node != NULL; node = node->next) {
		fprintf(stdout, "%4d ", *(int *) node->data);
		if (!(++i % 16))
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");

	if (destroy) {
		dlist_destroy(list);
		dlist_init(visit_list, NULL);
	}

	return i;
}

int visit(struct bitree_node *node)
{
	struct dlist *list;

	if (!visit_list) {
		visit_list = (struct dlist *) malloc(sizeof(struct dlist));
		dlist_init(visit_list, NULL);
	}
	list = visit_list;

	if (!node || !node->data)
		return -1;

	dlist_ins_next(list, dlist_tail(list), (const void *) node->data);

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
			fprintf(stdout, "%d, ", *pdata);
	}
	fprintf(stdout, "\n sum:%d\n", sum);
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
					fprintf(stdout, "path %d:", ++cnt);
					print_path_sum(paths);
				}
				stack_pop(parent, (void **) &last_visited);
				stack_pop(paths, (void **) &dummy);
			}
		}
	}

	return cnt;
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

typedef enum trave_direction {
	TRAVE_PREORDER,
	TRAVE_INORDER,
	TRAVE_POSTORDER,
	TRAVE_LEVELORDER,
	TRAVE_MAX,
} TRAVE_DIRECTION;

static const char* TRAVERSE_STRINGS[TRAVE_MAX] = {
	"preorder",
	"inorder",
	"postorder",
	"levelorder",
};

static int traverse_dump(struct bitree *tree, TRAVE_DIRECTION dir)
{
	int cnt = -1, depth;
	const char *str = NULL;

	if (!tree)
		goto exit;

	if (dir >= TRAVE_PREORDER && dir < TRAVE_MAX)
		str = TRAVERSE_STRINGS[dir];

	switch (dir) {
	case TRAVE_PREORDER:
		cnt = preorder_traversal(tree->root, visit);
		break;
	case TRAVE_INORDER:
		cnt = inorder_traversal(tree->root, visit);
		break;
	case TRAVE_POSTORDER:
		cnt = postorder_traversal(tree->root, visit);
		break;
	case TRAVE_LEVELORDER:
		cnt = levelorder_traversal(tree->root, visit, &depth);
		fprintf(stdout, "%s: depth:%d\n", str, depth);
		break;
	default:
		break;
	}

	fprintf(stdout, "%s: total %d nodes traversed\n",
			 (str == NULL) ? "N/A" : str , cnt);
	dump_visit_list(TRUE);

exit:
	return cnt;
}

#define DEFAULT_TREE_SIZE	5

int main(int argc, char *argv[])
{
	struct bitree *tree;
	int cnt;

	tree = build(DEFAULT_TREE_SIZE);
	if (!tree) {
		fprintf(stderr, "build tree error\n");
		exit(EXIT_FAILURE);
	}

	traverse_dump(tree, TRAVE_PREORDER);

	traverse_dump(tree, TRAVE_INORDER);

	traverse_dump(tree, TRAVE_POSTORDER);

	traverse_dump(tree, TRAVE_LEVELORDER);

	fprintf(stdout, "\nmirroring the tree\n");
	mirror(tree->root);
	traverse_dump(tree, TRAVE_LEVELORDER);
	mirror(tree->root); /* reverse */

	fprintf(stdout, "\ndoubling the tree\n");
	double_tree(tree);
	traverse_dump(tree, TRAVE_LEVELORDER);
	traverse_dump(tree, TRAVE_INORDER);
	traverse_dump(tree, TRAVE_PREORDER);

	cnt = print_paths(tree->root);
	fprintf(stdout, "total %d paths\n", cnt);


exit:
	if (visit_list)
		dlist_destroy(visit_list);
	free(bitree_root(tree)->data);
	bitree_destroy(tree);
	free(tree);

	exit(EXIT_SUCCESS);
}
