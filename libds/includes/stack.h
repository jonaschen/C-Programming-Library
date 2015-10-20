#ifndef __STACK_H
#define __STACK_H

#include "list.h"

typedef struct slist stact_t;

#define stack_init	slist_init
#define stack_destroy 	slist_destroy

int stack_push(stack_t *stack, const void *data);
int stack_pop(stack_t *stack, void **data);

#endif
