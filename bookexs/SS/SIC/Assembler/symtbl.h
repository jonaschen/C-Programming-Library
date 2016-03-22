#ifndef __SYMTBL_H
#define __SYMTBL_H

#include <stdint.h>
#include "chtbl.h"

struct symtbl_entry {
	char *symbol;
	uint32_t location;
}; 


#endif
