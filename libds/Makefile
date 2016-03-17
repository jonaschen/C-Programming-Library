OUTPUT = output
DIR_SLIST = slist
DIR_DLIST = dlist
DIR_STACK_QUEUE = stack-queues
DIR_BITREE = bitree
DIR_CHTBL = chtbl

OBJS = obj-slist obj-dlist obj-sq obj-bitree obj-chtbl

.PHONY: all
all: objdir $(OBJS) libds.a

objdir:
	mkdir -p $(OUTPUT)

obj-slist:
	$(MAKE) -C $(DIR_SLIST)

obj-dlist:
	$(MAKE) -C $(DIR_DLIST)

obj-sq:
	$(MAKE) -C $(DIR_STACK_QUEUE)

obj-bitree:
	$(MAKE) -C $(DIR_BITREE)

obj-chtbl:
	$(MAKE) -C $(DIR_CHTBL)


libds.a: $(OUTPUT)/slist.o $(OUTPUT)/dlist.o $(OUTPUT)/stack.o $(OUTPUT)/queue.o $(OUTPUT)/bitree.o $(OUTPUT)/chtbl.o
	ar -rv $(OUTPUT)/libds.a  $?

.PHONY: clean
clean:
	rm -rf $(OUTPUT)/*
	$(MAKE) -C $(DIR_SLIST)	clean
	$(MAKE) -C $(DIR_DLIST) clean
	$(MAKE) -C $(DIR_STACK_QUEUE) clean
	$(MAKE) -C $(DIR_BITREE) clean
	$(MAKE) -C $(DIR_CHTBL) clean
