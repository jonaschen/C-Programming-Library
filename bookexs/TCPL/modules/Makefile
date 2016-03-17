OUT_DIR := ../output
LIBDS_DIR := ../../libds
INCLUDE = $(LIBDS_DIR)/includes
CFLAGS := -g


SRCS += getwords.c

OBJS = $(addsuffix .o, $(basename $(SRCS)))

.PHONY: all
all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $(OUT_DIR)/$@ -c $<
