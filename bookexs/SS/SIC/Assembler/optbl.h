#ifndef __OPTBL_H
#define __OPTBL_H

#include <stdint.h>
#include "chtbl.h"

struct op_entry {
	const char 	*memonic;
	const uint8_t	opcode;
	int (*assemble)(const char *operand, struct chtbl_t *symtbl, char *record);
};


int optbl_init(struct chtbl_t *table);
int optbl_lc(const char *opcode, const char *operand, uint32_t *location_cntr);

#endif
