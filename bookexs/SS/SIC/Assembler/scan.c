#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "optbl.h"
#include "directive.h"
#include "symtbl.h"
#include "utils.h"

#define	BUF_LEN	4096

static const char SRC_FILE[] = "sic_sample_1.S";
static const char INTER_FILE[] = "inter.d";
static const char MACHINE_FILE[] = "sic.bin";

static struct chtbl_t op_table;
static struct chtbl_t directive_table;
static struct chtbl_t symbol_table;

static uint32_t location_counter = 0U;
static uint32_t start_addr = 0U;
static uint32_t program_len = 0U;
static FILE *intermediate = NULL;
static char *program_name = NULL;

#define MAX_TEXT_RECORD_LEN	100
static int record_len = 0;
static char text_record[MAX_TEXT_RECORD_LEN + 1];

struct instruction_t {
	char *label;
	char *opcode;
	char *operand;
};
static int flush_text_record(FILE *obj)
{
	if (record_len)
		fprintf(obj, "\nT%06X%02X%s", location_counter, record_len / 2, text_record);

	record_len = 0;
	memset(text_record, 0, MAX_TEXT_RECORD_LEN); 

	return 0;
}
static int scan_parse_columns(int number, char *line, struct instruction_t *instr)
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

static int asm_parse_columns(char *line, FILE *bin)
{
	char *addr, *opcode, *operand;
	char *dup, *ptr;
	struct sic_directive *dirc, e_dirc;
	struct op_entry *op, e_op;
	int ret, cnt;

	dup = (char *) malloc(strlen(line) + 1);

	sprintf(dup, "%s", line);
	ptr = dup;

	/* Parse addr column */
	addr = ptr;
	*(addr + 4) = '\0'; 

	/* Parse opcode column */
	opcode = ptr = addr + 5;
	while (!isspace(*ptr) && *ptr != '\0')
		ptr++;
	*ptr++ = '\0';

	/* Parse operand column */
	while (isspace(*ptr))
		ptr++;

	operand = ptr;
	while (!isspace(*ptr) && *ptr != '\0')
		ptr++;
	*ptr = '\0';
	if (!strlen(operand))
		operand = NULL;

	fprintf(stdout, "%s:%s:%s\n", addr, opcode, (operand == NULL) ? "": operand);

	if (record_len >= 60) {
		flush_text_record(bin);
		location_counter = utils_atoh(addr);
	}

	dirc = &e_dirc;
	op = &e_op;
	e_dirc.directive = opcode;
	e_op.memonic = opcode;
	if (chtbl_lookup(&directive_table, (void **) &dirc) == 0) {
		if (dirc->assemble) {
			ret = dirc->assemble(addr, operand, bin);
			if (ret == DIRECT_START) {
				fprintf(bin, "%-6s%06X%06X", program_name, start_addr, program_len); 
				location_counter = start_addr;
			} else if (ret == DIRECT_RSUB) {
				sprintf(&text_record[record_len], "4C0000");
				record_len += 6;
				flush_text_record(bin);
				location_counter = utils_atoh(addr);
			} else if (ret == DIRECT_BYTE) {
				ptr = operand + 2;

				if (*operand == 'C') {
					cnt = (strlen(operand) - 3);
					while (cnt) {
						sprintf(&text_record[record_len++], "%2X", *ptr++);
						cnt--;
					}
				} else if (*operand == 'X') {
					sprintf(&text_record[record_len++], "%C%C", *ptr, *(ptr + 1));
				}
			}
		}
	} else if (chtbl_lookup(&op_table, (void **) &op) == 0) {
		if (op->assemble) {
			sprintf(&text_record[record_len], "%02X", op->opcode);
			record_len += 2;
			record_len += op->assemble(operand, &symbol_table, &text_record[record_len]);
		}
	}

	free(dup);

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
			program_name = (char *) malloc(strlen(label) + 1);
			sprintf(program_name, "%s", label);
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
	FILE *source, *image;
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
			ret = scan_parse_columns(line_number, buffer, &instr);
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
	image = fopen(MACHINE_FILE, "w");
	fprintf(image, "H");
	while (1) {
		memset(buffer, '\0', sizeof(char) * BUF_LEN);
		ret = get_line_instruction(intermediate, buffer);
		if (ret > 1) {
			asm_parse_columns(buffer, image);
	//fprintf(image, "%-6s", "COPY");
		} else if (ret == 0) {
			break;
		}
	}

	fclose(image);
	fclose(intermediate);
	chtbl_destroy(&symbol_table);

	return 0;
}
