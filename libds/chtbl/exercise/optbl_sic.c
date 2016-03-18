#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "chtbl.h"

struct op_entry {
	const char 	*memonic;
	const uint8_t	opcode;
};

static const struct op_entry op_codes[] = {
	{"ADD",  0x18},
	{"AND",  0x40},
	{"COMP", 0x28},
	{"DIV",  0x24},
	{"J",	 0x3C},
	{"JEQ",  0x30},
	{"JGT",  0x34},
	{"JLT",  0x38},
	{"JSUB", 0x48},
	{"LDA",  0x00},
	{"LDCH", 0x50},
	{"LDL",  0x08},
	{"LDX",  0x04},
	{"UML",  0x20},
	{"OR",   0x44},
	{"RD",   0xD8},
	{"RSUB", 0x4C},
	{"STA",  0x0C},
	{"STL",  0x14},
	{"STSW", 0xE8},
	{"STX",  0x10},
	{"SUB",  0x1C},
	{"TIX",  0x2C},
	{"WD",   0xDC},
	{NULL,   0xFF},
};

static struct chtbl_t op_table;

int opcode_match(const void *key1, const void *key2)
{
	const struct op_entry *op1 = (const struct op_entry *) key1;
	const struct op_entry *op2 = (const struct op_entry *) key2;

	if (strncmp(op1->memonic, op2->memonic, strlen(op2->memonic)))
		return 0;

	return 1;
}

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
	struct op_entry *op;
	struct op_entry *t, temp;

	chtbl_init(&op_table, OPTBL_SLOTS, optbl_sic_hash, opcode_match, NULL);

	i = 0;
	while (op_codes[i].memonic != NULL) {
		chtbl_insert(&op_table, &op_codes[i]);
		i++;
	}

	/* TODO: lookup */

	i = 0;
	while (op_codes[i].memonic != NULL) {
		temp.memonic = op_codes[i].memonic;
		t = &temp;
		if (chtbl_lookup(&op_table, (void **) &t)) {
			printf("lookop %s fail\n", op_codes[i].memonic);
		} else {
			printf("opcode:%s -> 0x%02x\n", t->memonic, t->opcode);
		}

		i++;
	}

	return 0;
}
