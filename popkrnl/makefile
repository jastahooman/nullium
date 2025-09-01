OUT		   = 	../../bin/popkrnl/

#GCC64 	   = 	x86_64-elf-gcc
GCC		   =    i686-elf-gcc
NASM 	   = 	nasm
MAKE	   = 	make
KERNEL	   =	
SDK 	   =	sdk/
LIMINE	   =    ../../limine/
IMG		   =	../img/

GAS 	   = 	i686-elf-as
#GAS64 	   = 	x86_64-elf-as


GCC_86_SETTINGS = -std=gnu99 -ffreestanding -O2 -O6 -Wall -Wextra -fPIC -I include/ -g




make:
	@make clean

	@echo -e " "
	@echo -e " "
	@echo -e "Please say an option for what to compile."
	@echo -e "=== KERNEL:"
	@echo -e "'make mboot2-i686' - Make Intel IA-32 binaries for GRUB Multiboot2."
	@echo -e " "
	@echo -e "=== SDK:"
	@echo -e "'make i686-sdk' - Compile PopKrnl SDK for Intel IA-32."
	@echo -e " "
	@echo -e " "



clean:
	#rm -rf $(OUT)/

i686-sdk:
	


bin-i686:
	
	$(GCC) -c $(KERNEL)stage3/krnlBitmaps.c -o $(OUT)$(KERNEL)krnlBitmaps.o $(GCC_86_SETTINGS)
	$(GCC) -c $(KERNEL)drivers/graphics.c -o $(OUT)$(KERNEL)graphics.o $(GCC_86_SETTINGS)
	
	$(GCC) -c $(KERNEL)utils/utils.c -o $(OUT)$(KERNEL)utils.o $(GCC_86_SETTINGS)

	$(GCC) -c $(KERNEL)stage3/sysfunc.c -o $(OUT)$(KERNEL)sysfunc.o $(GCC_86_SETTINGS)
	

	$(GCC) -c $(KERNEL)stage3/stdio.c -o $(OUT)$(KERNEL)stdio.o $(GCC_86_SETTINGS)
	
	$(GCC) -c $(KERNEL)drivers/timer.c -o $(OUT)$(KERNEL)timer.o $(GCC_86_SETTINGS)
	$(GCC) -c $(KERNEL)mem/pmm.c -o $(OUT)$(KERNEL)pmm.o $(GCC_86_SETTINGS)




	$(GCC) -c $(KERNEL)utils/utils-x86.c -o $(OUT)$(KERNEL)utils86.o $(GCC_86_SETTINGS)
	$(GCC) -c $(KERNEL)utils/utils-x86.c -o $(OUT)$(KERNEL)utils86.o $(GCC_86_SETTINGS)
	$(GCC) -c $(KERNEL)boot/stage2/stage2-x86.c -o $(OUT)$(KERNEL)stage2.o $(GCC_86_SETTINGS)
	
	$(GCC) -c $(KERNEL)boot/stage1/gdt.c -o $(OUT)$(KERNEL)gdt.o $(GCC_86_SETTINGS)
	$(NASM) -felf32 $(KERNEL)boot/stage1/gdt.s -o $(OUT)$(KERNEL)gdtASM.o
	
	$(GCC) -c $(KERNEL)io/idt.c -o $(OUT)$(KERNEL)idt.o $(GCC_86_SETTINGS)
	$(NASM) -felf32 $(KERNEL)io/idt.s -o $(OUT)$(KERNEL)idtASM.o

	$(GCC) -c $(KERNEL)io/scheduler.c -o $(OUT)$(KERNEL)scheduler.o $(GCC_86_SETTINGS)

	$(GCC) -c $(KERNEL)drivers/timer-x86.c -o $(OUT)$(KERNEL)timer86.o $(GCC_86_SETTINGS)
	
	$(GCC) -c $(KERNEL)stage3/stage3.c -o $(OUT)$(KERNEL)stage3.o $(GCC_86_SETTINGS)
	
	$(GCC) -c $(KERNEL)drivers/PS2-x86.c -o $(OUT)$(KERNEL)PS2-x86.o $(GCC_86_SETTINGS)
	


	$(GCC) -c $(KERNEL)mem/paging.c -o $(OUT)$(KERNEL)paging.o $(GCC_86_SETTINGS)
	$(GCC) -c $(KERNEL)stage3/nterm.c -o $(OUT)$(KERNEL)nterm.o $(GCC_86_SETTINGS)

mboot2-bin:
	
	$(NASM) -felf32 $(KERNEL)boot/stage1/entry.s -o $(OUT)$(KERNEL)entry.o 
	$(GCC) -c $(KERNEL)boot/stage1/stage1.c -o $(OUT)$(KERNEL)stage1.o $(GCC_86_SETTINGS)
	

	@$(GCC) -T $(KERNEL)boot/stage1/linker.ld -o $(OUT)../popkrnl.bin -ffreestanding -O2 -nostdlib\
		$(OUT)$(KERNEL)entry.o\
		$(OUT)$(KERNEL)stage1.o\
		$(OUT)$(KERNEL)stage2.o\
		$(OUT)$(KERNEL)krnlBitmaps.o\
		$(OUT)$(KERNEL)graphics.o\
		$(OUT)$(KERNEL)utils.o\
		$(OUT)$(KERNEL)utils86.o\
		$(OUT)$(KERNEL)gdt.o\
		$(OUT)$(KERNEL)gdtASM.o\
		$(OUT)$(KERNEL)idtASM.o\
		$(OUT)$(KERNEL)idt.o\
		$(OUT)$(KERNEL)timer86.o\
		$(OUT)$(KERNEL)stdio.o\
		$(OUT)$(KERNEL)timer.o\
		$(OUT)$(KERNEL)sysfunc.o\
		$(OUT)$(KERNEL)PS2-x86.o\
		$(OUT)$(KERNEL)stage3.o\
		$(OUT)$(KERNEL)pmm.o\
		$(OUT)$(KERNEL)paging.o\
		$(OUT)$(KERNEL)nterm.o\
		$(OUT)$(KERNEL)scheduler.o\

	 -lgcc -fPIC



mboot2-i686:
	@mkdir -p $(OUT)

	@make bin-i686
	@make mboot2-bin

	