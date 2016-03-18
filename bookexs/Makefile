DIR_TCPL := TCPL
DIR_SS := SS

.PHONY: all
all: obj-tcpl obj-ss

obj-tcpl:
	$(MAKE) -C $(DIR_TCPL)

obj-ss:
	$(MAKE) -C $(DIR_SS)

.PHONY: clean
clean:
	$(MAKE) -C $(DIR_TCPL) clean
	$(MAKE) -C $(DIR_SS) clean
