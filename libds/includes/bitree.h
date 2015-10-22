#ifndef __BITREE_H
#define __BITREE_H

#include <stdlib.h>

struct bitree_node {
	void *data;

	struct bitree_node *left;
	struct bitree_node *right;
};

#endif
