#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "optbl.h"
#include "directive.h"
#include "symtbl.h"

#define	BUF_LEN	4096

static const char SRC_FILE[] = "sic_sample_1.S";
static const char INTER_FILE[] = "inter.d";

static struct chtbl_t op_table;
static struct chtbl_t directive_table;
static struct chtbl_t symbol_table;

static uint32_t location_counter = 0U;
static uint32_t start_addr = 0U;
static uint32_t program_len = 0U;
static FILE *intermediate = NULL;

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
	struct symtbl_entry *e_symbol;
	int ret;

	label = instr->label;
	opcode = instr->opcode;
	operand = instr->operand;

	fprintf(intermediate, "%04X", location_counter);

	if (label) {
		/* TODO */
		/* update symbol table */
		e_symbol = (struct symtbl_entry *) malloc(sizeof(struct symtbl_entry));
		label = (char *) malloc(strlen(instr->label) + 1);
		sprintf(label, "%s", instr->label);

		e_symbol->symbol = label;
		e_symbol->location = location_counter;
		chtbl_insert(&symbol_table, e_symbol);

		//fprintf(intermediate, "\t%s\t", label);
	} else {
		//fprintf(intermediate, "\t\t");
	}

	if (opcode) {
		e_op.memonic = opcode;
		op = &e_op;
		if (chtbl_lookup(&op_table, (void **) &op) == 0) {
			//fprintf(intermediate, "\t%s\t\t%02X", opcode, op->opcode);
			fprintf(intermediate, "\t%s", opcode);
		} else {
			op = NULL;
			e_dir.directive = opcode;
			dir = &e_dir;
			if (chtbl_lookup(&directive_table, (void **) &dir) == 0) {
				fprintf(intermediate, "\t%s", opcode);
			} else {
				dir = NULL;
				fprintf(stderr, "undefined opcode:%s", opcode);
			}
		}
	}

	if (operand) {
		fprintf(intermediate, "\t%s", operand);
	}

	if (op) {
		optbl_lc(op->memonic, operand, &location_counter);
	} else if (dir) {
		ret = dir->location_update(dir->directive, operand, &location_counter);
		if (ret == DIRECT_START) {
			start_addr = location_counter;
		} else if (ret == DIRECT_END) {
			program_len = location_counter - start_addr;
			fprintf(intermediate, "\n");
			return 1;
		}
	}

	fprintf(intermediate, "\n");
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

	intermediate = stdout;

	if (optbl_init(&op_table)) {
		printf("init optbl fail\n");
		exit(1);
	}
	
	if (directives_init(&directive_table)) {
		printf("init directive table fail\n");
		exit(1);
	}
	symbol_table_init(&symbol_table);

	source = fopen(SRC_FILE, "r");
	intermediate = fopen(INTER_FILE, "w");

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
	fclose(intermediate);

	intermediate = fopen(INTER_FILE, "r");
	while (1) {
		memset(buffer, '\0', sizeof(char) * BUF_LEN);
		ret = get_line_instruction(intermediate, buffer);
		printf("%s\n", buffer);
		if (ret > 1) {
		} else if (ret == 0) {
			break;
		}
	}

	fclose(intermediate);
	chtbl_destroy(&symbol_table);

	return 0;
}
