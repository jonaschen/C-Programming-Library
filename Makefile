DIR_LIBDS := libds
DIR_BOOKEX := bookexs

.PHONY: all
all: obj-libds obj-bookex

obj-libds:
	$(MAKE) -C $(DIR_LIBDS)

obj-bookex:
	$(MAKE) -C $(DIR_BOOKEX)

.PHONY: clean
clean:
	$(MAKE) -C $(DIR_LIBDS) clean
	$(MAKE) -C $(DIR_BOOKEX) clean
