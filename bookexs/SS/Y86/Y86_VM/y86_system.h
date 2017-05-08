#ifndef __Y86_SYSTEM_H
#define __Y86_SYSTEM_H

#include "y86_cpu_core.h"

struct system_run_arg {
	unsigned char *program;
	size_t size;
};

#endif
