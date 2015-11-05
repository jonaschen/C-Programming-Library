#include "slist.h"


void move_node(struct slist *dest, struct slist *src)
{
	struct slist_node *node;

	if (!dest || !src || slist_size(src) <= 0)
		return;

	node = slist_pop(src);
	slist_ins_after(dest, NULL, node);
}

void reverse_list(struct slist *list)
{
	struct slist dummy, *temp = &dummy;
	struct slist_node *node;

	if (!list)
		return;

	slist_init(temp, NULL);

	while (slist_size(list) > 0) {
		node = slist_pop(list);
		slist_push_node(temp, node);
	}

	list->head = temp->head;
	list->size = temp->size;
	list->tail = temp->tail;

	return;
}
