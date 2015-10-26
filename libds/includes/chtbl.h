#ifndef __CHTBL_H
#define __CHTBL_H

#include "common.h"
#include "slist.h"

struct chtbl_t {
	int 	buckets;

	int	(*match)(const void *key1, const void *key2);
	int 	(*hash)(const void *key);
	void 	(*free_data)(void *data);

	int	size;
	struct slist *table;
};

int chtbl_init(struct chtbl_t *htbl, int buckets, int (*h)(const void *key),
		int (*match)(const void *key1, const void *key2), void (*free_data)(void *data));
void chtbl_destroy(struct chtbl_t *htbl);

int chtbl_insert(struct chtbl_t *htbl, const void *data);
int chtbl_remove(struct chtbl_t *htbl, void **data);
int chtbl_lookup(const struct chtbl_t *htbl, void **data);

#define chtbl_size(htbl)	((htbl)->size)

#endif
