DIR_LIBDS := libds
DIR_TCPL := TCPL

.PHONY: all
all: obj-libds obj-TCPL

obj-libds:
	$(MAKE) -C $(DIR_LIBDS)

obj-TCPL:
	$(MAKE) -C $(DIR_TCPL)

.PHONY: clean
clean:
	$(MAKE) -C $(DIR_LIBDS) clean
	$(MAKE) -C $(DIR_TCPL) clean
