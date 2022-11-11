OSNAME = CustomOS

GNUEFI = boot/uefi
BIOSBOOT = boot/bios/stage1
S2 = boot/bios/stage2# a fucking mess
OVMFDIR = OVMFbin


BUILDDIR = bin
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

bootloader:
	cd $(GNUEFI) && make bootloader && cd .. && cd .. && cd .. && cd ..
	cd $(BIOSBOOT) && make && cd .. && cd .. && cd .. && cd ..
	cd $(S2) && make stage2 && cd .. && cd .. 

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)

buildimg: bootloader
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	mkfs.fat -F 12 -n "NBOS" $(BUILDDIR)/$(OSNAME).img
	dd if=$(BIOSBOOT)/boot.img of=$(BUILDDIR)/$(OSNAME).img conv=notrunc
	
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(S2)/bin/stage2.bin ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img iso/boot/aboot.cfg ::

run: buildimg
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
runbios: buildimg
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64
