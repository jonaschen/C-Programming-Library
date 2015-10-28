OUTPUT := output
DIR_SLIST := slist
DIR_DLIST := dlist
DIR_STACK_QUEUE := stack-queues
DIR_BITREE := bitree

.PHONY: all
all: objdir obj-slist obj-dlist obj-sq obj-bitree libds.a

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

libds.a: $(OUTPUT)/slist.o $(OUTPUT)/dlist.o $(OUTPUT)/stack.o $(OUTPUT)/queue.o $(OUTPUT)/bitree.o
	ar -rv $(OUTPUT)/libds.a  $?

.PHONY: clean
clean:
	rm -rf $(OUTPUT)/*
