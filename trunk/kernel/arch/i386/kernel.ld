OUTPUT_FORMAT(binary)
ENTRY(init)

phys_begin = 0x100000;
page_align = 0x1000;

SECTIONS
{
	.text phys_begin : AT(phys_begin)
	{
		code = . ;
		*(.text)
		. = ALIGN(page_align);
	}
	.data : AT(phys_begin + (data - code))
	{
		data = . ;
		*(.data)
		. = ALIGN(page_align);
	}
	.bss : AT(phys_begin + (bss - code))
	{
		bss = . ;
		*(.bss)
		. = ALIGN(page_align);
	}
	end = . ;
}
