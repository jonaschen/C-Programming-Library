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
static const char OBJECT_FILE[] = "sic.obj";

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
static int asm_parse_line = 0;
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

/*
 *
 */
typedef enum _parsing_state_t {
	STATE_INIT,
	STATE_IN_LABEL,
	STATE_BEFORE_OPCODE,
	STATE_IN_OPCODE,
	STATE_BEFORE_OPERAND,
	STATE_IN_OPERAND,
} parsing_state_t;

static int scan_parse_columns(char *line, struct instruction_t *instr)
{
	char *label = NULL, *opcode = NULL, *operand = NULL, *temp;
	char *ptr = line, c;
	int len;
	parsing_state_t state = STATE_INIT;

	do {
		switch (state) {
		case STATE_INIT:
			if (IS_LINE_END(*ptr))
				break;
			if (!isspace(*ptr)) {
				temp = ptr;
				state = STATE_IN_LABEL;
				len = 1;
			} else {
				state = STATE_BEFORE_OPCODE;
			}
			break;

		case STATE_IN_LABEL:
			if (isalnum(*ptr) || IS_COLON(*ptr)) {
				len++;
			} else {
				label = (char *) malloc(len + 1);
				memcpy(label, temp, len);
				label[len] = '\0';
				state = STATE_BEFORE_OPCODE;
			}
			break;

		case STATE_IN_OPCODE:
			if (isalnum(*ptr)) {
				len++;
			} else {
				opcode = (char *) malloc(len + 1);
				memcpy(opcode, temp, len);
				opcode[len] = '\0';
				state = STATE_BEFORE_OPERAND;
			}
			break;

		case STATE_IN_OPERAND:
			if (isspace(*ptr) || IS_LINE_END(*ptr)) {
				operand = (char *) malloc(len + 1);
				memcpy(operand, temp, len);
				operand[len] = '\0';
				break;
			} else {
				len++;
			}
			break;

		case STATE_BEFORE_OPCODE:
		case STATE_BEFORE_OPERAND:
			if (IS_LINE_END(*ptr))
				break;
			if (!isspace(*ptr)) {
				state++;
				temp = ptr;
				len = 1;
			}
			break;
		default:
			break;
		}
		c = *ptr++;
	} while (!IS_LINE_END(c));

	instr->label = label;
	instr->opcode = opcode;
	instr->operand = operand;

	if (!label && !opcode)
		return -1;

	return 0;
}

static int asm_to_object(FILE *obj, char *line, struct instruction_t *instr)
{
	char *addr, *opcode, *operand;
	char *dup, *ptr;
	struct sic_directive *dirc, e_dirc;
	struct op_entry *op, e_op;
	int ret, cnt;

	addr = instr->label;
	opcode = instr->opcode;
	operand = instr->operand;

	fprintf(stderr, "%s:%s:%s\n", addr, opcode, (operand == NULL) ? "": operand);

	if (asm_parse_line >= 10) {
		flush_text_record(obj);
		location_counter = utils_atoh(addr);
		asm_parse_line = 0;
	}

	dirc = &e_dirc;
	op = &e_op;
	e_dirc.directive = opcode;
	e_op.memonic = opcode;
	if (chtbl_lookup(&directive_table, (void **) &dirc) == 0) {
		asm_parse_line++;
		if (dirc->assemble) {
			ret = dirc->assemble(addr, operand, &text_record[record_len]);
			if (ret == DIRECT_START) {
				fprintf(obj, "%-6s%06X%06X", program_name, start_addr, program_len); 
				location_counter = start_addr;
				asm_parse_line = 0;
			} else if (ret == DIRECT_END) {
				if (record_len) {
					flush_text_record(obj);
					location_counter = utils_atoh(addr);
				}
				fprintf(obj, "\nE%06X", start_addr); 
			} else {
				record_len += ret;
			}
		}
	} else if (chtbl_lookup(&op_table, (void **) &op) == 0) {
		asm_parse_line++;
		if (op->assemble) {
			sprintf(&text_record[record_len], "%02X", op->opcode);
			record_len += 2;
			record_len += op->assemble(operand, &symbol_table, &text_record[record_len]);
		}
	}

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
 *
 */
static int special_symbol_handler(FILE *source, char *ch)
{
	int len = 0, ret;
	char c = *ch;

	/* TODO: error handling of file operations */
	switch (c) {
	case ';':
		do {
			ret = fread(&c, sizeof(char), 1, source);
			len++;
		} while (c != '\n' && ret == 1);
		*ch = '\0';
		break;
	case ',':
		do {
			ret = fread(&c, sizeof(char), 1, source);
			len++;
		} while (isspace(c) && ret == 1);

		if (ret != 1) {
			*ch = '\0';
		} else {
			ret = fseek(source, -1, SEEK_CUR);
			*ch = ',';
		}
		break;
	default:
		break;
	}

	return len;
}

/*
 * Get one line of instruction. Comments are stripped out.
 */
static int get_line_instruction(FILE *source, char *buffer)
{
	char c;
	int len = 0, ret;
	size_t bytes;

	do {
		ret = fread(&c, sizeof(char), 1, source);
		if (ret != 1) {
			/* TODO: error handling if ret < 0 */
			break;
		}

		if (IS_SPECIAL_SYMBOL(c))
			special_symbol_handler(source, &c);

		if (c == '\n') {
			c = '\0';
		}

		buffer[len++] = c;
	} while (c != '\0');

	return len;
}

int main(int argc, char *argv[])
{
	FILE *source, *object;
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
			ret = scan_parse_columns(buffer, &instr);
			if (ret >= 0) {
				update_location_cntr(&instr);
				if (instr.label) free(instr.label);
				if (instr.opcode) free(instr.opcode);
				if (instr.operand) free(instr.operand);
			} else {
				/* TODO: error handling */
			}
		} else if (ret == 0) {
			break;
		}
		line_number++;
	}
	fclose(source);
	fclose(intermediate);

	intermediate = fopen(INTER_FILE, "r");
	object = fopen(OBJECT_FILE, "w");
	fprintf(object, "H");
	while (1) {
		memset(buffer, '\0', sizeof(char) * BUF_LEN);
		ret = get_line_instruction(intermediate, buffer);
		if (ret > 1) {
			ret = scan_parse_columns(buffer, &instr);
			asm_to_object(object, buffer, &instr);
	//fprintf(object, "%-6s", "COPY");
		} else if (ret == 0) {
			break;
		}
	}

	fclose(object);
	fclose(intermediate);
	chtbl_destroy(&symbol_table);

	return 0;
}
