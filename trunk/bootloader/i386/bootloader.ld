OUTPUT_FORMAT(binary)
ENTRY(init)

boot_begin = 0x100000;

SECTIONS
{
	.all boot_begin : AT(boot_begin)
	{
		all = . ;
		*(.text)
		*(.data)
		*(.bss)
	}
}
