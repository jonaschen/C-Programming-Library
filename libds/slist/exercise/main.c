#include <stdlib.h>
#include <stdio.h>

#include "slist.h"
#include "prob.h"

struct slist *build_123(int n)
{
	struct slist *list;
	int *data;
	int i;

	list = (struct slist *) malloc(sizeof(struct slist));
	if (!list)
		return NULL;

	slist_init(list, free);

	for (i = n; i > 0; i--) {
		data = (int *) malloc(sizeof(int));
		if (!data) {
			perror("data");
			exit(EXIT_FAILURE);
		}
		*data = i;
		slist_ins_next(list, NULL, data);
	}

	return list;
}

int main(int argc, char *argv[])
{
	struct slist *list;
	struct slist_node *node;
	int i, n = 3;

	if (argc >= 2)
		n = atoi(argv[1]);

	list = build_123(n);

	for (node = list->head, i = 0; node; node = node->next, i++)
		printf("node[%i], data:%d\n", i, *(int *) node->data);

	for (i = 1; i < 10; i++)
		printf("%d instances of data %d\n", prob_count(list, i), i);

	return 0;
}
