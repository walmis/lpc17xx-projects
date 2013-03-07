/* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* THE AUTHORS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER. USE IT AT YOUR OWN RISK */

MEMORY
{

	FLASH (rx) : ORIGIN = 0x00001000, LENGTH = 28k

    RAM (rwx) : ORIGIN = 0x10000000, LENGTH = 8k-16
}



/* SECTION command : Define mapping of input sections */
/* into output sections. */

RAM_top = ORIGIN(RAM) + LENGTH(RAM);

/* Used for interrupt handlers */
HANDLER_STACK_SIZE = 0;

/* Used during normal program execution */
THREAD_STACK_SIZE = 512;

PROVIDE(__ram_start = ORIGIN(RAM));
PROVIDE(__flash_start = ORIGIN(FLASH));
PROVIDE(__ram_end   = ORIGIN(RAM) + LENGTH(RAM));
PROVIDE(__heap_end  = ORIGIN(RAM) + LENGTH(RAM) - THREAD_STACK_SIZE - HANDLER_STACK_SIZE);
PROVIDE(__stack_start  = __heap_end);
PROVIDE(__stack_end  = __ram_end);

SECTIONS
{
	/******************************************/
	/* code section */

        /* "normal" code */
	
	.text :
	{
		KEEP(*(.irq_vectors .irq_vectors.*))
		KEEP(*(.fwversion))
		
		/*. = 0x000002FC; 
                KEEP(*(.crp))
		*/
		/* Create a symbol for each input file in the current section, set to
		 * the address of the first byte of data written from that input file.
		 * */
		CREATE_OBJECT_SYMBOLS
		
		/* ABOUT .gnu.linkonce.* sections
		 * 
		 * Unlike other input section types a section that is prefixed with
		 * .gnu.linkonce. is treated differently by the linker. If for example
		 * .gnu.linkonce.t.abc appears in two or more different object files then 
		 * the linker will only keep one and discard the others.
		 * 
		 * This is done is to support C++ vague linkage.  It is related to
		 * C++'s ODR (One Definition Rule), and can cause surprises if ODR is
		 * violated.
		 * 
		 * For example if you compile with RTTI turned on, you'll see linkonce
		 * sections for all the RTTI of all the classes in the translation unit
		 * of that object file.
		 * Or if you keep generated functions for inline functions 
		 * (-fkeep-inline-functions), you'll see linkonce sections for all the
		 * emitted kept inline functions.
		 * 
		 * Might be replaced by ELF section groups in newer versions of ld.
		 */
		*(.text .text.* .gnu.linkonce.t.*)
		. = ALIGN(4);
		
		/* Position independent code will call non-static functions via the
		 * Procedure Linkage Table or PLT. This PLT does not exist in .o files.
		 * In a .o file, use of the PLT is indicated by a special relocation.
		 * When the program linker processes such a relocation, it will create
		 * an entry in the PLT
		 */
		*(.plt)
		. = ALIGN(4);
		
		/* These sections glue together ARM and Thumb code. 
		 * .glue_7 is used for ARM code calling Thumb code, and .glue_7t is
		 * used for Thumb code calling ARM code.
		 */
		*(.glue_7t)
		*(.glue_7)
		
		/* .ARM.extab names a section that contains exception unwinding information */
		*(.ARM.extab* .gnu.linkonce.armextab.*)
		
		/* .gcc_except_table is an input section name, which gcc uses
		 * for information used to unwind the stack when an exception occurs. */
		*(.gcc_except_table)
		
		/* When gcc generates code that handles exceptions, it produces tables
		 * that describe how to unwind the stack. These tables are found in
		 * the .eh_frame and .eh_frame_hdr section.
		 * 
		 * See http://www.airs.com/blog/archives/460 or
		 * http://www.codesourcery.com/public/cxx-abi/exceptions.pdf
		 */
		*(.eh_frame_hdr)
		*(.eh_frame)
		
		*(.gnu.warning)
		
		*(.rodata .rodata.* .gnu.linkonce.r.*)
		
		. = ALIGN(4);
		KEEP(*(.init))
		
		. = ALIGN(4);
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
		
		. = ALIGN(4);
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
		
		. = ALIGN(4);
		KEEP(*(.fini))
		
		. = ALIGN(4);
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		PROVIDE_HIDDEN (__fini_array_end = .);
		
		/* These are for static constructors and destructors under ELF */
		PROVIDE(__ctors_start__ = .);
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*crtend.o(.ctors))
		PROVIDE(__ctors_end__ = .);

		/*PROVIDE(__dtors_start__ = .);
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))
		PROVIDE(__dtors_end__ = .);*/
		
		*(.init .init.*)
		*(.fini .fini.*)
		
	} >FLASH
	

	.fastcode :
	{
		__fastcode_load = LOADADDR (.fastcode);		/* address in FLASH */
		__fastcode_start = .;						/* address in RAM */
		
		/* used for vectors remapped to RAM */
		KEEP(*(.vectors))
		KEEP(*(.fastcode))
		
		*(.glue_7t)		/* TODO: are these usefull here or better only in .text? */
		*(.glue_7)
		
		. = ALIGN (4);
		__fastcode_end = .;
	} >RAM AT>FLASH
	
	/* .ARM.exidx is sorted, so has to go in its own output section.  */
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} >FLASH
	__exidx_end = .;


	.fastcode :
	{
		__fastcode_load = LOADADDR (.fastcode);		/* address in FLASH */
		__fastcode_start = .;						/* address in RAM */
		
		/* used for vectors remapped to RAM */
		KEEP(*(.vectors))
		KEEP(*(.fastcode))
		
		*(.glue_7t)		/* TODO: are these usefull here or better only in .text? */
		*(.glue_7)
		
		. = ALIGN (4);
		__fastcode_end = .;
	} >RAM AT>FLASH


	/* initialized variables */
	.data : ALIGN (8)
	{
		__data_load = LOADADDR (.data);		/* address in FLASH */
		__data_start = .;					/* address in RAM */
		
		KEEP(*(.jcr))
		*(.got.plt) *(.got)
		*(.shdata)
		
		*(.data .data.* .gnu.linkonce.d.*)
		
		. = ALIGN (4);
		__data_end = .;
	} >RAM AT>FLASH
	
	.noinit :
	{
		. = ALIGN (4);
		PROVIDE (__noinit_start = .);
		
		KEEP(*(.noinit*))
		
		. = ALIGN (4);
		PROVIDE (__noinit_end = .);
	} >RAM
	
	/******************************************/
	/* For no-init variables section */
	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		__bss_start = . ;

		*(.bss .bss.*)
		*(.gnu.linkonce.b*)
		*(COMMON)

		. = ALIGN(4);
		__bss_end = . ;
	} >RAM
	
	. = ALIGN (4);
	PROVIDE (__heap_start = .);
	

	
	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	
	/* DWARF debug sections.
	 Symbols in the DWARF debugging sections are relative to the beginning
	 of the section so we begin them at 0.  */
	
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
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	
	.note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
	.ARM.attributes 0 : { KEEP (*(.ARM.attributes)) }
	/DISCARD/ : { *(.note.GNU-stack)  }
  	
  	

}
