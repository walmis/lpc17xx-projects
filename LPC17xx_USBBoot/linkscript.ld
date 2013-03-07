/* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* THE AUTHORS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER. USE IT AT YOUR OWN RISK */

MEMORY
{
        /* LPC1768 : 512k ROM + 64k SRAM */
        /*------------------------------ */

	/* On-chip ROM is a readable (r), executable region (x) */
	/* On-chip SRAM is a readable (r), writable (w) and */
	/* executable region (x) */

	/* Main ROM region - 512k for LPC1768 */
	IROM (rx) : ORIGIN = 0x00000000, LENGTH = 32k

        /* local static RAM - 32k for LPC1756 */
        IRAM0 (rwx) : ORIGIN = 0x10000000, LENGTH = 8k

        /* AHB SRAM - 16k for LPC1756 - often used for USB */
        IRAM1 (rwx) : ORIGIN = 0x2007C000, LENGTH = 16k
		/*IRAM2 (rwx) : ORIGIN = 0x20080000, LENGTH = 16k*/
}

/* SECTION command : Define mapping of input sections */
/* into output sections. */

IRAM0_top = ORIGIN(IRAM0) + LENGTH(IRAM0);

SECTIONS
{
	/******************************************/
	/* code section */

        /* "normal" code */
        
	
	.text :
	{
		KEEP(*(.irq_vectors .irq_vectors.*))
		
		KEEP(*(.precrp)) 
		*sbl_iap.o (.text.*)
		*usbinit.o (.text.*)
		
		. = 0x000002FC ; /* or 1FC for LPC2000 */
                KEEP(*(.crp))
		
		*(.text .text.*)
		*(.gnu.linkonce.t.*)
		*(.glue_7)
		*(.glue_7t)
		*(.gcc_except_table)
		*(.rodata .rodata*)
		*(.gnu.linkonce.r.*)
		. = ALIGN(4);
		KEEP(*(.init))
		
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
		
		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;
		
		. = ALIGN(0x4);
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*crtend.o(.ctors))
		
		. = ALIGN(4);
		KEEP(*(.fini))
		
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;
		

		. = ALIGN(0x4);
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))
		
		/* End Of .text section */
		_etext = .;
                _sifastcode = .;
	} >IROM
	

	
	/* .ARM.exidx is sorted, so has to go in its own output section.  */
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} >IROM
	__exidx_end = .;

        /**************************************************/
        /* fastcode - copied at startup & executed in RAM */

        .fastcode :
        {
          . = ALIGN (4);
          _sfastcode = . ;

          *(.glue_7t) *(.glue_7)
          *(.fastcode)
          
          /* add other modules here ... */

          . = ALIGN (4);
          _efastcode = . ;
          _sidata = .;
        } >IRAM0 AT>IROM

	/******************************************/
	/* This used for USB RAM section */
	.usb_ram (NOLOAD):
	{
		*.o (USB_RAM)
	} > IRAM1

        /******************************************/
	/* data section */
	.data :
  	{
        _sidata = LOADADDR (.data);
  		. = ALIGN(4);
		_sdata = .;

		*(vtable vtable.*)
		*(.data .data.*)
		*(.gnu.linkonce.d*)

		. = ALIGN(4);
		_edata = . ;
	} >IRAM0 AT>IROM

	/******************************************/
	/* For no-init variables section */
	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		_sbss = . ;

		*(.bss .bss.*)
		*(.gnu.linkonce.b*)
		*(COMMON)

		. = ALIGN(4);
		_ebss = . ;
	} >IRAM0

  	
  	stack_entry = IRAM0_top - 32;

	/******************************************/
	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	/* .comment       0 : { *(.comment) } */
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
}
