#include "stack.h"

void stack_init(struct stack_t *stack, void (*free_data)(void *data))
{
	if (!stack)
		return;

	stack->depth = 0;
	stack->top = stack->bottom = NULL;
	stack->free_data = free_data;
}

void stack_destroy(struct stack_t *stack)
{
	void *data = NULL;

	while (!stack_is_empty(stack)) {
		stack_pop(stack, &data);
		if (stack->free_data && data)
			stack->free_data(data);
	}

	memset(stack, 0, sizeof(struct stack_t));
}

struct stack_elem *stack_alloc_elem(void *data)
{
	struct stack_elem *elem;

	elem = (struct stack_elem *) malloc(sizeof(struct stack_elem));
	if (!elem)
		return NULL;

	elem->next = NULL;
	elem->data = data;

	return elem;
}

int stack_push(struct stack_t *stack, const void *data)
{
	struct stack_elem *new;

	if (!stack)
		return -1;

	new = stack_alloc_elem((void *) data);
	if (!new)
		return -1;

	new->next = stack->top;
	stack->top = new;

	stack->depth++;

	return 0;
}

int stack_pop(struct stack_t *stack, void **data)
{
	struct stack_elem *top;

	if (data)
		*data = NULL;

	if (!stack || stack_is_empty(stack))
		return -1;

	top = stack->top;
	stack->top = top->next;

	*data = top->data;
	free(top);

	stack->depth--;

	return 0;
}

