#ifndef __DIRECTIVE_H
#define __DIRECTIVE_H

#include <stdint.h>
#include "chtbl.h"

enum DIRECTIVE_TYPE {
	DIRECT_START = 1,
	DIRECT_END,
};

struct sic_directive {
	const char *directive;
	int (*location_update)(const char *opcode, const char *operand, uint32_t *location_cntr);
};


#endif
