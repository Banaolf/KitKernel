# --- Compiler & Tools ---
CXX = x86_64-elf-g++
AS = nasm
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
LDFLAGS  = -n -T linker.ld

# --- Rules ---
all: kernel.iso

# 1. Link the kernel binary
kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# 2. Build the ISO
kernel.iso: kernel.bin
	mkdir -p $(ISODIR)/boot/grub
	cp kernel.bin $(ISODIR)/boot/kernel.bin
	cp grub/grub.cfg $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso $(ISODIR)

# Rule for C++ files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for Assembly files (suffixed with _asm to avoid collisions)
$(OBJDIR)/%_asm.o: $(SRCDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) $(ISODIR) kernel.bin kernel.iso

.PHONY: all clean