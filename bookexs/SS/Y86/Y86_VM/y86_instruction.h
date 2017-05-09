#ifndef _Y86_INSTRUCTION_H
#define _Y86_INSTRUCTION_H

enum y86_inst_op {
	Y86_INST_HALT = 0,
	Y86_INST_NOP,
	Y86_INST_RRMOV,
	Y86_INST_IRMOV,
	Y86_INST_RMMOV,
	Y86_INST_MRMOV,
	Y86_INST_OPL,
	Y86_INST_JMP,

	Y86_INST_CALL,
	Y86_INST_RET,
	Y86_INST_PUSH,
	Y86_INST_POP,

	Y86_INST_TYPES,
};

enum y86_mov_fn {
	Y86_RRMOVL = 0,
	Y86_CMOVLE,
	Y86_CMOVL,
	Y86_CMOVE,
	Y86_CMOVNE,
	Y86_CMOVGE,
	Y86_CMOVG,

	Y86_MOV_FN_TYPES,
};

enum y86_jmp_fn {
	Y86_JMP_DIRECT = 0,
	Y86_JMP_JLE,
	Y86_JMP_JL,
	Y86_JMP_JE,
	Y86_JMP_JNE,
	Y86_JMP_JGE,
	Y86_JMP_JG,

	Y86_JMP_FN_TYPES,
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

#endif
