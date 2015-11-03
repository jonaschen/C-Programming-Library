#include "slist.h"


void alternating_split(struct slist *source, struct slist *a, struct slist *b)
{
	struct slist_node *node;
	int alt = 0;

	while (slist_size(source) > 0) {	
		node = slist_pop(source);

		if (!alt) {
			slist_append_node(a, node);
			alt = 1;
		} else {
			slist_append_node(b, node);
			alt = 0;
		}
	}
}

struct slist *shuffle_merge(struct slist *a, struct slist *b)
{
	struct slist_node *node;
	struct slist *list;

	list = (struct slist *) malloc(sizeof(struct slist));
	slist_init(list, a->free_data);

	while (slist_size(a) > 0 && slist_size(b) > 0) {
		node = slist_pop(a);
		slist_append_node(list, node);
		node = slist_pop(b);
		slist_append_node(list, node);
	}

	if (slist_size(a) > 0) {
		slist_append_list(list, a);
	}

	if (slist_size(b) > 0) {
		slist_append_list(list, b);
	}

	return list;
}
