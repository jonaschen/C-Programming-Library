#include "slist.h"


void move_node(struct slist *dest, struct slist *src)
{
	struct slist_node *node;

	if (!dest || !src || slist_size(src) <= 0)
		return;

	node = slist_pop(src);
	slist_ins_after(dest, NULL, node);
}
