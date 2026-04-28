# --- Compiler & Tools ---
CXX = x86_64-elf-g++
AS = nasm
EMU = qemu-system-x86_64
LD = x86_64-elf-ld

# --- Project Structure ---
INCDIR = include
SRCDIR = kernel
OBJDIR = build
ISODIR = iso

# --- Files ---
# Find all .cpp and .s files anywhere in the kernel directory
CPPSRC = $(shell find $(SRCDIR) -name "*.cpp")
ASSRC  = $(shell find $(SRCDIR) -name "*.s")

# Map source files to the build directory
CPPOBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(CPPSRC))
ASOBJS  = $(patsubst $(SRCDIR)/%.s, $(OBJDIR)/%_asm.o, $(ASSRC))
OBJS    = $(CPPOBJS) $(ASOBJS)

# --- Flags ---
CXXFLAGS = -I$(INCDIR) -m64 -ffreestanding -fno-exceptions \
           -fno-rtti -nostdlib -Wall -Wextra -mno-red-zone \
           -fno-stack-protector

ASFLAGS  = -f elf64
LDFLAGS  = -n -T linker.ld --no-warn-rwx-segments
TARGET = kernel.iso
# --- Rules ---
all: $(TARGET)

# Link the kernel binary
kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Build the ISO
$(TARGET): kernel.bin
	mkdir -p $(ISODIR)/boot/grub
	cp kernel.bin $(ISODIR)/boot/kernel.bin
	grub-mkrescue -o $(TARGET) $(ISODIR)

# Rule for C++ files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for Assembly files
$(OBJDIR)/%_asm.o: $(SRCDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) $(ISODIR)/boot/kernel.bin kernel.bin $(TARGET)

run:
	$(EMU) -cdrom $(TARGET) -m 256M -serial file:serial.log
debug:
	$(EMU) -cdrom $(TARGET) -m 256M -d int,cpu_reset -no-reboot -serial file:serial.log
.PHONY: all clean