#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

struct register_files {
	unsigned int	eax;
	unsigned int	ecx;
	unsigned int	edx;
	unsigned int	ebx;
	unsigned int	esi;
	unsigned int	edi;
	unsigned int	esp;
	unsigned int	ebp;
};

#define MAIN_MEMORY_SIZE	(8 * 1024)
struct y86_exec_env {
	struct register_files grf;	/* general purpose registers */
	unsigned char condition;	/* condition codes */
	unsigned int program_count;	/* program counter */
	unsigned int stat;		/* program status */
	unsigned char *memory;
};

struct y86_instruction {
	unsigned char code;
	unsigned char rindex;		/* index of registers rA, rB */
	unsigned int value;
};
#define ICODE(code)	((code & 0xF0) >> 4)
#define IFUNC(code)	(code & 0x0F)
#define OP_RA(index)	((index & 0xF0) >> 4)
#define OP_RB(index)	(index & 0x0F)


enum {
	ERR_OK = 0,
	ERR_OUT_MEMORY = -1,
	ERR_UNRECOGNIZE = -2,
};


#define SYSTEM_TICK_UNIT_MS	(50 * 1000)
#define SYSTEM_TICK_SCALE	20
#define SYSTEM_TICK		(SYSTEM_TICK_UNIT_MS * SYSTEM_TICK_SCALE)


int y86_exec(struct y86_instruction *imem, struct y86_exec_env *next);
int y86_fetch(struct y86_exec_env *env, struct y86_instruction *imem, unsigned int *next_pc);
int y86_write_back(struct y86_instruction *imem, struct y86_exec_env *env, struct y86_exec_env *next);
