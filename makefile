OUT		   = 	../bin/

GCC64 	   = 	x86_64-elf-gcc
GCC		   =    i686-elf-gcc
NASM 	   = 	nasm
MAKE	   = 	make
KERNEL	   =	kernel/
LIMINE	   =    ../limine/
IMG		   =	../img/

GAS 	   = 	i686-elf-as
GAS64 	   = 	x86_64-elf-as

IMGNAME-x86 = nulliumV100-x86

make:
	make clean
	make mboot2-i686

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

	cp -v $(KERNEL)archspecific/i686/limine.conf $(OUT)$(KERNEL)limine-ISO/boot/limine.conf
	


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

	make bin-i686
	make mboot2-bin
	make inst-limineISO

bin-i686:
	$(GCC) -c $(KERNEL)archspecific/x86_general/stage2-x86.c -o $(OUT)$(KERNEL)stage2.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC
	
	$(GCC) -c $(KERNEL)krnlBitmaps.c -o $(OUT)$(KERNEL)krnlBitmaps.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC
	$(GCC) -c $(KERNEL)graphics.c -o $(OUT)$(KERNEL)graphics.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC
	
	$(GCC) -c $(KERNEL)utils.c -o $(OUT)$(KERNEL)utils.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC
	
	$(GCC) -c $(KERNEL)utils-x86.c -o $(OUT)$(KERNEL)utils86.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC
	

	

mboot2-bin:
	
	$(NASM) -felf32 $(KERNEL)archspecific/i686/entry.s -o $(OUT)$(KERNEL)entry.o
	$(GCC) -c $(KERNEL)archspecific/i686/stage1.c -o $(OUT)$(KERNEL)stage1.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC
	

	$(GCC) -T $(KERNEL)archspecific/i686/linker.ld -o $(OUT)nullium.bin -ffreestanding -O2 -nostdlib\
		$(OUT)$(KERNEL)entry.o\
		$(OUT)$(KERNEL)stage1.o\
		$(OUT)$(KERNEL)stage2.o\
		$(OUT)$(KERNEL)krnlBitmaps.o\
		$(OUT)$(KERNEL)graphics.o\
		$(OUT)$(KERNEL)utils.o\
		$(OUT)$(KERNEL)utils86.o\
	 -lgcc -fPIC

	