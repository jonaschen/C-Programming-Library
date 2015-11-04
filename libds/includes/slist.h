#ifndef __SLIST_H
#define __SLIST_H

#include "common.h"

struct slist_node {
	struct slist_node *next;

	void *data;
};

/* TODO: the interfaces for simple-version is not completed yet */
#ifdef SIMPLE_VERSION
struct slist {
	struct slist_node *head;
};
#else
struct slist {
	int size;

	struct slist_node *head;
	struct slist_node *tail;

	void (*free_data)(void *data);
};
#endif

void slist_init(struct slist *list, void (*free_data)(void *data));
void slist_destroy(struct slist *list);
struct slist_node *slist_alloc_node(void *data);
int slist_ins_next(struct slist *list, struct slist_node *node, const void *data);
int slist_ins_after(struct slist *list, struct slist_node *old, struct slist_node *ins);
int slist_rem_next(struct slist *list, struct slist_node *node, void **data);

struct slist_node *slist_get_nth(struct slist *list, int nth);
struct slist_node *slist_pop(struct slist *list);

static int slist_size(const struct slist *list)
{
	return (list) ? list->size : -1;
}

static struct slist_node *slist_head(const struct slist *list)
{
	return (list) ? list->head : NULL;
}

static struct slist_node *slist_tail(const struct slist *list)
{
	return (list) ? list->tail : NULL;
}

static int slist_is_head(struct slist *list, struct slist_node *node)
{
	return (list && node && node == slist_head(list)) ? 1 : 0;
}

static int slist_is_tail(struct slist *list, struct slist_node *node)
{
	return (list && node && node == slist_tail(list)) ? 1 : 0;
}

static void *slist_data(const struct slist_node *node)
{
	return (node) ? (node->data) : NULL;
}

static struct slist_node *slist_next(const struct slist_node *node)
{
	return (node) ? (node->next) : NULL;
}

#define slist_append_data(list, data)	\
	slist_ins_next(list, (list)->tail, (const void *)data)

#define slist_append_node(list, node)	\
	slist_ins_after(list, (list)->tail, node)

#define slist_append_list(front, back)		\
	do {					\
		if (slist_size(back) == 0)	\
			break;			\
		front->tail->next = back->head;	\
		front->tail = back->tail;	\
		front->size += back->size;	\
		back->tail = back->head = NULL;	\
		back->size = 0;			\
	} while (0);

#endif
