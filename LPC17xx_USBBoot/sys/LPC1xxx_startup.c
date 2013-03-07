/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2010, Roel Verdult
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

// These are defined and created by the linker, locating them in memory
extern unsigned long _etext;
extern unsigned long _sidata;		/* start address for the initialization values of the .data section. defined in linker script */
extern unsigned long _sdata;		/* start address for the .data section. defined in linker script */
extern unsigned long _edata;		/* end address for the .data section. defined in linker script */

extern unsigned long _sifastcode;		/* start address for the initialization values of the .fastcode section. defined in linker script */
extern unsigned long _sfastcode;		/* start address for the .fastcode section. defined in linker script */
extern unsigned long _efastcode;		/* end address for the .fastcode section. defined in linker script */

extern unsigned long _sbss;			/* start address for the .bss section. defined in linker script */
extern unsigned long _ebss;			/* end address for the .bss section. defined in linker script */

extern void _estack;		/* init value for the stack pointer. defined in linker script */

// Prototype the required startup functions
extern void main(void);
extern void SystemInit(void);
extern void  __libc_init_array();
// The entry point of the application, prepare segments,
// initialize the cpu and execute main()


extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));
extern void (*__fini_array_start []) (void) __attribute__((weak));
extern void (*__fini_array_end []) (void) __attribute__((weak));

void *__dso_handle __attribute__ ((__visibility__ ("hidden"))) = &__dso_handle;

int __aeabi_atexit(void *object,void (*destructor)(void *),void *dso_handle)
{
	//object = object; // avoid warnings
	//destructor = destructor;
	//dso_handle = dso_handle;
    return 0;
}

static void _ctors_init() {
    register int count;
    register int i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
      __preinit_array_start[i] ();

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
      __init_array_start[i] ();
}



static void _segs_init() {
    register unsigned long *pulDest;
    register unsigned long *pulSrc;

    //
    // Copy the data segment initializers from flash to SRAM in ROM mode
    //

    if (&_sidata != &_sdata) {	// only if needed
		pulSrc = &_sidata;
		for(pulDest = &_sdata; pulDest < &_edata; ) {
			*(pulDest++) = *(pulSrc++);
		}
    }

    // Copy the .fastcode code from ROM to SRAM

    if (&_sifastcode != &_sfastcode) {	// only if needed
    	pulSrc = &_sifastcode;
		for(pulDest = &_sfastcode; pulDest < &_efastcode; ) {
			*(pulDest++) = *(pulSrc++);
		}
    }

    //
    // Zero fill the bss segment.
    //
    for(pulDest = &_sbss; pulDest < &_ebss; )
    {
        *(pulDest++) = 0;
    }

}
void boot_entry(void) __attribute__((__interrupt__));


#include <lpc17xx.h>
void boot_entry(void)
{

	SystemInit();

	_segs_init();
	//__libc_init_array();
    _ctors_init();

   // LPC_GPIO1->FIOSET = 1<<18;
    //while(1);
	// Execute the code at the program entry point
	main();

	// Do nothing when returned from main, just keep looping
	while(1);
}
