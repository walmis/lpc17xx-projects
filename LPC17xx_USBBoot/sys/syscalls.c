/*
 * syscalls.c
 *
 *  Created on: 03.12.2009
 *      Author: Martin Thomas, 3BSD license
 */
 /* MOdified by Sagar G V, Feb 2011.
 Ported to LPC17xx.
 */

//#include "syscalls.h"
#include <lpc17xx.h>
#include <errno.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

#undef errno
extern int errno;

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

void _exit(int status)
{
	while(1) {;}
}

int _getpid(void)
{
	return 1;
}


extern char _ebss; /* Defined by the linker */
static char *heap_end;

inline char* get_heap_end(void)
{
	return (char*) heap_end;
}

inline char* get_stack_top(void)
{
	return (char*) __get_MSP();
	// return (char*) __get_PSP();
}

caddr_t _sbrk(int incr)
{
	char *prev_heap_end;
	if (heap_end == 0) {
		heap_end = &_ebss;
	}
	prev_heap_end = heap_end;
#if 1
	if (heap_end + incr > get_stack_top()) {

		abort();
	}
#endif
	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

int _close(int file)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir) {

	return 0;
}

int _read(int file, char *ptr, int len)
{

	return 0;

}

int _write(int file, char *ptr, int len)
{
	int todo;

	for (todo = 0; todo < len; todo++) {
		ptr++;
	}
	return len;
}


#ifdef __cplusplus
}
#endif
