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

/*
 * even there is already node with the same data, insert anyway
 */
int chtbl_insert(struct chtbl_t *htbl, const void *data)
{
	void *temp;
	int bucket, retval;

	temp = (void *) data;

	bucket = htbl->h(data) % htbl->buckets;

	if ((retval = slist_ins_next(&htbl->table[bucket], NULL, data)) == 0)
		htbl->size++;

	return retval;
}

int chtbl_remove(struct chtbl_t *htbl, void **data)
{
	struct list_node *elem, *prev;
	int bucket;

	bucket = htbl->hash(*data) % htbl->buckets;
	prev = NULL;

	for (elem = list_head(&htbl->table[bucket]); elem; elem = slist_next(elem)) {
		if (htbl->match(*data, list_data(elem))) {
			if (slist_rem_next(&htbl->table[bucket], prev, data) == 0) {
				htbl->size--;
				return 0;
			} else {
				return -1;
			}
		}
		prev = elem;
	}
	return -1;
}

int chtbl_lookup(const struct chtbl_t *htbl, void **data)
{
	struct slist_node *elem;
	int bucket;

	bucket = htbl->hash(*data) % htbl->buckets;

	for (elem = list_head(&htbl->table[bucket]); elem; elem = slist_next(elem)) {
		if (htbl->match(*data, slist_data(elem))) {
			*data = list_data(elem);
			return 0;
		}
	}

	return -1;
}
