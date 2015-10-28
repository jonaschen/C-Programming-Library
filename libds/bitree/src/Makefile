obj-bitree += bitree.o

OUT_DIR = ../../output
INCLUDE = ../../includes
CFLAGS := -g

.PHONY: all
all: $(obj-bitree)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $(OUT_DIR)/$@ -c $<
