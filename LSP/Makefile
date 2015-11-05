
DIR_FILE_IO = File-IO
DIR_DEMO = Demo

.PHONY: all
all:
	$(MAKE) -C $(DIR_FILE_IO) all
	$(MAKE) -C $(DIR_DEMO) all

OUT_DIR = ./output

.PHONY: clean
clean:
	rm -f $(OUT_DIR)/*
	$(MAKE) -C $(DIR_FILE_IO) clean
	$(MAKE) -C $(DIR_DEMO) clean
