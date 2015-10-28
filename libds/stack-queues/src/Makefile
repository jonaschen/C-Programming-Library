obj-stack += stack.o
obj-queue += queue.o

OBJS = $(obj-stack) $(obj-queue)

OUT_DIR = ../../output
INCLUDE = ../../includes
CFLAGS := -g

.PHONY: all
all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $(OUT_DIR)/$@ -c $<

