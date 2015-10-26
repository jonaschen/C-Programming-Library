PATH_OUTPUT := output
PATH_SRC := src
CFLAGS := -Wall
SRC := grep.c getwords.c calculator.c
CC := gcc
INCLUDE = ../libds/includes
LDADD = -L../libds/output -lds

OBJ = $(addsuffix .o, $(basename $(SRC)))

.PHONY: all
all: objdir $(OBJ) grep calculator

objdir:
	mkdir -p $(PATH_OUTPUT)

%.o:	$(PATH_SRC)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $(PATH_OUTPUT)/$@ -c $<

grep: $(PATH_SRC)/grep.c
	$(CC) $(CFLAGS) -o $(PATH_OUTPUT)/$@ $(PATH_OUTPUT)/$@.o

calculator:	$(PATH_SRC)/calculator.c
	$(CC) $(CFLAGS) -o $(PATH_OUTPUT)/$@ $(PATH_OUTPUT)/$@.o $(LDADD)

.PHONY: clean
clean:
	rm -rf $(PATH_OUTPUT)
