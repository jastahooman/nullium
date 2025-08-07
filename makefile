OUT		   = 	../bin/

GCC 	   = 	x86_64-elf-gcc
NASM 	   = 	nasm
MAKE	   = 	make
KERNEL	   =	kernel/
LIMINE	   =    ../limine/
IMG		   =	../img/

IMGNAME-x86_64-LIMINE = nulliumV100-x64-lim

make:
	make clean
	make limine-x64

clean:
	rm -rf $(OUT)

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

	cp -v $(OUT)nullium.bin $(OUT)$(KERNEL)limine-ISO/nullium/os.nlp

	cp -v $(KERNEL)/archspecific/x86_64/limine/limine.conf $(OUT)$(KERNEL)limine-ISO/boot/limine.conf
	


	xorriso -as mkisofs -R -r -J -b "/boot/biosCD.bin" \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot "/boot/uefiCD.bin" \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        "$(OUT)$(KERNEL)limine-ISO/" -o $(IMG)$(IMGNAME-x86_64-LIMINE).iso

		$(LIMINE)limine bios-install $(IMG)$(IMGNAME-x86_64-LIMINE).iso

inst-limineIMG:

limine-x64:
	make binary
	make limine-bin
	make inst-limineISO

limine-bin:
	$(GCC) -c $(KERNEL)archspecific/x86_64/limine/entry.c -o $(OUT)$(KERNEL)entry.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC

	$(GCC) -T $(KERNEL)/archspecific/x86_64/limine/linker.ld -o $(OUT)nullium.bin -ffreestanding -O2 -nostdlib\
		$(OUT)$(KERNEL)entry.o\
		$(OUT)$(KERNEL)stage2.o\
		$(OUT)$(KERNEL)utils.o\
		$(OUT)$(KERNEL)graphics.o\
		$(OUT)$(KERNEL)krnlBitmaps.o\
		$(OUT)$(KERNEL)gdt.o\
		$(OUT)$(KERNEL)gdtASM.o\
		$(OUT)$(KERNEL)idt.o\
		$(OUT)$(KERNEL)idtASM.o\
		$(OUT)$(KERNEL)utils-x86.o\
		$(OUT)$(KERNEL)pic.o\
	 -lgcc -fPIC
binary:
	mkdir -p $(OUT)
	mkdir -p $(OUT)$(KERNEL)

	$(GCC) -c $(KERNEL)archspecific/x86_general/stage2-x86.c -o $(OUT)$(KERNEL)stage2.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC
	$(GCC) -c $(KERNEL)utils.c -o $(OUT)$(KERNEL)utils.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC
	$(GCC) -c $(KERNEL)utils-x86.c -o $(OUT)$(KERNEL)utils-x86.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC

	$(GCC) -c $(KERNEL)graphics.c -o $(OUT)$(KERNEL)graphics.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC

	$(GCC) -c $(KERNEL)archspecific/x86_64/gdt.c -o $(OUT)$(KERNEL)gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC

	$(GCC) -c $(KERNEL)archspecific/x86_64/pic.c -o $(OUT)$(KERNEL)pic.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC


	$(GCC) -c $(KERNEL)krnlBitmaps.c -o $(OUT)$(KERNEL)krnlBitmaps.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC
	$(GCC) -c $(KERNEL)archspecific/x86_64/idt.c -o $(OUT)$(KERNEL)idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC

	$(NASM) -felf64 $(KERNEL)archspecific/x86_64/gdt.s -o $(OUT)$(KERNEL)gdtASM.o
	$(NASM) -felf64 $(KERNEL)archspecific/x86_64/idt.s -o $(OUT)$(KERNEL)idtASM.o