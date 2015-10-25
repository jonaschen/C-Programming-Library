PATH_SRC := src
PATH_OUTPUT := ../output
PATH_EXER := exercise
SRC = bitree.c
CC := gcc
CFLAGS := -Wall -g
INCLUDE := ../includes
OBJ = $(addsuffix .o, $(basename $(SRC)))

.PHONY: all
all: $(OBJ) bitree_exercise

%.o:	$(PATH_SRC)/%.c
	$(CC) $(CFLAGS) -o $(PATH_OUTPUT)/$@ -c $< -I$(INCLUDE)

DEP_OBJ := $(PATH_OUTPUT)/bitree.o $(PATH_OUTPUT)/stack.o $(PATH_OUTPUT)/queue.o
bitree_exercise:	$(PATH_EXER)/main.c
	$(CC) -o $(PATH_OUTPUT)/$@.o $(CFLAGS) -c $< -I$(INCLUDE)
	$(CC) -o $(PATH_OUTPUT)/$@ $(PATH_OUTPUT)/$@.o $(DEP_OBJ)
