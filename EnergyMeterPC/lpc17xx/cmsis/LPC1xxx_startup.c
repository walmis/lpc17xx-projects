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
extern unsigned long __data_load;		/* start address for the initialization values of the .data section. defined in linker script */
extern unsigned long __data_start;		/* start address for the .data section. defined in linker script */
extern unsigned long __data_end;		/* end address for the .data section. defined in linker script */

extern unsigned long __fastcode_load;		/* start address for the initialization values of the .fastcode section. defined in linker script */
extern unsigned long __fastcode_start;		/* start address for the .fastcode section. defined in linker script */
extern unsigned long __fastcode_end;		/* end address for the .fastcode section. defined in linker script */

extern unsigned long __bss_start;			/* start address for the .bss section. defined in linker script */
extern unsigned long __bss_end;			/* end address for the .bss section. defined in linker script */

extern void __stack_end;		/* init value for the stack pointer. defined in linker script */

extern void __flash_start;

// Prototype the required startup functions
extern void main(void);
extern void SystemInit(void);
extern void  __libc_init_array();
extern void __xpcc_initialize_memory(void);
// The entry point of the application, prepare segments,
// initialize the cpu and execute main()


extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));
extern void (*__fini_array_start []) (void) __attribute__((weak));
extern void (*__fini_array_end []) (void) __attribute__((weak));

//void *__dso_handle __attribute__ ((__visibility__ ("hidden"))) = &__dso_handle;

//int __aeabi_atexit(void *object,void (*destructor)(void *),void *dso_handle)
//{
//	//object = object; // avoid warnings
//	//destructor = destructor;
//	//dso_handle = dso_handle;
//    return 0;
//}

inline __attribute__((always_inline)) void __ctors_init() {
    register int count;
    register int i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
      __preinit_array_start[i] ();

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
      __init_array_start[i] ();
}



inline __attribute__((always_inline)) void __segs_init() {

    //
    // Copy the data segment initializers from flash to SRAM in ROM mode
    //

	register unsigned long* src = &__fastcode_load;
	register unsigned long* dest = &__fastcode_start;
	while (dest < &__fastcode_end)
	{
		*(dest++) = *(src++);
	}

	// Copy the data segment initializers from flash to RAM (.data)
	src = &__data_load;
	dest = &__data_start;
	while (dest < &__data_end)
	{
		*(dest++) = *(src++);
	}

	// Fill the bss segment with zero (.bss)
	dest = &__bss_start;
	while (dest < &__bss_end)
	{
		*(dest++) = 0;
	}

}
void boot_entry(void) __attribute__((noreturn));

void boot_entry(void)
{
	SystemInit();



	__segs_init();

	__xpcc_initialize_memory();

    __ctors_init();


	main();

	// Do nothing when returned from main, just keep looping
	while(1);
}
