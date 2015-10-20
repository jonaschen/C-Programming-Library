CFLAGS := -Wall -I../includes
#LIST_INCS := slist.h
LIST_INCS := ../incluses/slist.h
LIST_SRCS := slist.c
LIST_DEPS := $(LIST_INCS) $(LIST_SRCS)
PROB_INCS := prob.h
PROB_SRCS := main.c count.c
PROB_DEPS := $(PROB_INCS) $(PROB_SRCS)
P := list_example

.PHONY: all
all: $(PROB_DEPS) $(LIST_DEPS) 
	gcc $(CFLAGS) $(PROB_SRCS) $(LIST_SRCS) -o $(P)

.PHONY: clean
clean:
	rm -f $(P)
