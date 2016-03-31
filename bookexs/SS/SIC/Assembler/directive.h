#ifndef __DIRECTIVE_H
#define __DIRECTIVE_H

#include <stdio.h>
#include <stdint.h>
#include "chtbl.h"

enum DIRECTIVE_TYPE {
	DIRECT_START = 1,
	DIRECT_END,
	DIRECT_RSUB,
	DIRECT_BYTE,
};

struct sic_directive {
	const char *directive;
	int (*location_update)(const char *opcode, const char *operand, uint32_t *location_cntr);
	int (*assemble)(const char *addr, const char *operand, FILE *bin);
};


#endif
