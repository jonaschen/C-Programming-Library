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

static int y86_exec_jump(struct y86_cpu_core *core, struct y86_instruction *jcode)
{
	unsigned char func;
	uint32_t next_pc;

	func = IFUNC(jcode->code);
	
	switch (func) {
	case Y86_JMP_DIRECT:
		next_pc = jcode->value;
		break;
	default:
		next_pc = 0;
		break;
	}

	/* write back */
	core->program_count = next_pc;

	return 0;
}

static int y86_exec_irmov(struct y86_cpu_core *core, struct y86_instruction *imem)
{
	struct register_files gpr;	/* general purpose registers */
	unsigned int *rb;

	memcpy(&gpr, &core->gpr, sizeof(struct register_files));

	rb = (unsigned int *) &gpr;

	rb += OP_RB(imem->rindex);
	*(rb) = imem->value;

	/* write back */
	memcpy(&core->gpr, &gpr, sizeof(struct register_files));

	return 0;
}

static int y86_cpu_core_exec(struct y86_cpu_core *core, struct y86_instruction *imem)
{
	unsigned char icode;

	dump_instruction(imem);

	icode = ICODE(imem->code);

	switch (icode) {
	case Y86_INST_NOP:
		break;
	case Y86_INST_IRMOV:
		y86_exec_irmov(core, imem);
		break;
	case Y86_INST_JMP:
		y86_exec_jump(core, imem);

		break;
	case Y86_INST_HALT:
	default:
		return -1;
	}


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
	case Y86_INST_HALT:
	case Y86_INST_NOP:
		break;

	case Y86_INST_IRMOV:
		imem->rindex = memory[pc++];
		for (i = 0; i < 4; i++)
			value = (value << 8) + memory[pc++];

		imem->value = value;
		break;

	case Y86_INST_JMP:
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

	/* TODO: set instruction memory, data memory, stack */
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
