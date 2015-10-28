obj-slist += slist.o

OUT_DIR = ../../output
INCLUDE = ../../includes
CFLAGS := -g

.PHONY: all
all: $(obj-slist)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $(OUT_DIR)/$@ -c $<

