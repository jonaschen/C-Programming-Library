#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "slist.h"
#include "prob.h"

#define slist_dump_int_data(list)	\
	do {									\
		struct slist_node *node;					\
		int i;								\
		for (node = (list)->head, i = 0; node; node = node->next, i++)	\
			printf("node[%i], data:%d\n", i, *(int *) node->data);	\
	} while (0);

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

static void do_nth_test(struct slist *list)
{
	int i, data, *pdata;

	for (i = 4; i < slist_size(list); i += 3) {
		if (prob_get_nth(list, i, &data)) {
			perror("get_nth");
			exit(EXIT_FAILURE);
		}
		printf("%dth nodes of list: %d\n", i, data);
	}

	for (i = 4; i < slist_size(list); i += 3) {
		pdata = (int *) malloc(sizeof(int));
		*pdata = i * i;
		if (prob_ins_nth(list, i , pdata)) {
			perror("ins_nth");
			exit(EXIT_FAILURE);
		}
	}

	for (i = 4; i < slist_size(list); i += 3) {
		if (prob_get_nth(list, i, &data)) {
			perror("get_nth");
			exit(EXIT_FAILURE);
		}
		printf("%dth nodes of list: %d\n", i, data);
	}

	for (i = 4; i < slist_size(list); i += 3) {
		pdata = (int *) malloc(sizeof(int));
		*pdata = i * i;
		if (prob_ins_nth(list, i , pdata)) {
			perror("ins_nth");
			exit(EXIT_FAILURE);
		}
	}
}

void do_sort_split_merge_test(struct slist *list)
{
	struct slist *temp, *a, *b;

	a = (struct slist *) malloc(sizeof(struct slist));
	b = (struct slist *) malloc(sizeof(struct slist));

	slist_init(a, list->free_data);
	slist_init(b, list->free_data);

	do_nth_test(list);
	slist_dump_int_data(list);

	insert_sort(list);
	slist_dump_int_data(list);

	remove_duplicates(list);
	slist_dump_int_data(list);

	front_back_split(list, a);
	printf("\nafter front_back_split:\n");
	slist_dump_int_data(list);
	slist_dump_int_data(a);

	temp = shuffle_merge(list, a);
	free(list);

	list = temp;
	printf("\nafter shuffle_merge:\n");
	slist_dump_int_data(list);

	printf("\nafter alternating_split:\n");
	alternating_split(list, a, b);
	slist_dump_int_data(a);
	slist_dump_int_data(b);

	free(a);
	free(b);
}

int main(int argc, char *argv[])
{
	struct slist *list;
	struct slist_node *node;
	int i, n = 3;

	if (argc >= 2)
		n = atoi(argv[1]);

	list = build_123(n);
	slist_dump_int_data(list);

	for (i = 1; i < n; i++)
		printf("%d instances of data %d\n", prob_count(list, i), i);

	do_sort_split_merge_test(list);

	exit(EXIT_SUCCESS);
}
