#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state_t {
	IN_WORD = 0,
	QUOTED_STRING,
	COMMENT,
	OUT_WORD,
};

typedef enum state_t	word_parsing_t;

#define MAX_WORDS_PER_LINE	256
#define IS_WORD(c)	((isalnum(c)) || (c) == '_')
int is_word(char c, word_parsing_t *state)
{
}

int getwords(char *str, size_t len, char ***pwords)
{
	int cnt = 0, ret = 0;
	char c;
	char *start ;
	char *buf, *word;
	word_parsing_t state;
	char **words;

	if (!str || !len || !pwords)
		return -1;

	buf = strdup((const char *) str);
	if (buf == NULL) {
		perror("strdup");
		return -1;
	}
	words = (char **) malloc(MAX_WORDS_PER_LINE * sizeof(char *));
	if (!words) {
		perror("malloc");
		return -1;
	}

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
