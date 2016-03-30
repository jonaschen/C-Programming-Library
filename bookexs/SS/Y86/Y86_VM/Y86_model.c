#include "Y86_common.h"

static void dump_instruction(struct y86_instruction *imem)
{
	printf("\n[%s]\n", __func__);
	printf("code: 0x%02X\n", imem->code);
	printf("\tregister index: 0x%02X\n", imem->rindex);
	printf("\timmdediate value: 0x%08X\n", imem->value);
}

static int exec_branch(struct y86_instruction *jcode, struct y86_exec_env *next)
{
	unsigned char func;

	func = IFUNC(jcode->code);
	
	switch (func) {
	case 0x0:
		next->program_count = jcode->value;
		break;
	default:
		return -1;
	}

	return 0;
}

int y86_exec(struct y86_instruction *imem, struct y86_exec_env *next)
{
	unsigned char icode;
	unsigned int *rb;

	dump_instruction(imem);


	icode = ICODE(imem->code);

	switch (icode) {
	case 0x1:
		break;
	case 0x3:
		rb = (unsigned int *) &next->grf;

		rb += OP_RB(imem->rindex);
		*(rb) = imem->value;
		break;
	case 0x7:
		exec_branch(imem, next);
		break;
	case 0x0:
	default:
		return -1;
	}

	return 0;
}

int y86_write_back(struct y86_instruction *imem, struct y86_exec_env *env, struct y86_exec_env *next)
{
	unsigned int *ra, *rb, *new;

	ra = (unsigned int *) &env->grf;
	rb = (unsigned int *) &env->grf;
	new = (unsigned int *) &next->grf;

	ra += OP_RA(imem->rindex);
	rb += OP_RB(imem->rindex);

	env->program_count = next->program_count;

	switch (ICODE(imem->code)) {
	case 0x3:
		*rb = *(new + OP_RB(imem->rindex));
		break;
	default:
		return -1;
	}

	return 0;
}


int y86_fetch(struct y86_exec_env *env, struct y86_instruction *imem, unsigned int *next_pc)
{
	int i;
	unsigned int pc = env->program_count;
	unsigned char code;
	unsigned int value = 0U;

	memset(imem, 0, sizeof(struct y86_instruction));

	if (pc >= MAIN_MEMORY_SIZE)
		return ERR_OUT_MEMORY;

	code = env->memory[pc++];
	imem->code = code;

	switch (ICODE(code)) {
	case 0:
	case 1:
		break;

	case 3:
		imem->rindex = env->memory[pc++];
		for (i = 0; i < 4; i++)
			value = (value << 8) + env->memory[pc++];

		imem->value = value;
		break;

	case 7:
		for (i = 0; i < 4; i++)
			value = (value << 8) + env->memory[pc++];

		imem->value = value;
		break;

	default:
		return ERR_UNRECOGNIZE;
	}

	*next_pc = pc;

	return ERR_OK;
}


