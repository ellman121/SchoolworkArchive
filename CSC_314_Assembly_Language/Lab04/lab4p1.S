.data

buffer:	.skip 4096, 0			 	/* char buffer[4096] = '\0' */
	.align 2

str1:	.asciz "Enter text (ctrl-D to end): "	 /* Format string */
	.align 2
	
str2:	.asciz "\nThe checksum is %d\n"	 /* Format string */
	.align 2

str3:	.asciz "%s\n"				/* Format string */
	.align 2

str4:	.asciz "%4096c"				/* testing string */
	.align 2

teststr: .asciz "This is the end of checksum"
	.align 2

.text

/*==================================================================*/
	/* int checksum() */
checksum:
	stmfd sp!, {lr}				/* push lr onto the stack */
	
	/* printf("Enter text (ctrl-D to end): "); */
	ldr r0, =str1				/* Load r0 with address of string */
	bl printf				/* Printf call */

	ldr r0, =str4
	ldr r1, =buffer
	bl scanf

	ldr r3, =sum
	ldr r3, [r3]
	mov r0, #0				/* set r0 to 0 */
start:	ldr r1, =buffer				/* load address of buffer */
	add r1, r1, r0				/* increment */
	ldrb r1, [r1]				/* load 1/4 word value */
	cmp r1, #0
	beq end
	add r3, r3, r1
	add r0, r0, #1
	b start
end:	
	
	/* return sum in register 0 */
	ldmfd sp!, {lr}				/* pop from stack */
	mov r0, r3				/* r2 still has sum from last loop */
	mov pc, lr				/* return from function */

/*==================================================================*/
	/* int main() */
	.global main
main:	stmfd sp!, {lr}				/* push lr onto the stack */

	bl checksum				/* make call to checksum */
	
	mov r1, r0				/* r0 has things in it from checksum */
	ldr r0, =str2				/* load address of string */
	bl printf				/* make call to printf */
	
	ldmfd sp!, {lr}				/* pop lr from the stack */
	mov r0, #0				/* load return value for main */
	mov pc, lr				/* return from main */
.end
