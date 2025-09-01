OUT		   = 	../bin/
KERNEL     =    popkrnl/
LIMINE     =	../limine/
IMG        =    ../img/

IMGNAME-x86 = nulliumV100-x86


GCC_86_SETTINGS = -std=gnu99 -ffreestanding -O2 -O6 -Wall -Wextra -fPIC -I kernel/include -g


make:
	make clean

	make mboot2-i686

	qemu-system-i386 --cdrom $(IMG)$(IMGNAME-x86).iso -m 4G -M accel=tcg,smm=off -d int -no-reboot -no-shutdown
	
clean:
	#rm -rf $(OUT)/

inst-limineISO:
	mkdir -p $(OUT)$(KERNEL)limine-ISO/
	mkdir -p $(OUT)$(KERNEL)limine-ISO/
	mkdir -p $(OUT)$(KERNEL)limine-ISO/nullium/

	mkdir -p $(OUT)$(KERNEL)limine-ISO/efi/
	mkdir -p $(OUT)$(KERNEL)limine-ISO/efi/boot/

	mkdir -p $(OUT)$(KERNEL)limine-ISO/boot/

	cp -v $(LIMINE)BOOTX64.EFI  $(OUT)$(KERNEL)limine-ISO/efi/boot/
	cp -v $(LIMINE)BOOTIA32.EFI $(OUT)$(KERNEL)limine-ISO/efi/boot/

	cp -v $(LIMINE)limine-bios-cd.bin $(OUT)$(KERNEL)limine-ISO/boot/biosCD.bin
	cp -v $(LIMINE)limine-uefi-cd.bin $(OUT)$(KERNEL)limine-ISO/boot/uefiCD.bin
	cp -v $(LIMINE)limine-uefi-cd.bin $(OUT)$(KERNEL)limine-ISO/boot/limine-bios.sys
	cp -v $(LIMINE)limine-bios.sys $(OUT)$(KERNEL)limine-ISO/boot/

	cp -v $(OUT)popkrnl.bin $(OUT)$(KERNEL)limine-ISO/boot/popkrnl

	cp -v $(KERNEL)boot/stage1/limine.conf $(OUT)$(KERNEL)limine-ISO/boot/limine.conf
	


	xorriso -as mkisofs -R -r -J -b "/boot/biosCD.bin" \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot "/boot/uefiCD.bin" \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        "$(OUT)$(KERNEL)limine-ISO/" -o $(IMG)$(IMGNAME-x86).iso

		$(LIMINE)limine bios-install $(IMG)$(IMGNAME-x86).iso

inst-limineIMG:


mboot2-i686:
	mkdir -p $(OUT)
	mkdir -p $(OUT)$(KERNEL)

	make -C popkrnl mboot2-i686
	make inst-limineISO