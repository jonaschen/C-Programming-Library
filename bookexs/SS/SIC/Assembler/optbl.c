#include <stdio.h>
#include <string.h>

#include "optbl.h"
#include "symtbl.h"

int optbl_lc(const char *opcode, const char *operand, uint32_t *location_cntr)
{
	if (!strncmp(opcode, "ADD", 3))
		*location_cntr += 1;
	else
		*location_cntr += 3;

	return 0;
}

static int asm_addr_parse(const char *operand, struct chtbl_t *symtbl, char *record)
{
	struct symtbl_entry *sym, e_sym;

	*record = '\0';

	sym = &e_sym;
	sym->symbol = (char *) operand;
	if (chtbl_lookup(symtbl, (void **) &sym) == 0)
		sprintf(record, "%04X", sym->location);
	else
		sprintf(record, "%04X", utils_atoh(operand));

	return strlen(record);
}

static int asm_rsub(const char *operand, struct chtbl_t *symtbl, char *record)
{
	sprintf(record, "0000");

	return 4;
}

static int asm_store(const char *operand, struct chtbl_t *symtbl, char *record)
{
	return asm_addr_parse(operand, symtbl, record);
}

static int asm_jump(const char *operand, struct chtbl_t *symtbl, char *record)
{
	return asm_addr_parse(operand, symtbl, record);
}

static int asm_load(const char *operand, struct chtbl_t *symtbl, char *record)
{
	return asm_addr_parse(operand, symtbl, record);
}

static int asm_comp(const char *operand, struct chtbl_t *symtbl, char *record)
{
	return asm_addr_parse(operand, symtbl, record);
}

static int asm_io(const char *operand, struct chtbl_t *symtbl, char *record)
{
	return asm_addr_parse(operand, symtbl, record);
}

static int asm_ch(const char *operand, struct chtbl_t *symtbl, char *record)
{
	char *label = (char *) operand, reg = 0;
	char *ptr = label;
	char buf[5];
	uint32_t value;

	while (*ptr != '\0' && *ptr != ',')
		ptr++;

	/* find reg*/
	if (*ptr == ',') {
		*ptr++ = '\0';
	}
	while (!isalpha(*ptr) && *ptr != '\0')
		ptr++;
	if (*ptr != '\0');
		reg = *ptr;
	asm_addr_parse(label, symtbl, buf);

	value = utils_atoh(buf);

	if (reg == 'X')
		value |= (1 << 15);

	sprintf(record, "%04X", value);

	return 4;
}

static const struct op_entry op_codes[] = {
	{"ADD",  0x18,	NULL},
	{"AND",  0x40,	NULL},
	{"COMP", 0x28,	asm_comp},
	{"DIV",  0x24,	NULL},
	{"J",	 0x3C,	asm_jump},
	{"JEQ",  0x30,	asm_jump},
	{"JGT",  0x34,	asm_jump},
	{"JLT",  0x38,	asm_jump},
	{"JSUB", 0x48,	asm_jump},
	{"LDA",  0x00,	asm_load},
	{"LDCH", 0x50,	asm_ch},
	{"LDL",  0x08,	asm_load},
	{"LDX",  0x04,	asm_load},
	{"UML",  0x20,	NULL},
	{"OR",   0x44,	NULL},
	{"RD",   0xD8,	asm_io},
	{"RSUB", 0x4C,	asm_rsub},
	{"STA",  0x0C,	asm_store},
	{"STCH", 0x54,	asm_ch},
	{"STL",  0x14,	asm_store},
	{"STSW", 0xE8,	asm_store},
	{"STX",  0x10,	asm_store},
	{"SUB",  0x1C,	NULL},
	{"TD",   0xE0,	asm_io},
	{"TIX",  0x2C,	asm_io},
	{"WD",   0xDC,	NULL},
	{NULL,   0xFF,	NULL},
};

static int opcode_match(const void *key1, const void *key2)
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

int optbl_init(struct chtbl_t *table)
{
	int i;
	struct op_entry *op;
	struct op_entry *t, temp;

	chtbl_init(table, OPTBL_SLOTS, optbl_sic_hash, opcode_match, NULL);

	i = 0;
	while (op_codes[i].memonic != NULL) {
		chtbl_insert(table, &op_codes[i]);
		i++;
	}

	/* TODO: lookup */

	i = 0;
	while (op_codes[i].memonic != NULL) {
		temp.memonic = op_codes[i].memonic;
		t = &temp;
		if (chtbl_lookup(table, (void **) &t)) {
			//printf("lookop %s fail\n", op_codes[i].memonic);
			return -1;
		} else {
			//printf("opcode:%s -> 0x%02x\n", t->memonic, t->opcode);
		}

		i++;
	}

	return 0;
}
