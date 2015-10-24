PATH_OUTPUT := output
PATH_SRC := src
CFLAGS := -Wall
SRC := grep.c getwords.c
CC := gcc


OBJ = $(addsuffix .o, $(basename $(SRC)))

.PHONY: all
all: objdir $(OBJ) grep

objdir:
	mkdir -p $(PATH_OUTPUT)

%.o:	$(PATH_SRC)/%.c
	$(CC) $(CFLAGS) -o $(PATH_OUTPUT)/$@ -c $<

grep: $(PATH_SRC)/grep.c
	$(CC) $(CFLAGS) -o $(PATH_OUTPUT)/$@ $(PATH_OUTPUT)/$@.o


.PHONY: clean
clean:
	rm -rf $(PATH_OUTPUT)
