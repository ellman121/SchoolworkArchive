/***************************************************************************//**
 * \file hellogcc.S
 *
 * \brief Hello, World in ARM
 *
 * \author Scott K Logan
 *
 * \date Tue Jan 15, 2012
 *
 * This is a simple Hello, World program written in ARM assembly and using
 * Linux system calls.
 * This version requires the C standard library, which provides the _start
 * function.  The _start function will call main, which is in this file.
 * It should be compiled and linked as follows:
 *   gcc -o hellogcc hellogcc.S
 * gcc will call the assembler and linker, telling the linker to include
 * the C standard library.
 ******************************************************************************/

	.data
msg:		
	.ascii "Hello, World!\n"
len 	= . - msg
	
	.text
.globl main
main:
	/* syscall write(int fd, const void *buf, size_t count) */
	mov r0, $1	/* fd -> stdout */
	ldr r1, =msg	/* buf -> msg */
	ldr r2, =len	/* count -> len(msg) */
	mov r7, $4	/* write is syscall #4 */
	swi $0		/* invoke syscall */
	/* return from main() */
        mov r0, $0      /* status -> 0 */
	mov pc,lr       /* return to _start function. It will call exit */

