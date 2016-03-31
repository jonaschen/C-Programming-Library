#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define VIR_MEMORY_SIZE		(40 * 1024)

static const char OBJECT_FILE[] = "sic.obj";

static void dump_main_memory(void *addr, uint32_t offset, uint32_t size)
{
	unsigned char *ptr = (unsigned char *) addr + offset;
	uint32_t i = 0;

	while (i < size) {
		if (!(i % 16)) fprintf(stdout, "%04X:", offset + i);
		fprintf(stdout, "%02X", *(ptr + i));
		i++;
		if (!(i % 16)) fprintf(stdout, "\n");
		else if (!(i % 4)) fprintf(stdout, "  ");
	}
	fprintf(stdout, "\n");

	return;
}

static int get_text_record(FILE *source, char *buffer)
{
	char c;
	int len = 0;
	size_t bytes;

	do {
		if (1 != fread(&c, sizeof(char), 1, source)) {
			buffer[len] = '\0';
			break;
		}

		if (c == '\n') {
			c = '\0';
		}

		buffer[len++] = c;
	} while (c != '\0');

	return len;
}

static int load_into_memory(void *memory, char *buffer)
{
	char address[5];
	char length[3];
	char code[3];
	uint32_t addr, len, bin;
	unsigned char *ptr = buffer + 3, *pos;

	memcpy(address, ptr, 4);
	address[4] = 0;
	ptr += 4;

	length[0] = *ptr++;
	length[1] = *ptr++;
	length[2] = 0;

	sscanf(address, "%06X", &addr);
	sscanf(length, "%02X", &len);
	fprintf(stdout, "(%06X, %02X)\n", addr, len);

	code[2] = 0;
	pos = (unsigned char *) memory + addr;
	while (len) {
		code[0] = *ptr++;
		code[1] = *ptr++;
		sscanf(code, "%02X", &bin);
		*pos++ = (unsigned char) bin;
		len--;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	FILE *obj;
	void *memory;
	uint32_t addr;
	char buffer[100];
	uint32_t len;

	obj = fopen(OBJECT_FILE, "r");
	memory = malloc(VIR_MEMORY_SIZE);
	memset(memory, 0, VIR_MEMORY_SIZE);

	do {
		len = (uint32_t) get_text_record(obj, buffer);
		switch (buffer[0]) {
		case 'H':
			break;
		case 'T':
			load_into_memory(memory, buffer);
			break;
		case 'E':
			len = -1;
			break;
		default:
			/* error */
			break;
		}
	} while (len > 0);

	fclose(obj);

	dump_main_memory(memory, 4 * 1024, 128);
	dump_main_memory(memory, 8 * 1024, 256);
	free(memory);

	return 0;
}
