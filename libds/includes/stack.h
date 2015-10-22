#ifndef __STACK_H
#define __STACK_H

#include <stdlib.h>

struct stack_elem {
	struct stack_elem *next;

	void *data;
};

struct stack_t {
	int depth;

	struct stack_elem *top;
	struct stack_elem *bottom;

	void (*free_data)(void *data);
};


void stack_init(struct stack_t *stack, void (*free_data)(void *data));
void stack_destroy(struct stack_t *stack);
struct stack_elem *stack_alloc_elem(void *data);

int stack_push(struct stack_t *stack, const void *data);
int stack_pop(struct stack_t *stack, void **data);

#define stack_peek(stack)	((stack)->top == NULL ? NULL : (stack)->top->data)
#define stack_size(stack)	((stack) == NULL ? (stack)->depth : -1)

#endif
