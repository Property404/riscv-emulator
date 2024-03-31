TRIPLE=riscv64-linux-gnu-
LD=$(TRIPLE)ld
AS=$(TRIPLE)as
OBJCOPY=$(TRIPLE)objcopy
ASFLAGS=-march=rv64imac
LDFLAGS=-T linker.ld

EXECUTABLE=weenie
OBJECTS=start.o

all: $(OBJECTS) 
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE).elf
	$(OBJCOPY) -O binary $(EXECUTABLE).elf $(EXECUTABLE).bin
%.o: %.S
	$(AS) $(ASFLAGS) -c $(@:.o=.S) -o $@
.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE).elf $(EXECUTABLE).bin

