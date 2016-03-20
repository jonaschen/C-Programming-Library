#ifndef __DIRECTIVE_H
#define __DIRECTIVE_H

#include <stdint.h>
#include "chtbl.h"

struct sic_directive {
	const char *directive;
};

int directive_lc(const char *opcode, const char *operand, uint32_t *location_cntr);

#endif
