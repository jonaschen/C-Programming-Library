#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "optbl.h"

#define	BUF_LEN	4096

static const char FILE_NAME[] = "sic_sample_1.S";

static struct chtbl_t op_table;

static int parse_columns(int number, char *line)
{
	char *label, *opcode, *operand;
	char *ptr = line;
	struct op_entry *t, temp;

	printf("%d:  %s\n", number, line);

	if (!strlen(line))
		return -1;

	if (isspace(*ptr)) {
		label = NULL;
	} else {
		label = line;
		while (!isspace(*ptr++));
		ptr--;
		*ptr++ = '\0';
	}

	if (isspace(*ptr)) {
		while (isspace(*ptr++));
		ptr--;
	}

	opcode = ptr;
	while (!isspace(*ptr++));
	ptr--;

	*ptr++ = '\0';
	if (strlen(opcode) == 0)
		opcode = NULL;

	if (isspace(*ptr)) {
		while (isspace(*ptr++));
		ptr--;
	} 
	if (isalnum(*ptr)) {
		operand = ptr;
		while (isalnum(*ptr))
			ptr++;
	} else
		operand = NULL;

	if (label)
		printf("\t\tlabel:%s\n", label);
	if (opcode) {
		temp.memonic = opcode;
		t = &temp;
		if (chtbl_lookup(&op_table, (void **) &t)) {
			printf("\t\tundefined opcode:%s\n", opcode);
		} else {
			printf("\t\topcode:%s:%02X\n", opcode, t->opcode);
		}

	}
	if (operand)
		printf("\t\toperand:%s\n", operand);

	memset(line, '\0', sizeof(char) * BUF_LEN);

	return 0;
}

int main(int argc, char *argv[])
{
	FILE *source;
	char buffer[BUF_LEN];
	char c;
	int len = 0;
	int line_number = 0;

	if (optbl_init(&op_table)) {
		printf("init optbl fail\n");
		exit(1);
	}

	source = fopen(FILE_NAME, "r");
	memset(buffer, '\0', sizeof(char) * BUF_LEN);

	do {
		if (1 != fread(&c, sizeof(char), 1, source))
			break;

		if (c == ';') {
			while (c != '\n') {
				if (1 != fread(&c, sizeof(char), 1, source))
					exit(1);
			}
		}

		if (c == '\n') {
			line_number++;
			c = '\0';
		}

		buffer[len++] = c;
		if (c == '\0' || (len == BUF_LEN - 1)) {
			parse_columns(line_number, buffer);
			len = 0;
		}
	} while (c != EOF);

	fclose(source);

	return 0;
}
