#include "slist.h"

void slist_init(struct slist *list, void (*free_data)(void *data))
{
	if (!list)
		return;

	list->head = NULL;

#ifndef SIMPLE_VERSION
	list->tail = NULL;
	list->size = 0;
	list->free_data = free_data;
#endif
}

void slist_destroy(struct slist *list)
{
	void *data = NULL;

	if (!list)
		return;

	while (slist_head(list)) {
		slist_rem_next(list, NULL, &data);
		if (list->free_data && data)
			list->free_data(data);
	}

	memset(list, 0, sizeof(struct slist));
}

struct slist_node *slist_alloc_node(void *data)
{
	struct slist_node *node;

	node = (struct slist_node *) malloc(sizeof(struct slist_node));
	if (!node)
		return NULL;

	node->next = NULL;
	node->data = data;

	return node;
}

int slist_ins_after(struct slist *list, struct slist_node *old, struct slist_node *new)
{
	if (!list || !new)
		return -1;

	if (old == NULL) {
		new->next = list->head;
		list->head = new;
	} else {
		new->next = old->next;
		old->next = new;
	}

	list->size++;

	if (new->next == NULL)
		list->tail = new;

	return 0;
}

int slist_ins_next(struct slist *list, struct slist_node *node, const void *data)
{
	struct slist_node *new;

	if (!list)
		return -1;

	new = slist_alloc_node((void *) data);
	if (!new)
		return -1;

	return slist_ins_after(list, node, new);
}

int slist_rem_next(struct slist *list, struct slist_node *node, void **data)
{
	struct slist_node *old;

	if (data)
		*data = NULL;

	if (!list || slist_size(list) == 0)
		return -1;

	if (node == NULL) {
		old = slist_head(list);
		list->head = slist_next(old);
	} else {
		old = slist_next(node);
		if (!old)
			return -1;
		node->next = old->next;
	}

	*data = slist_data(old);

	if (slist_is_tail(list, old))
		list->tail = node;

	list->size--;

	free(old);

	return 0;
}

