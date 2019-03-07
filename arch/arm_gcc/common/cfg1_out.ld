/*
 *  $Id: cfg1_out.ld 487 2018-10-23 14:48:52Z ertl-hiro $
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm","elf32-littlearm") 
OUTPUT_ARCH(arm)

PROVIDE(_gp = 0);
PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);

SECTIONS
{
	vector : {
		__start_text = .;
		*(.vector)
	}

	.text : {
		*(.text .text.*)
		_end_text = .;
	}
	.rodata : {
		__start_rodata = .;
		*(.rodata .rodata.*)
		__end_rodata = .;
	}

	. = ALIGN(4);
	__start_idata = .;
	.data : AT(__start_idata) {
		__start_data = .;
		*(.data)
	}
	__end_idata = __start_idata + SIZEOF(.data);
	__end_data = .;

	. = ALIGN(4);
	.bss : {
		__start_bss = .;
		*(.bss)
		*(COMMON)
		__end_bss = .;
	}
	. = ALIGN(4);
	PROVIDE(end = .);

	.comment 0 : { *(.comment) }	

	/* DWARF debug sections.
	Symbols in the DWARF debugging sections are relative to 
	the beginning of the section so we begin them at 0.  */

	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }

	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }

	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }

	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	.debug_ranges   0 : { *(.debug_ranges) }

	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
}
