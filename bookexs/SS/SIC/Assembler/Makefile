P := scan

INCLUDES := ../../../../libds/includes
LINK := ../../../../libds/output
OUTPUT := ../output

CFLAGS += -I$(INCLUDES)
LDFLAGS += -lds -L$(LINK)

SRCS := scan.c optbl.c directive.c

all:
	gcc -o $(P) $(SRCS) $(CFLAGS) $(LDFLAGS)
	mv $(P) $(OUTPUT)/$(P)

clean:
	rm -f $(P)
