SRC_DIR := ./src
PROB_DIR := ./exercise


P = bst_exercise

.PHONY: all
all: obj-src obj-prob


obj-src:
	$(MAKE) -C $(SRC_DIR)

obj-prob:
	$(MAKE) P=$(P) -C $(PROB_DIR)




.PHONY: clean
clean:
	rm -f $(PATH_OUTPUT)/$(P)
	$(MAKE) -C $(PROB_DIR) clean
