LD = ld
CC = gcc

CFLAGS = -ffreestanding -fshort-wchar
LDFLAGS = -T link.ld -static -Bsymbolic -nostdlib

SRCDIR := src
OBJDIR := bin

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.c)          
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
DIRS = $(wildcard $(SRCDIR)/*)
stage2: $(OBJS)
	$(LD) $(LDFLAGS) -o bin/stage2.bin $(OBJS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@
