#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "optbl.h"
#include "directive.h"

#define	BUF_LEN	4096

static const char FILE_NAME[] = "sic_sample_1.S";

static struct chtbl_t op_table;
static struct chtbl_t directive_table;

static uint32_t location_counter = 0U;

struct instruction_t {
	char *label;
	char *opcode;
	char *operand;
};

static int parse_columns(int number, char *line, struct instruction_t *instr)
{
	char *label, *opcode, *operand;
	char *ptr = line;

	/* Parse label column */
	if (isspace(*ptr)) {
		label = NULL;
	} else {
		label = line;
		while (!isspace(*ptr++));
		ptr--;
		*ptr++ = '\0';
	}

	/* Parse opcode column */
	while (isspace(*ptr))
		ptr++;

	opcode = ptr;
	if (strlen(opcode) == 0) {
		opcode = NULL;
	} else {
		while (isalnum(*ptr))
			ptr++;
		*ptr++ = '\0';
	}

	/* Parse operand column */
	while (isspace(*ptr))
		ptr++;

	operand = ptr;
	if (strlen(operand) == 0) {
		operand = NULL;
	} else {
		while (!isspace(*ptr) && *ptr != '\0')
			ptr++;
		*ptr = '\0';
	}

	instr->label = label;
	instr->opcode = opcode;
	instr->operand = operand;

	return 0;
}

static int update_location_cntr(struct instruction_t *instr)
{
	char *label, *opcode, *operand;
	struct op_entry *op, e_op;
	struct sic_directive *dir, e_dir;

	label = instr->label;
	opcode = instr->opcode;
	operand = instr->operand;

	printf("%04X", location_counter);

	if (label) {
		/* TODO */
		/* update symbol table */
		printf("\t%s\t", label);
	} else {
		printf("\t\t");
	}

	if (opcode) {
		e_op.memonic = opcode;
		op = &e_op;
		if (chtbl_lookup(&op_table, (void **) &op) == 0) {
			printf("opcode:%s\t\t%02X", opcode, op->opcode);
		} else {
			op = NULL;
			e_dir.directive = opcode;
			dir = &e_dir;
			if (chtbl_lookup(&directive_table, (void **) &dir) == 0) {
				printf("directive:%s", opcode);
			} else {
				dir = NULL;
				printf("undefined opcode:%s", opcode);
			}
		}
	}

	if (operand) {
		//printf("\t\toperand:%s\n", operand);
	}

	if (op) {
		optbl_lc(op->memonic, operand, &location_counter);
	} else if (dir) {
		if (dir->location_update(dir->directive, operand, &location_counter)) {
			printf("\n");
			return 1;
		}
	}

	printf("\n");
	//printf("\t\tlocation:%04x\n", location_counter);

	return 0;
}

/*
 * Get one line of instruction. Comments are stripped out.
 */
static int get_line_instruction(FILE *source, char *buffer)
{
	char c;
	int len = 0;
	size_t bytes;

	do {
		if (1 != fread(&c, sizeof(char), 1, source)) {
			break;
		}

		if (c == ';') {
			do {
				bytes = fread(&c, sizeof(char), 1, source);
			} while (c != '\n' && bytes == 1);
			c = '\n';
		}

		if (c == '\n') {
			c = '\0';
		}

		buffer[len++] = c;
	} while (c != '\0');

	return len;
}

int main(int argc, char *argv[])
{
	FILE *source;
	char buffer[BUF_LEN];
	int line_number = 0;
	int ret;
	struct instruction_t instr;

	if (optbl_init(&op_table)) {
		printf("init optbl fail\n");
		exit(1);
	}
	
	if (directives_init(&directive_table)) {
		printf("init directive table fail\n");
		exit(1);
	}

	source = fopen(FILE_NAME, "r");

	while (1) {
		memset(buffer, '\0', sizeof(char) * BUF_LEN);
		ret = get_line_instruction(source, buffer);
		if (ret > 1) {
			ret = parse_columns(line_number, buffer, &instr);
			if (ret >= 0)
				update_location_cntr(&instr);
			if (ret == 1)
				break;
		} else if (ret == 0) {
			break;
		}
		line_number++;
	}

	fclose(source);

	return 0;
}
