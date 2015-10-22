PATH_SRC := src
PATH_OUTPUT := ../output
SRC = stack.c queue.c
CC := gcc
CFLAGS := -Wall
INCLUDE := ../includes
OBJ = $(addsuffix .o, $(basename $(SRC)))

.PHONY: all
all: $(OBJ)

%.o:	$(PATH_SRC)/%.c
	$(CC) $(CFLAGS) -o $(PATH_OUTPUT)/$@ -c $< -I$(INCLUDE)
