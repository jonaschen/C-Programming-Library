#ifndef __QUEUE_H
#define __QUEUE_H

#include "common.h"

struct queue_elem {
	struct queue_elem *next;

	void *data;
};

struct queue_t {
	int size;

	struct queue_elem *first;
	struct queue_elem *last;

	void (*free_data)(void *data);
};

void queue_init(struct queue_t *q, void (*free_data)(void *data));
void queue_destroy(struct queue_t *q);
struct queue_elem *queue_alloc_elem(void *data);
int queue_enqueue(struct queue_t *q, const void *data);
int queue_dequeue(struct queue_t *q, void **data);

#define queue_peek(q)	((q)->head == NULL ? NULL : (q)->head->data)
#define queue_size(q)	((q) == NULL ? (q)->size : -1)

#endif
