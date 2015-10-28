obj-dlist += dlist.o

OBJS := $(obj-dlist)

OUT_DIR = ../../output
INCLUDE = ../../includes
CFLAGS := -g

.PHONY: all
all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $(OUT_DIR)/$@ -c $<

