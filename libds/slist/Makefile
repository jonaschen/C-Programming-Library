PATH_SRC := ./src
PATH_EXER := ./exercise
PATH_OUTPUT := ../output
INCLUDE := ../includes

CFLAGS := -Wall -g
CC := gcc

LIST_INCS := $(INCLUDE)/slist.h
LIST_SRCS := $(PATH_SRC)/slist.c
LIST_DEPS := $(LIST_INCS) $(LIST_SRCS)

PROB_INCS := $(PATH_EXER)/prob.h
PROB_SRCS := $(PATH_EXER)/main.c $(PATH_EXER)/count.c
PROB_DEPS := $(PROB_INCS) $(PROB_SRCS)

P := list_exercise

.PHONY: all
all: $(PROB_DEPS) $(LIST_DEPS) 
	gcc $(CFLAGS) -I$(INCLUDE) $(PROB_SRCS) $(LIST_SRCS) -o $(PATH_OUTPUT)/$(P)
	gcc $(CFLAGS) -I$(INCLUDE) -o $(PATH_OUTPUT)/slist.o -c $(LIST_SRCS)

.PHONY: clean
clean:
	rm -f $(PATH_OUTPUT)/$(P)
