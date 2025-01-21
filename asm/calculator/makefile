ASM=nasm
ASMFLAGS=-f elf64
CC=gcc
CCFLAGS=-no-pie

SOURCE=calc.asm
OBJECT=calc.o
EXECUTABLE=calc

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECT)
	$(CC) $(CCFLAGS) $(OBJECT) -o $(EXECUTABLE)

$(OBJECT): $(SOURCE)
	$(ASM) $(ASMFLAGS) $(SOURCE) -o $(OBJECT)

clean:
	rm -rf $(OBJECT) $(EXECUTABLE)

