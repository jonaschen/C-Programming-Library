#include <stdlib.h>
#include <string.h>

#include "symtbl.h"

static int symtbl_match(const void *key1, const void *key2)
{
	const struct symtbl_entry *e1 = (const struct symtbl_entry *) key1;
	const struct symtbl_entry *e2 = (const struct symtbl_entry *) key2;

	if (strncmp(e1->symbol, e2->symbol, strlen(e2->symbol)))
		return 0;

	return 1;
}

/* TODO: choose a proper hash function (Prime number table) */
#define SYMBOL_TABLE_SLOTS	256
static int symtbl_hash(const void *key)
{
	int val = 0;
	struct symtbl_entry *entry = (struct symtbl_entry *) key;
	char *ptr = (char *) entry->symbol;

	while (*ptr != '\0')
		val += (int) (*ptr++);

	return (val % SYMBOL_TABLE_SLOTS);
}

static void symtbl_free_data(void *data)
{
	struct symtbl_entry *entry = (struct symtbl_entry *) data;

	free((void*) entry->symbol);
}

int symbol_table_init(struct chtbl_t *table)
{
	int i;
	struct symtbl_entry *e;
	struct symtbl_entry *t, temp;

	chtbl_init(table, SYMBOL_TABLE_SLOTS, symtbl_hash, symtbl_match, symtbl_free_data);

	return 0;
}
