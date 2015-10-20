#include "slist.h"
#include "prob.h"

int prob_count(struct slist *list, int data)
{
	int cnt = 0;
	struct slist_node *node;

	if (!list)
		return -1;

	for (node = list->head; node; node = node->next)
		if (node->data && *((int *) node->data) == data)
			cnt++;

	return cnt;
}
