
P_COPY = copy
obj-copy = copy.o read_r.o

OUT_DIR = ../output
INCLUDE = ../include 
CFLAGS := -g
CC := gcc

PROGRAMS = $(P_COPY)
OBJS = $(obj-copy)

.PHONY: all
all: $(PROGRAMS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ -c $<

$(P_COPY): $(obj-copy)
	$(CC) $(CFLAGS) -o $@ $? -I$(INCLUDE)
	cp $@ $(OUT_DIR)

.PHONY: clean
clean:
	rm -f $(PROGRAMS) $(OBJS)
