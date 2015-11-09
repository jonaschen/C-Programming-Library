OUT_DIR := ../output
LIBDS_DIR := ../../libds
INCLUDE = $(LIBDS_DIR)/includes
LDADD = -L$(LIBDS_DIR)/output -lds -lm
CFLAGS := -g

# program grep
P_GREP = grep
SRC_GREP = grep.c
SRCS += $(SRC_GREP)

# program calculator
P_CALCULATOR = calculator
SRC_CALCULATOR = calculator.c
SRCS += $(SRC_CALCULATOR)

# program wc
P_WC = wc
SRC_WC = wc.c
SRCS += $(SRC_WC)

# program lower & upper

OBJS = $(addsuffix .o, $(basename $(SRCS)))

.PHONY: all
all: $(OBJS) $(P_GREP) $(P_CALCULATOR) $(P_WC)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $(OUT_DIR)/$@ -c $<

$(P_GREP): $(SRC_GREP)
	$(CC) $(CFLAGS) -o $(OUT_DIR)/$@ $(OUT_DIR)/$@.o $(LDADD)

$(P_CALCULATOR): $(SRC_CALCULATOR)
	$(CC) $(CFLAGS) -o $(OUT_DIR)/$@ $(OUT_DIR)/$@.o $(LDADD)

$(P_WC): $(SRC_WC)
	$(CC) $(CFLAGS) -o $(OUT_DIR)/$@ $(OUT_DIR)/$@.o $(OUT_DIR)/getwords.o $(LDADD)
