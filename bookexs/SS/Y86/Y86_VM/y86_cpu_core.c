#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "y86_cpu_core.h"

static void dump_instruction(struct y86_instruction *imem)
{
	printf("\n[%s]\n", __func__);
	printf("code: 0x%02X\n", imem->code);
	printf("\tregister index: 0x%02X\n", imem->rindex);
	printf("\timmdediate value: 0x%08X\n", imem->value);
}

static uint32_t exec_branch(struct y86_cpu_core *core, struct y86_instruction *jcode)
{
	unsigned char func;
	uint32_t next_pc;

	func = IFUNC(jcode->code);
	
	switch (func) {
	case 0x0:
		next_pc = jcode->value;
		break;
	default:
		next_pc = 0;
		break;
	}

	return next_pc;
}

static int y86_cpu_core_exec(struct y86_cpu_core *core, struct y86_instruction *imem)
{
	struct register_files gpr;	/* general purpose registers */
	uint32_t next_pc = core->program_count;

	//uint32_t status;               	/* program status */
	//uint8_t condition;         	/* condition codes */

	unsigned char icode;
	unsigned int *rb;

	dump_instruction(imem);

	memcpy(&gpr, &core->gpr, sizeof(struct register_files));

	icode = ICODE(imem->code);

	switch (icode) {
	case 0x1:
		break;
	case 0x3:
		rb = (unsigned int *) &gpr;

		rb += OP_RB(imem->rindex);
		*(rb) = imem->value;
		break;
	case 0x7:
		next_pc = exec_branch(core, imem);
		break;
	case 0x0:
	default:
		return -1;
	}

	/* write back */
	memcpy(&core->gpr, &gpr, sizeof(struct register_files));
	core->program_count = next_pc;

	return 0;
}


static int y86_cpu_core_fetch(struct y86_cpu_core *core, struct y86_instruction *imem, unsigned char *memory)
{
	int i;
	uint32_t pc = core->program_count;
	unsigned char code;
	unsigned int value = 0U;

	memset(imem, 0, sizeof(struct y86_instruction));

	//if (pc >= MAIN_MEMORY_SIZE)
	//	return ERR_OUT_MEMORY;

	code = memory[pc++];
	imem->code = code;

	switch (ICODE(code)) {
	case 0:
	case 1:
		break;

	case 3:
		imem->rindex = memory[pc++];
		for (i = 0; i < 4; i++)
			value = (value << 8) + memory[pc++];

		imem->value = value;
		break;

	case 7:
		for (i = 0; i < 4; i++)
			value = (value << 8) + memory[pc++];

		imem->value = value;
		break;

	default:
		return ERR_UNRECOGNIZE;
	}

	core->program_count = pc;

	return ERR_OK;
}



static void y86_cpu_core_init(struct y86_cpu_core *core, uint32_t init_pc)
{
	memset(core, 0, sizeof(*core));

	core->program_count = init_pc;

	core->exec = y86_cpu_core_exec;
	core->fetch = y86_cpu_core_fetch;
}

struct y86_cpu_core *y86_cpu_core_new(uint32_t init_pc)
{
	struct y86_cpu_core *core;

	core = (struct y86_cpu_core *) malloc(sizeof(*core));

	y86_cpu_core_init(core, init_pc);

	return core;
}

void y86_cpu_core_cleanup(struct y86_cpu_core *core)
{
	free(core);
}
