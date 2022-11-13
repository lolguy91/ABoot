LD = ld
CC = gcc
ASM = nasm

CFLAGS = -ffreestanding -fshort-wchar
ASMFLAGS = -f bin
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
	$(LD) $(LDFLAGS) -o bin/kernel.bin $(OBJS)

$(ASMOBJS):${ASMSRC}
	$(ASM) $(ASMFLAGS) $^ -o $@
$(COBJS): ${CSRC}
	$(CC) $(CFLAGS) -c $^ -o $@
