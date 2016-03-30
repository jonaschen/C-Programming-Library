#include <stdlib.h>
#include <string.h>

#include "directive.h"


/*
 * directive location counter update functions
 */
static int dirlc_start(const char *opcode, const char *operand, uint32_t *location_cntr)
{
	const char *ptr = operand;
	uint32_t cntr = 0U;

	while (isxdigit(*ptr)) {
		cntr *= 16;
		if (isdigit(*ptr)) {
			cntr += (*ptr - '0');
		} else {
			cntr += (toupper(*ptr) - 'A' + 10);
		}

		ptr++;
	}

	*location_cntr = cntr;

	return DIRECT_START;
}

static int dirlc_end(const char *opcode, const char *operand, uint32_t *location_cntr)
{
	return DIRECT_END;
}

static int dirlc_byte(const char *opcode, const char *operand, uint32_t *location_cntr)
{
	if (*operand == 'C')
		*location_cntr += (strlen(operand) - 3);
	else if (*operand == 'X')
		*location_cntr += 1;

	return 0;
}

static int dirlc_word(const char *opcode, const char *operand, uint32_t *location_cntr)
{
	*location_cntr += 3;

	return 0;
}

static int dirlc_resw(const char *opcode, const char *operand, uint32_t *location_cntr)
{
	*location_cntr += 3 * atoi(operand);

	return 0;
}

static int dirlc_resb(const char *opcode, const char *operand, uint32_t *location_cntr)
{
	*location_cntr += 1 * atoi(operand);

	return 0;
}

static const struct sic_directive directives[] = {
	{"START", dirlc_start },
	{"END"  , dirlc_end   },
	{"BYTE" , dirlc_byte  },
	{"WORD" , dirlc_word  },
	{"RESB" , dirlc_resb  },
	{"RESW" , dirlc_resw  },
	{NULL   , },
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
