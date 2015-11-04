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

void remove_duplicates(struct slist *list)
{
	struct slist_node *node, *prev;
	void *data;

	if (!list || slist_size(list) < 2)
		return;

	prev = list->head;
	node = prev->next;
	while (node) {
		if (int_list_compare(prev, node) == 0) {
			slist_rem_next(list, prev, &data);
			if (list->free_data)
				list->free_data(data);
			node = prev->next;
		} else {
			prev = node;
			node = node->next;
		}
	}
}

int sorted_merge(struct slist *a, struct slist *b)
{
	struct slist_node *prev_a = NULL, *node_a, *node_b;
	struct slist *sorted;

	if (!a || !b || slist_size(b) <= 0)
		return -1;

	node_a = a->head;
	while (slist_size(b) > 0) {
		node_b = slist_pop(b);
		while (node_a && int_list_compare(node_a, node_b) < 0) {
			prev_a = node_a;
			node_a = node_a->next;
		}

		if (!node_a) {
			slist_append_list(a, b);
			break;
		} else {
			slist_ins_after(a, prev_a, node_b);
		}
	}

	return 0;
}
