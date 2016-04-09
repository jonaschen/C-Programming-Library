obj-prob += main.o traverse.o build.o

P := bitree_problems

OUT_DIR = ../../output
INCLUDE = ../../includes
CFLAGS := -g
CC := gcc
LDADD := $(OUT_DIR)/bitree.o $(OUT_DIR)/stack.o $(OUT_DIR)/queue.o

.PHONY: all
all: $(obj-prob) $(P)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ -c $<

$(P): $(obj-prob)
	$(CC) $(CFLAGS) -o $@ $? $(LDADD)
	mv $@ $(OUT_DIR)


.PHONY: clean
clean:
	rm -f *.o
