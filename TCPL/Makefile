OUT_DIR := output
MODULES := modules
EXAMPLES := examples


.PHONY: all
all: $(OUT_DIR) module-objs example-programs

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

module-objs:
	$(MAKE) -C $(MODULES)

example-programs:
	$(MAKE) -C $(EXAMPLES)




.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
