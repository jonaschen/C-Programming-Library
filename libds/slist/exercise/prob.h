#ifndef __PROB_H
#define __PROB_H

int prob_count(struct slist *list, int data);
int prob_get_nth(struct slist *list, int nth, int *data);
int prob_ins_nth(struct slist *list, int nth, const void *data);
void sorted_insert(struct slist *sorted, struct slist_node *new);
void insert_sort(struct slist *list);
int sorted_merge(struct slist *a, struct slist *b);
void merge_sort(struct slist *list);
void front_back_split(struct slist *list, struct slist *back);
void remove_duplicates(struct slist *list);

void alternating_split(struct slist *source, struct slist *a, struct slist *b);
struct slist *shuffle_merge(struct slist *a, struct slist *b);

void move_node(struct slist *dest, struct slist *src);
void reverse_list(struct slist *list);

#endif
