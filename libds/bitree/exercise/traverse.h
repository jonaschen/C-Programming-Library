#ifndef __TRAVERSE_H
#define __TRAVERSE_H

#include "dlist.h"
#include "stack.h"
#include "queue.h"
#include "bitree.h"


int preorder_traversal(struct bitree_node *root, int (*visit)(struct bitree_node *));
int inorder_traversal(struct bitree_node *root, int (*visit)(struct bitree_node *));
int postorder_traversal(struct bitree_node *root, int (*visit)(struct bitree_node *));
int levelorder_traversal(struct bitree_node *root, int (*visit)(struct bitree_node *), int *depth);

#endif
