#include <stdint.h>

#include "chtbl.h"

struct op_entry {
	const char 	*memonic;
	const uint8_t	opcode;
};

static const struct op_entry op_codes[] = {
	{"ADD",	0x18},
	{"AND", 0x40},
	{"COMP", 0x28},
	{"DIV", 0x24},
	{"J",	0x3C}, 
	{NULL, 0x00},
};

static struct chtbl_t op_table;

#define OPTBL_SLOTS	256
static int optbl_sic_hash(const void *key)
{
	int val = 0;
	struct op_entry *entry = (struct op_entry *) key;
	char *ptr = (char *) entry->memonic;

	while (*ptr != '\0')
		val += (int) (*ptr++);

	val = val * val;

	return (val % OPTBL_SLOTS);
}

int main(int argc, char *argv[])
{
	int i;

	chtbl_init(&op_table, OPTBL_SLOTS, optbl_sic_hash, NULL, NULL);

	i = 0;
	while (op_codes[i].memonic != NULL) {
		chtbl_insert(&op_table, &op_codes[i]);
	}

	/* TODO: lookup */

	return 0;
}
