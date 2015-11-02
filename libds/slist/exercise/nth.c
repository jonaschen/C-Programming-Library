#include "slist.h"
#include "prob.h"


int prob_get_nth(struct slist *list, int nth, int *data)
{
	struct slist_node *node;

	if (!list || slist_size(list) < nth || !data)
		return -1;

	if ((node = slist_get_nth(list, nth)) == NULL)
		return -1;

	*data = *(int *) node->data;

	return 0;
}

int prob_ins_nth(struct slist *list, int nth, const void *data)
{
	struct slist_node *node;

	if (!list || slist_size(list) < nth || !data)
		return -1;

	if ((node = slist_get_nth(list, nth - 1)) == NULL)
		return -1;

	return slist_ins_next(list, node, data);
}
