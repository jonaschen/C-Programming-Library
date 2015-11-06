#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lsp-common.h"

const char HELP_STR[] = "--help";
#define OPEN_FLAG 	(O_CREAT | O_WRONLY | O_TRUNC)

#define PERM_RW_USER	(S_IRUSR | S_IWUSR)
#define PERM_RW_GRP	(S_IRGRP | S_IWGRP)
#define PERM_RW_OTH	(S_IROTH | S_IWOTH)
#define PERM_RW_ALL	(PERM_RW_USER | PERM_RW_GRP | PERM_RW_OTH)

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

	in_fd = open(argv[1], O_RDONLY);
	if (in_fd == -1) {
		perror("open input");
		exit(EXIT_FAILURE);
	}

	flags = OPEN_FLAG;
	perms = PERM_RW_ALL;
	out_fd = open(argv[2], flags, perms);
	if (out_fd == -1) {
		perror("open output");
		exit(EXIT_FAILURE);
	}

	while ((bytes = read(in_fd, buf, CHAR_BUF_SIZE)) > 0) {
		if (write(out_fd, buf, bytes) != bytes) {
			perror("write");
			exit(EXIT_FAILURE);
		}
	}

	close(in_fd);
	close(out_fd);

	exit(EXIT_SUCCESS);
}
