#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int getline_j(char *buf, int size, FILE *stream)
{
	int c, cnt = 0;
	char *ptr = buf;

	while ((c = fgetc(stream)) != EOF) {
		ptr[cnt++] = (char) c;
		if (cnt == size - 1)
			break;
		if (c == '\n')
			break;
	}

	ptr[cnt] = '\0';

	return cnt;
}

enum state_t {
	IN_WORD = 0,
	OUT_WORD,
};

#define MAX_WORDS	256
#define IS_WORD(c)	(isalnum(c))
int getwords(char *str, size_t len, char ***pwords)
{
	int cnt = 0, ret = 0;
	char c;
	char *start ;
	char *buf, *word;
	enum state_t state;
	char **words;

	buf = strdup((const char *) str);
	words = (char **) malloc(MAX_WORDS * sizeof(char *));

	if (IS_WORD(buf[0])) {
		state = IN_WORD;
		start = buf;
	} else {
		state = OUT_WORD;
	}

	do {
		c = buf[cnt++];
		if (state == IN_WORD) {
			if (!IS_WORD(c)) {
				state = OUT_WORD;
				buf[cnt - 1] = '\0';
				word = strdup(start);
				words[ret++] = word;
			}
		} else if (state == OUT_WORD) {
			if (IS_WORD(c)) {
				state = IN_WORD;
				start = &buf[cnt - 1];
			}
		}
	} while (cnt < len);


	*pwords = words;
	return ret;
}

#define BUF_SIZE 1024
int main(int argc, char *argv[])
{
	int len, n, i;
	char buf[1024];
	char **words;

	while ((len = getline_j(buf, BUF_SIZE, stdin)) > 0) {
		n = getwords(buf, (size_t) len, &words);
		for (i = 0; i < n; i++)
			printf("word:%s\n", words[i]);
	}

	return 0;
}
