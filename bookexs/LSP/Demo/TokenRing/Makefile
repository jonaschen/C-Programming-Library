OUT_DIR = ../../output
INCLUDE = ../../include 
CFLAGS := -g
CC := gcc

P_TRING = tk_ring
obj-tr = ring.o


PROGRAMS = $(P_TRING)
OBJS = $(obj-tr)

.PHONY: all
all: $(PROGRAMS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ -c $<

$(P_TRING): $(obj-tr)
	$(CC) $(CFLAGS) -o $@ $? -I$(INCLUDE)
	mv $@ $(OUT_DIR)

.PHONY: clean
clean:
	rm -f $(PROGRAMS) $(OBJS)
