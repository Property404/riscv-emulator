TRIPLE=riscv64-linux-gnu-
CC=$(TRIPLE)gcc
LD=$(TRIPLE)ld
AS=$(TRIPLE)as
OBJCOPY=$(TRIPLE)objcopy
CCFLAGS=-march=rv64i -mabi="lp64" -O0 -ffreestanding
ASFLAGS=-march=rv64i -mabi="lp64"
LDFLAGS=-T linker.ld

EXECUTABLE=weenie
OBJECTS=start.o main.o

all: $(OBJECTS) 
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE).elf
	$(OBJCOPY) -O binary $(EXECUTABLE).elf $(EXECUTABLE).bin
%.o: %.S
	$(AS) $(ASFLAGS) -c $(@:.o=.S) -o $@
%.o: %.c
	$(CC) $(CCFLAGS) -c $(@:.o=.c) -o $@
.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE).elf $(EXECUTABLE).bin

