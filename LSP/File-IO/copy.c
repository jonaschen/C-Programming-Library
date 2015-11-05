#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lsp-common.h"

const char HELP_STR[] = "--help";

void usage(const char *prog)
{
	fprintf(stderr, "usage: %s src dest\n", prog);
}

int main(int argc, char *argv[])
{
	int in_fd, out_fd, flags;
	mode_t perms;
	ssize_t bytes;
	char buf[CHAR_BUF_SIZE];

	if (argc != 3 || !strncmp(argv[1], HELP_STR, strlen(HELP_STR))) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
