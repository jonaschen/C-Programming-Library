#include "queue.h"

void queue_init(struct queue_t *q, void (*free_data)(void *data))
{
	if (!q)
		return;

	q->first = q->last = NULL;
	q->size = 0;
	q->free_data = free_data;
}

void queue_destroy(struct queue_t *q)
{
	void *data;

	if (!q)
		return;

	while (queue_size(q)) {
		queue_dequeue(q, &data);
		if (q->free_data && data)
			q->free_data(data);
	}
}

struct queue_elem *queue_alloc_elem(void *data)
{
	struct queue_elem *new;

	new = (struct queue_elem *) malloc(sizeof(struct queue_elem));
	if (!new)
		return NULL;

	new->next = NULL;
	new->data = data;

	return new;
}

int queue_enqueue(struct queue_t *q, const void *data)
{
	struct queue_elem *new;

	if (!q)
		return -1;

	new = queue_alloc_elem((void *) data);
	if (!new)
		return -1;

	new->next = q->first;
	if (!new->next)
		q->last = new;

	q->first = new;

	q->size++;

	return 0;
}

int queue_dequeue(struct queue_t *q, void **data)
{
	struct queue_elem *old;

	if (data)
		*data = NULL;

	if (!q || queue_size(q) == 0)
		return -1;

	if (queue_size(q) == 1) {
		old = q->first;
		q->first = q->last = NULL;
	} else {
		for (old = q->first; old->next != q->last; old = old->next)
			;
		q->last = old;
		old = old->next;
	}

	if (data)
		*data = old->data;

	q->size--;
	free(old);

	return 0;
}
