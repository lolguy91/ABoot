LD = i386-elf-ld
CC = i386-elf-g++
ASM = nasm

CFLAGS = -ffreestanding -fshort-wchar
ASMFLAGS = -f elf32
LDFLAGS = -T link.ld -static -Bsymbolic -nostdlib

SRCDIR = src
OBJDIR = bin

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CSRC = $(call rwildcard,$(SRCDIR),*.c)
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)          

COBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CSRC))
ASMOBJS = $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%.o, $(ASMSRC))

OBJS = ${ASMOBJS} ${COBJS}

stage2: $(OBJS)
	$(LD) $(LDFLAGS) -o bin/stage2.bin $(filter-out bin/main.o,$(OBJS))


$(ASMOBJS):${ASMSRC}
	$(ASM) $(ASMFLAGS) $^ -o $@
$(COBJS): ${CSRC}
	$(CC) $(CFLAGS) -c $^ -o $@

