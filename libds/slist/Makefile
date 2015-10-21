PATH_SRC := ./src
PATH_EXER := ./exercise
PATH_OUTPUT := ./output
PATH_INCL := ../includes

CFLAGS := -Wall -I$(PATH_INCL)

LIST_INCS := $(PATH_INCL)/slist.h
LIST_SRCS := $(PATH_SRC)/slist.c
LIST_DEPS := $(LIST_INCS) $(LIST_SRCS)

PROB_INCS := $(PATH_EXER)/prob.h
PROB_SRCS := $(PATH_EXER)/main.c $(PATH_EXER)/count.c
PROB_DEPS := $(PROB_INCS) $(PROB_SRCS)

P := list_exercise

.PHONY: all
all: $(PROB_DEPS) $(LIST_DEPS)
	gcc $(CFLAGS) $(PROB_SRCS) $(LIST_SRCS) -o $(PATH_OUTPUT)/$(P)

.PHONY: clean
clean:
	rm -f $(PATH_OUTPUT)/$(P)
