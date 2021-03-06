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


struct slist *build_arithematic_seq(int start, int length, int step)
{
	struct slist *list;
	int *data;
	int i;
	int max = start + length * step - 1;

	list = (struct slist *) malloc(sizeof(struct slist));
	if (!list)
		return NULL;

	slist_init(list, free);

	for (i = max; i >= start; i -= step) {
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

static inline struct slist *build_sequence(int start, int number)
{
	return build_arithematic_seq(start, number, 1);
}

/*
 * do_nth_test
 *	1. get_nth
 *	2. ins_nth
 */
static void do_nth_test(struct slist *list)
{
	int i, data, *pdata;

	for (i = 0; i < slist_size(list); i++) {
		if (prob_get_nth(list, i, &data)) {
			perror("get_nth");
			exit(EXIT_FAILURE);
		}
		printf("%dth nodes of list: %d\n", i, data);
	}

	printf("insert nodes into the list\n");
	for (i = 3; i < slist_size(list); i += 3) {
		pdata = (int *) malloc(sizeof(int));
		*pdata = i * i * 10;
		if (prob_ins_nth(list, i , pdata)) {
			perror("ins_nth");
			exit(EXIT_FAILURE);
		}
	}

	slist_dump_int_data(list);
}

struct slist *do_sort_split_merge_test(struct slist *list)
{
	struct slist *temp, *a, *b;

	printf("[%s] start test\n", __func__);

	a = (struct slist *) malloc(sizeof(struct slist));
	b = (struct slist *) malloc(sizeof(struct slist));

	slist_init(a, list->free_data);
	slist_init(b, list->free_data);

	do_nth_test(list);
	slist_dump_int_data(list);

	printf("[%s] do insert sort\n", __func__);
	insert_sort(list);
	slist_dump_int_data(list);

	printf("[%s] remove_duplicates\n", __func__);
	remove_duplicates(list);
	slist_dump_int_data(list);

	front_back_split(list, a);
	printf("\n[%s] front_back_split:\n", __func__);
	printf("front:\n");
	slist_dump_int_data(list);
	printf("back:\n");
	slist_dump_int_data(a);

	temp = shuffle_merge(list, a);

	list = temp;
	printf("\nafter shuffle_merge:\n");
	slist_dump_int_data(list);

	printf("\nafter alternating_split:\n");
	alternating_split(list, a, b);
	slist_dump_int_data(a);
	slist_dump_int_data(b);

	if (slist_size(list) > 0) {
		fprintf(stderr, "split error\n");
	}

	printf("\nafter shuffle_merge again:\n");
	temp = shuffle_merge(a, b);
	slist_dump_int_data(temp);
	free(a);
	free(b);

	return temp;
}

#define MOVE_NUMBER	3
void do_move_test(struct slist *list)
{
	struct slist *temp;
	struct slist_node *node;
	int i;

	printf("[%s] start test\n", __func__);
	slist_dump_int_data(list);

	temp = build_sequence(1000, MOVE_NUMBER);
	slist_dump_int_data(temp);
	printf("list:%p, temp:%p\n", (void *)list, (void *)temp);

	for (i = 0; i < MOVE_NUMBER; i++) {
		node = slist_pop(temp);
		slist_ins_next(list, NULL, node->data);
	}

	slist_dump_int_data(list);
	if (slist_size(temp) > 0) {
		fprintf(stderr, "move error\n");
	}
	free(temp);

	printf("[%s] reverse the list:\n", __func__);
	reverse_list(list);
	slist_dump_int_data(list);
}

void do_sort_merge_test(void)
{
	struct slist *a, *b, *c;

	printf("[%s] start test\n", __func__);
	a = build_sequence(6, 9);
	b = build_sequence(23, 18);
	c = build_sequence(12, 9);
	a = shuffle_merge(a, c);
	a = shuffle_merge(a, b);
	slist_dump_int_data(a);
	printf("[%s] after merge_sort:\n", __func__);
	merge_sort(a);
	slist_dump_int_data(a);
}

void do_count_test(struct slist *list)
{
	int i, j, *data;
	int n = slist_size(list);

	/* append several repeat values into the list */
	for (i = 0; i < n; i++) {
		for (j = 0; j < i; j++) {
			data = (int *) malloc(sizeof(int));
			*data = i;
			slist_append_data(list, data);
		}
	}
	printf("[%s] The processed list:\n", __func__);
	slist_dump_int_data(list);

	for (i = 0; i <= n; i++)
		printf("[%s] %d instances of data %d\n", __func__, prob_count(list, i), i);
}

int main(int argc, char *argv[])
{
	struct slist *list, *temp;
	struct slist_node *node;
	int i, n = 3;

	if (argc >= 2)
		n = atoi(argv[1]);

	list = build_sequence(1, n);
	printf("The initial list:\n");
	slist_dump_int_data(list);

	do_count_test(list);

	temp = do_sort_split_merge_test(list);
	free(list);

	list = temp;
	do_move_test(list);

	do_sort_merge_test();

	exit(EXIT_SUCCESS);
}
