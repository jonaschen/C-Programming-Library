#ifndef __DLIST_H
#define __DLIST_H

#include <stdlib.h>

struct dlist_node {
	struct dlist_node *prev;
	struct dlist_node *next;

	void *data;
};

struct dlist {
	int size;

	void (*free_data)(void *data);

	struct dlist_node *head;
	struct dlist_node *tail;
};


void dlist_init(struct dlist *list, void (*free_data)(void *data));
void dlist_destroy(struct dlist *list);
struct dlist_node *dlist_alloc_node(void *data);

int dlist_ins_next(struct dlist *list, struct dlist_node *node, const void *data);
int dlist_ins_prev(struct dlist *list, struct dlist_node *node, const void *data);
int dlist_remove(struct dlist *list, struct dlist_node *node, void **data);


#define dlist_size(list)	((list)->size)
#define dlist_head(list)	((list)->head)
#define dlist_tail(list)	((list)->tail)

#define dlist_data(node)	((node)->data)
#define dlist_prev(node)	((node)->prev)
#define dlist_next(node)	((node)->next)

#endif
