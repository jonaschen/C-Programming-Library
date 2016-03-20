#include <stdlib.h>
#include <string.h>

#include "directive.h"


int directive_lc(const char *opcode, const char *operand, uint32_t *location_cntr)
{
	if (!strncmp(opcode, "START", 5))
		*location_cntr = (uint32_t) atoi(operand);

	if (!strncmp(opcode, "END", 3))
		return -1;

	return 0;
}


static const struct sic_directive directives[] = {
	{"START" },
	{"END"  },
	{"BYTE" },
	{"WORD" },
	{"RESB" },
	{"RESW" },
	{NULL   },
};

static int directive_match(const void *key1, const void *key2)
{
	const struct sic_directive *e1 = (const struct sic_directive *) key1;
	const struct sic_directive *e2 = (const struct sic_directive *) key2;

	if (strncmp(e1->directive, e2->directive, strlen(e2->directive)))
		return 0;

	return 1;
}

#define DIRECTIVE_SLOTS	32
static int directive_hash(const void *key)
{
	int val = 0;
	struct sic_directive *entry = (struct sic_directive *) key;
	char *ptr = (char *) entry->directive;

	while (*ptr != '\0')
		val += (int) (*ptr++);

	val = val * val;

	return (val % DIRECTIVE_SLOTS);
}

int directives_init(struct chtbl_t *table)
{
	int i;
	struct sic_directive *e;
	struct sic_directive *t, temp;

	chtbl_init(table, DIRECTIVE_SLOTS, directive_hash, directive_match, NULL);

	i = 0;
	while (directives[i].directive != NULL) {
		chtbl_insert(table, &directives[i]);
		i++;
	}

	return 0;
}
