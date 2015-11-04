obj-prob += 
obj-sq +=

P := sq_problems

OUT_DIR = ../../output
INCLUDE = ../../includes
CFLAGS := -g
CC := gcc

.PHONY: all
all: $(obj-prob) $(P)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ -c $<

$(P): $(obj-prob)
	$(CC) $(CFLAGS) -o $@ $? $(OUT_DIR)/$(obj-sq)
	mv $@ $(OUT_DIR)


.PHONY: clean
clean:
	rm -f *.o
