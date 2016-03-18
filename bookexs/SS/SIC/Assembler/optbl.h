#ifndef __OPTBL_H
#define __OPTBL_H

#include <stdint.h>
#include "chtbl.h"

struct op_entry {
	const char 	*memonic;
	const uint8_t	opcode;
};


int optbl_init(struct chtbl_t *table);

#endif
