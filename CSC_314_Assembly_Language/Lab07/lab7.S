.data

newline: .asciz "\n"
	.align 2
	
buffer:	.asciz "%s"					/* Input buffer for 32-Bit integers */
	.align 2

buff2:	.asciz "%d"
	.align 2

msg:	.asciz "This program will output a table for the function f(x) = (x^2 - 3x - 2) / (x^2 - 4)\n"
	.align 2					/* Output descritive message */

input1:	.asciz "Please give me the lower value for x: "
	.align 2

input2:	.asciz "Please give me an upper limit for x: "
	.align 2

input3:	.asciz "How many rows do you want in the table: "
	.align 2

temp:	.skip 16
	.align 2

tabtop:	.asciz "x      | f(x)\n---------------\n"	/* Top of table in format string */
	.align 2

onerow:	.asciz "f(x_low) = "				/* Things */
	.align 2

tab:	.asciz "\t"
	.align 2
	
.text
/* ========================================================================== */
	/* Calculate Denominator --- r0 = r0^2 - 4 --- EXPECTS A S(15,16) */
cdenom:	mov r2, r0					/* copy x */
	
	smull r0, r1, r2, r2				/* x^2 (we now have a S(31, 32) */
	lsr r0, r0, #16					/* shift */
	orr r0, r1, lsl #16				/* or them together */
	
	sub r0, r0, #262144				/* subtract 4 */
	
endcde:	mov pc, lr					/* return */

/* ========================================================================== */
	/* Calculate Numerator */
cnumer: mov r2, r0					/* copy x */

	smull r0, r1, r2, r2				/* x^2 */
	lsr r0, r0, #16					/* shift */
	orr r0, r1, lsl #16				/* or */

	lsr r2, #16					/* move over */
	mov r1, #3
	mul r2, r2, r1					/* multiply by 3 */
	add r2, r2, #2					/* add 2 */
	lsl r2, r2, #16					/* move back */

	sub r0, r0, r2					/* s^2 - (3x + 2) */
	
endcn:	mov pc, lr					/* return */
/* ========================================================================== */	
	/* f(x) =  (x^2 - 3x - 2) / (x^2 - 4) */
funx:	stmfd sp!, {r4-r9, lr}

	mov r4, r0					/* copy x in to r4 */
	cmp r0, #2					/* if x == 2, we can't divide by 0 */
	moveq r0, #0xFFFFFFFF				/* Return max int value */
	eoreq r0, #0x80000000				/* XOR the sign bit */
	beq endfunx

	// CALCULATE NUMERATOR
	mov r0, r4					/* arguments */
	bl cnumer					/* calculate numerator */
	mov r8, r0					/* r8 = numerator */

	// CALCULATE DENOMINATOR
	mov r0, r4					/* copy x into r0 as argument */
	bl cdenom					/* calculate denominator */
	mov r9, r0					/* r9 = denominator */

	// r5 will be 1 if we have 1 negative, 2 if we have 2 negatives
	mov r5, #0					/* just wait */
	
	cmp r8, #0					/* if negative, make positive */
	mvnlt r8, r8					/* flip sign */
	addlt r5, r5, #1				/* add one if we have a negative */

	cmp r9, #0					/* if negative, make positive */
	mvnlt r9, r9					/* flip sign */
	addlt r5, r5, #1				/* add one if we have a negative */

	// DIVIDE
	mov r1, r8					/* SET UP ARGUMENTS */
	mov r0, #0
	mov r3, #0
	mov r2, r9
	bl sdiv64					/* r1 has high 32 bits, r2 has low 32 bits */

	// Shift back into place
	lsr r0, #16					/* shift fractional part */
	lsl r1, #16					/* shift integer part */
	orr r0, r0, r1					/* or them */
	mov r3, #1					/* blah */
	lsl r3, #31					/* shift into sign bit */
	mvn r3, r3					/* r3 = 0b0111111111111111111111111111111111111 */
	and r0, r0, r3					/* force sign bit to be 0 */

	cmp r5, #1					/* if r5 is 1, we have a negative number */
	mvneq r0, r0					/* flip sign */

	// RETURN result in r0
endfunx: ldmfd sp!, {r4-r9, lr}
	mov pc, lr
	
/* ========================================================================== */	
	
.global main
main:	stmfd sp!, {lr}

	// r4 = x_low
	// r5 = x_high
	// r6 = numrows
	
	/* Give user a description of the program */
	ldr r0, =msg					/* load pointer to output */
	bl printf					/* call printf */
	
	/* Prompt and read in lower limit for x */
	ldr r0, =input1					/* load prompt */
	bl printf					/* call printf */
	ldr r0, =buffer					/* load input buffer */
	ldr r1, =temp					/* temp memory to hold our input */
	bl scanf					/* call scanf */

	/* convert the number we just input to a S(15, 16) */
	ldr r0, =temp					/* pointer to temp */
	mov r1, #16					/* argument to strtoSfxied */
	bl strtoSfixed					/* call to convert string to Signed fixed number */
	mov r4, r0					/* copy to r4 (Lower Limit) */

	/* Prompt and read in lower limit for x */
	ldr r0, =input2					/* load prompt */
	bl printf					/* call printf */
	ldr r0, =buffer					/* load input buffer */
	ldr r1, =temp					/* temp memory to hold our input */
	bl scanf					/* call scanf */

	/* convert the number we just input to a S(15, 16) */
	ldr r0, =temp					/* pointer to temp */
	mov r1, #16					/* argument to strtoSfxied */
	bl strtoSfixed					/* call to convert string to Signed fixed number */
	mov r5, r0					/* copy to r5 (Upper Limit) */

	/* Ask user for how many rows they want */
	ldr r0, =input3					/* string */
	bl printf
	ldr r0, =buff2
	ldr r1, =temp
	bl scanf
	ldr r6, =temp					/* load numrows */
	ldr r6, [r6]

	// If the user wants 1 or fewer rows
	cmp r6, #1					/* if user wants 1 row output f(x_low) */
	blt endmain					/* if < 1, exit main */
	beq oner					/* if == 1, special case for 1 row */

	/* Calculate the step (stored in r7) */
	sub r7, r5, r4					/* r7 = r5 - r4 */
	mov r0, r7					/* first argument  */
	mov r1, r6					/* num rows into second argument */
	bl udiv32					/* call divide */
	mov r7, r0					/* step into r7 */
	
	/* output table */
	ldr r0, =tabtop					/* output top of table */
	bl printf

	mov r11, r4					/* i = x_low */
loop:	cmp r11, r5					/* compare i = x_high */
	bgt endmain 					/* end if we need to */

	mov r0, r11					/* first argument */
	mov r1, #16					/* second argument */
	bl printS					/* call prints (output x) */

	ldr r0, =tab					/* spacing */
	bl printf
		
	mov r0, r11					/* copy i into r0 */
	bl funx						/* r0 = funx(i) */

	mov r1, #16					/* argument for printS */
	bl printS

	ldr r0, =newline				/* output a newline */
	bl printf

	add r11, r11, r7				/* add the step */
	b loop						/* branch up */
	
	/* one row */
oner:	ldr r0, =onerow
	bl printf
	mov r0, r4
	bl funx
	mov r1, #16
	bl printS
	ldr r0, =newline
	bl printf
	
	/* Return from main */
endmain: 
	ldmfd sp!, {lr}
	mov r0, #0
	mov pc, lr
.end
