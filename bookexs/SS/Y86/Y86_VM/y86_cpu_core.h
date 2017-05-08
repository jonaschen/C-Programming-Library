#ifndef __Y86_CPU_CORE_H
#define __Y86_CPU_CORE_H

#include <stdint.h>

enum y86_register_type {
	Y86_GPR_EAX = 0,
	Y86_GPR_ECX,
	Y86_GPR_EDX,
	Y86_GPR_EBX,
	Y86_GPR_ESI,
	Y86_GPR_EDI,
	Y86_GPR_ESP,
	Y86_GPR_EBP,

	Y86_GPR_NUM,
};

struct register_files {
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esi;
	uint32_t edi;
	uint32_t esp;
	uint32_t ebp;
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

struct y86_cpu_core {
	struct register_files gpr;	/* general purpose registers */
	uint32_t program_count;	     	/* program counter */

	uint32_t status;               	/* program status */
	uint8_t condition;         	/* condition codes */

	int (*exec)(struct y86_cpu_core *core, struct y86_instruction *imem);
	int (*fetch)(struct y86_cpu_core *core, struct y86_instruction *imem, unsigned char *memory);
};

struct y86_cpu_core *y86_cpu_core_new(uint32_t init_pc);
void y86_cpu_core_cleanup(struct y86_cpu_core *core);


enum {
	ERR_OK = 0,
	ERR_OUT_MEMORY = -1,
	ERR_UNRECOGNIZE = -2,
};

#endif
