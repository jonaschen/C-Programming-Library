
TARGET:= Y86_machine
SOURCES:= main.c y86_cpu_core.c y86_system.c
INCLUDE:= y86_cpu_core.h y86_system.h
LDFLAG:=-lpthread

all: $(SOURCES) $(INCLUDE)
	gcc $(SOURCES) -o $(TARGET) $(LDFLAG)

clean:
	rm -f $(TARGET)
