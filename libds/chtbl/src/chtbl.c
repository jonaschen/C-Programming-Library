#include "slist.h"
#include "chtbl.h"

int chtbl_init(struct chtbl_t *htbl, int buckets, int (*h)(const void *key),
		int (*match)(const void *key1, const void *key2),
		void (*free_data)(void *data))
{
	int i;

	if (!htbl)
		return -1;

	htbl->table = (struct slist *) malloc(buckets * sizeof(struct slist));
	if (htbl->table == NULL)
		return -1;

	htbl->buckets = buckets;

	for (i = 0; i < htbl->buckets; i++)
		list_init(&htbl->table[i], free_data);

	htbl->hash = h;
	htbl->match = match;
	htbl->free_data = free_data;
	htbl->size = 0;

	return 0;
}

void chtbl_destroy(struct chtbl_t *htbl)
{
	if (!htbl)
		return;

	for (i = 0; i < htbl->buckets; i++) {
		list_destroy(&htbl->table[i]);
	}

	free(htbl->table);

	memset(htbl, 0, sizeof(struct chtbl_t));

	return;
}

int chtbl_insert(struct chtbl_t *htbl, const void *data)
{
	void *temp;
	int bucket, retval;

	temp = (void *) data;
}

int chtbl_remove(struct chtbl_t *htbl, void **data);
int chtbl_lookup(const struct chtbl_t *htbl, void **data);
