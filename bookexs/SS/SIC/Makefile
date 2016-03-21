DIR_ASM := Assembler
#DIR_SS := SS

OUTPUT_DIR := ./output

.PHONY: all
all: obj-asm

obj-asm:
	mkdir -p $(OUTPUT_DIR)
	$(MAKE) -C $(DIR_ASM)


.PHONY: clean
clean:
	$(MAKE) -C $(DIR_ASM) clean
