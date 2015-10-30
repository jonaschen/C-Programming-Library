#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "slist.h"
#include "chtbl.h"

#define MAX_WORD_LEN	64

int getwords(char *str, size_t len, char ***pwords);

struct wc_t {
	char *word;
	int cnt;
};

int word_hash(const int *key)
{
	struct wc_t *data = (struct wc_t *) key;
	char *word = data->word; 
	char *ptr = word;
	int hash_value = 0;

	while (*ptr != '\0') {
		hash_value += *ptr++;
	}

	return (hash_value * hash_value);
}

int word_match(const void *key1, const void *key2)
{
	struct wc_t *data1 = (struct wc_t *) key1;
	struct wc_t *data2 = (struct wc_t *) key2;

	if (strncmp(data1->word, data2->word, MAX_WORD_LEN))
		return 0;

	return 1;
}

static struct chtbl_t words_tbl, *wc_table = &words_tbl;
void update_word_table(int num_words, char **words)
{
	int i;
	struct wc_t *data, *ptr;

	for (i = 0; i < num_words; i++) {
		//printf("words[%d]\t%s\n", i, words[i]);
		ptr = data = (struct wc_t *) malloc(sizeof(struct wc_t));
		data->word = words[i];
		data->cnt = 1;
		if (chtbl_lookup(wc_table, (void **) &data) == 0) {
			data->cnt++;
			free(ptr);
		} else {
			chtbl_insert(wc_table, (const void *) data);
		}
	}
}

int wrod_table_list(struct wc_t ***pwords)
{
	struct wc_t **words, *data;
	struct slist_node *elem;
	int i, cnt = 0;

	if (chtbl_size(wc_table) == 0)
		return 0;

	words = (struct wc_t **) malloc(chtbl_size(wc_table) * sizeof(struct wc_t*));


	for (i = 0; i < wc_table->buckets; i++) {
		for (elem = slist_head(&wc_table->table[i]); elem; elem = slist_next(elem)) {
			data = (struct wc_t *) slist_data(elem);
			words[cnt++] = data;
		}
	}

	*pwords = words;

	return cnt;
}

int wc_sorting_cmp(const void *key1, const void *key2)
{
	struct wc_t *data1 = *(struct wc_t **) key1;
	struct wc_t *data2 = *(struct wc_t **) key2;

	return (data1->cnt - data2->cnt);
}

void do_wc_sorting(void)
{
	int i, cnt;
	struct wc_t **wc_nodes;

	cnt = wrod_table_list(&wc_nodes);

	for (i = 0; i < cnt; i++)
		printf("word:%s, cnt:%d\n", wc_nodes[i]->word, wc_nodes[i]->cnt);

	printf("\n\n\nsorting...\n");
	qsort((void *) wc_nodes, cnt, sizeof(struct wc_t *), wc_sorting_cmp);


	for (i = 0; i < cnt; i++)
		printf("word:%s, cnt:%d\n", wc_nodes[i]->word, wc_nodes[i]->cnt);
}

#define WORD_SLOTS	512
int main(int argc, char *argv[])
{
	int cnt, i;
	char **words;
	char *str, *lineptr = NULL;
	size_t len;
	ssize_t bytes;
	FILE *input = stdin;

	if (argc > 1) {
		input = fopen(argv[1], "r");
		if (!input) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}
	}

	chtbl_init(wc_table, WORD_SLOTS, word_hash, word_match, NULL);

	while ((bytes = getline(&lineptr, &len, input)) > 0) {
		if ((cnt = getwords(lineptr, bytes, &words)) > 0) {
			update_word_table(cnt, words);
		}
	}

	do_wc_sorting();

	exit(EXIT_SUCCESS);
}
