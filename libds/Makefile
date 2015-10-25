PATH_OUTPUT := output
DIR_SLIST := slist
DIR_DLIST := dlist
DIR_STACK_QUEUE := stack-queues
DIR_BITREE := bitree

.PHONY: all
all: objdir obj-slist obj-dlist obj-sq obj-bitree

objdir:
	mkdir -p $(PATH_OUTPUT)

obj-slist:
	$(MAKE) -C $(DIR_SLIST)

obj-dlist:
	$(MAKE) -C $(DIR_DLIST)

obj-sq:
	$(MAKE) -C $(DIR_STACK_QUEUE)

obj-bitree:
	$(MAKE) -C $(DIR_BITREE)


.PHONY: clean
clean:
	rm -rf $(PATH_OUTPUT)
