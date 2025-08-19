
ENTRY(_OS_Prep)

/* - 0xC0000000 */

SECTIONS
{

	. = 2M;

    ld_start = .;
    
	.multiboot : {
		*(.multiboot)
	}
	.boot : {
		*(.boot)
	}


	.text ALIGN (4K) : AT (ADDR (.text))
	{
		*(.text)
	}

	.rodata ALIGN (4K) : AT (ADDR (.rodata))
	{
		*(.rodata)
	}

	.data ALIGN (4K) : AT (ADDR (.data) )
	{
		*(.data)
	}

    ld_end_data = .;
	.bss ALIGN (4K) : AT (ADDR (.bss) )
	{
		ld_sbss = .;
		*(COMMON)
		*(.bss)
		ld_endbss = .;
	}

	ld_end = .;
}