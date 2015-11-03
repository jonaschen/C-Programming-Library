#ifndef __PROB_H
#define __PROB_H

int prob_count(struct slist *list, int data);
int prob_get_nth(struct slist *list, int nth, int *data);
int prob_ins_nth(struct slist *list, int nth, const void *data);
void sorted_insert(struct slist *sorted, struct slist_node *new);
void insert_sort(struct slist *list);
void front_back_split(struct slist *list, struct slist *back);

#endif
