
TARGET:= Y86_machine
SOURCES:= main.c Y86_model.c Y86_system.c
INCLUDE:= Y86_common.h
LDFLAG:=-lpthread

all: $(SOURCES) $(INCLUDE)
	gcc $(SOURCES) -o $(TARGET) $(LDFLAG)

clean:
	rm -f $(TARGET)
