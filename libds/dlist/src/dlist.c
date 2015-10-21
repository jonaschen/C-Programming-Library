#include "dlist.h"

void dlist_init(struct dlist *list, void (*free_data)(void *data))
{
	if (!list)
		return;

	list->size = 0;
	list->head = list->tail = NULL;
	list->free_data = free_data;
}

void dlist_destroy(struct dlist *list)
{
	void *data;

	if (!list)
		return;

	while (dlist_size(list) > 0) {
		dlist_remove(list, dlist_head(list), &data);
		if (list->free_data && data)
			list->free_data(data);
	}
}

struct dlist_node *dlist_alloc_node(void *data)
{
	struct dlist_node *new;

	new = (struct dlist_node *) malloc(sizeof(struct dlist_node));
	if (!new)
		return NULL;

	new->prev = new->next = NULL;
	new->data = data;

	return new;
}

int dlist_ins_next(struct dlist *list, struct dlist_node *node, const void *data)
{
	struct dlist_node *old = node, *new;

	if (!list)
		return -1;

	new = dlist_alloc_node((void *) data);
	if (!new)
		return -1;

	if (old) {
		new->next = old->next;
		new->prev = old;
	} else {
		new->next = list->head;
		new->prev = NULL;
		list->head = new;
	}

	if (!new->prev)
		list->head = new;
	else
		new->prev->next = new;

	if (!new->next)
		list->tail = new;
	else
		new->next->prev = new;

	list->size++;

	return 0;
}

int dlist_ins_prev(struct dlist *list, struct dlist_node *node, const void *data)
{
	struct dlist_node *old = node, *new;

	if (!list)
		return -1;

	new = dlist_alloc_node((void *) data);
	if (!new)
		return -1;

	if (old) {
		new->prev = old->prev;
		new->next = old;
	} else {
		new->prev = list->tail;
		new->next = NULL;
		list->tail = new;
	}

	if (!new->prev)
		list->head = new;
	else
		new->prev->next = new;

	if (!new->next)
		list->tail = new;
	else
		new->next->prev = new;

	list->size++;

	return 0;
}

int dlist_remove(struct dlist *list, struct dlist_node *node, void **data)
{
	struct dlist_node *old = node;

	if (!list || !node || dlist_size(list) == 0)
		return -1;

	if (node == NULL) {
		old = list->head;
	}

	if (old->prev)
		old->prev->next = old->next;
	else
		list->head = old->next;

	if (old->next)
		old->next->prev = old->prev;
	else
		list->tail = old->prev;

	if (data)
		*data = old->data;

	free(old);

	list->size--;

	return 0;
}
