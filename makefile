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

	cp -v $(KERNEL)arch/i686/limine.conf $(OUT)$(KERNEL)limine-ISO/boot/limine.conf
	


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
	
	$(GCC) -c $(KERNEL)stage3/krnlBitmaps.c -o $(OUT)$(KERNEL)krnlBitmaps.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	$(GCC) -c $(KERNEL)drivers/graphics.c -o $(OUT)$(KERNEL)graphics.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	
	$(GCC) -c $(KERNEL)utils/utils.c -o $(OUT)$(KERNEL)utils.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	$(GCC) -c $(KERNEL)stage3/winmgr.c -o $(OUT)$(KERNEL)winmgr.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	
	$(GCC) -c $(KERNEL)stage3/stdio.c -o $(OUT)$(KERNEL)stdio.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	
	$(GCC) -c $(KERNEL)drivers/timer.c -o $(OUT)$(KERNEL)timer.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	$(GCC) -c $(KERNEL)drivers/memmgr.c -o $(OUT)$(KERNEL)memmgr.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include



	# Architecture specific:

	$(GCC) -c $(KERNEL)utils/utils-x86.c -o $(OUT)$(KERNEL)utils86.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	$(GCC) -c $(KERNEL)utils/utils-x86.c -o $(OUT)$(KERNEL)utils86.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	$(GCC) -c $(KERNEL)arch/x86_general/stage2-x86.c -o $(OUT)$(KERNEL)stage2.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	
	$(GCC) -c $(KERNEL)arch/i686/gdt.c -o $(OUT)$(KERNEL)gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	$(NASM) -felf32 $(KERNEL)arch/i686/gdt.s -o $(OUT)$(KERNEL)gdtASM.o
	
	$(GCC) -c $(KERNEL)arch/i686/idt.c -o $(OUT)$(KERNEL)idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	$(NASM) -felf32 $(KERNEL)arch/i686/idt.s -o $(OUT)$(KERNEL)idtASM.o

	$(GCC) -c $(KERNEL)drivers/timer-x86.c -o $(OUT)$(KERNEL)timer86.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	
	$(GCC) -c $(KERNEL)stage3/stage3.c -o $(OUT)$(KERNEL)stage3.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	
	$(GCC) -c $(KERNEL)drivers/PS2-x86.c -o $(OUT)$(KERNEL)PS2-x86.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	

	$(GCC) -c $(KERNEL)arch/i686/paging.c -o $(OUT)$(KERNEL)paging.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	$(NASM) -felf32 $(KERNEL)arch/i686/paging.s -o $(OUT)$(KERNEL)pagingASM.o
	


mboot2-bin:
	
	$(NASM) -felf32 $(KERNEL)arch/i686/entry.s -o $(OUT)$(KERNEL)entry.o 
	$(GCC) -c $(KERNEL)arch/i686/stage1.c -o $(OUT)$(KERNEL)stage1.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fPIC -I kernel/include
	

	$(GCC) -T $(KERNEL)arch/i686/linker.ld -o $(OUT)nullium.bin -ffreestanding -O2 -nostdlib\
		$(OUT)$(KERNEL)entry.o\
		$(OUT)$(KERNEL)stage1.o\
		$(OUT)$(KERNEL)stage2.o\
		$(OUT)$(KERNEL)krnlBitmaps.o\
		$(OUT)$(KERNEL)graphics.o\
		$(OUT)$(KERNEL)utils.o\
		$(OUT)$(KERNEL)utils86.o\
		$(OUT)$(KERNEL)gdt.o\
		$(OUT)$(KERNEL)gdtASM.o\
		$(OUT)$(KERNEL)winmgr.o\
		$(OUT)$(KERNEL)idtASM.o\
		$(OUT)$(KERNEL)idt.o\
		$(OUT)$(KERNEL)timer86.o\
		$(OUT)$(KERNEL)stdio.o\
		$(OUT)$(KERNEL)timer.o\
		$(OUT)$(KERNEL)PS2-x86.o\
		$(OUT)$(KERNEL)stage3.o\
		$(OUT)$(KERNEL)paging.o\
		$(OUT)$(KERNEL)pagingASM.o\
		$(OUT)$(KERNEL)memmgr.o\
	 -lgcc -fPIC

	