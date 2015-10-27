#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	size_t len = 0;
	ssize_t bytes;
	char *strptr;

	if (argc < 2) {
		fprintf(stderr, "usage: %s pattern\n", argv[0]);
		return -1;
	}

	while ((bytes = getline(&strptr, &len, stdin)) > 0) {
		if (strstr(strptr, argv[1])) {
			printf("%s", strptr);
		}
	}

	return 0;
}
