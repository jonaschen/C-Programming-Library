obj-prob += nth.o count.o main.o
obj-slist += slist.o

P := list_problems

OUT_DIR = ../../output
INCLUDE = ../../includes
CFLAGS := -g
CC := gcc

.PHONY: all
all: $(obj-prob) $(P)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ -c $<

$(P): $(obj-prob)
	$(CC) $(CFLAGS) -o $@ $? $(OUT_DIR)/$(obj-slist)
	mv $@ $(OUT_DIR)


.PHONY: clean
clean:
	rm *.o
