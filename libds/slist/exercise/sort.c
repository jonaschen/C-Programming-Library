#include <string.h>

#include "slist.h"
#include "prob.h"

static int int_list_compare(void *key1, void *key2)
{
	struct slist_node *node1, *node2;
	int data1, data2;

	node1 = (struct slist_node *) key1;
	node2 = (struct slist_node *) key2;

	data1 = *(int *) node1->data;
	data2 = *(int *) node2->data;

	return (data1 - data2);
}

void sorted_insert(struct slist *sorted, struct slist_node *new)
{
	struct slist_node *node, *prev = NULL;

	if (slist_size(sorted) < 1) {
		slist_ins_after(sorted, prev, new);
		return;
	}

	for (node = sorted->head; node; prev = node, node = node->next) {
		if (int_list_compare(new, node) < 0) {
			slist_ins_after(sorted, prev, new);
			return;
		}
	}

	slist_ins_after(sorted, sorted->tail, new);
}

void insert_sort(struct slist *list)
{
	struct slist_node *node;
	struct slist *sorted;

	if (!list || slist_size(list) < 2)
		return;

	sorted = (struct slist *) malloc(sizeof(struct slist));
	slist_init(sorted, list->free_data);
	list->free_data = NULL;

	while ((node = slist_pop(list)) != NULL) {
		sorted_insert(sorted, node);
	}

	memcpy(list, sorted, sizeof(struct slist));
	
	free(sorted);
}