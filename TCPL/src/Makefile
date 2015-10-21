PATH_OUTPUT := ../output
CFLAGS := -Wall
SRC := grep.c getwords.c
CC := gcc

OBJ = $(addsuffix .o, $(basename $(SRC)))

.PHONY: all
all: $(OBJ) grep


%.o:	%.c
	$(CC) $(CFLAGS) -o $(PATH_OUTPUT)/$@ -c $<

grep: grep.c
	$(CC) $(CFLAGS) -o $(PATH_OUTPUT)/$@ $(PATH_OUTPUT)/$@.o

