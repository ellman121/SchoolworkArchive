.data

prompt:		.asciz "Input a positive integer: "	/* String for prompt */
		.align 2

buffer:		.asciz "%d"				/* Format string to read in an integer */
		.align 2

outbuffer:	.asciz "%d\n"				/* format output string */
		.align 2
	
output:		.asciz "%d primes\n"			/* C-string for reasons and things */
		.align 2

n:		.word 0					/* Integer to hold user's value n */

.text
/*========================================================================================*/
		/* isprime(int n) */
isprime:	stmfd sp!, {r4-r10,lr}			/* Push link register to stack */

		mov r4, r0				/* copy our argument | r4 = x */
		mov r5, #2				/* Load r5 with 2 | r5 = i */

		cmp r4, #1				/* 1 is not prime -> return 1 */
		moveq r0, #1				/* load return value */
		beq retisprime				/* return */
	
		cmp r4, #2				/* 2 is prime -> return 0 */
		beq enda

		cmp r4, #3				/* 3 is prime -> return 0 */
		beq enda	
	
		/* for( i = 2...x ) */
starta:		cmp r5, r4				/* Compare i and n */
		beq enda				/* skip the loop if we need to break */

		mov r0, r4				/* set up arguments for divide */
		mov r1, r5				/* x / i -> r4 / r5 */
		bl divide				/* r0 = quotient, r1 = remainder */

		cmp r1, #0				/* if remainder is 0, the number is not prime */
		moveq r0, #1				/* return 1 if the number is not prime */		
		beq retisprime				/* return */

		add r5, r5, #1				/* increment i */
	
		b starta				/* loop up */
enda:		mov r0, #0				/* load return value - 0 if the number is prime */

retisprime:	ldmfd sp!, {r4-r10,lr}			/* Clear our stack */
		mov pc, lr				/* End function */
	
/*========================================================================================*/
		/* void divide(int &x, int &y) */
divide:		stmfd sp!, {lr}				/* Push link register to the stack */

		mov r2, r1				/* y */
		mov r1, r0				/* x */
		mov r0, #0				/* More setup */
		mov r3, #1
startb:		cmp r2, r1				/* is y < x */
		movls r2, r2, lsl#1			/* shift y left until y >= x */
		movls r3, r3, lsl#1			/* also shift r3 */
		bls startb				/* loop up */
	
loop:		cmp r1, r2				/* carry set if r1 > r2 */
		subcs r1, r1, r2			/* if(carry set) r1 = r1 - r2 */
		addcs r0, r0, r3			/* if(carry set) r0 = r0 + r3 */
		movs r3, r3, lsr#1			/* shift r3 right*/
		movcc r2, r2, lsr#1			/* if(r3 > 0) shift r2 right */
		bcc loop				/* loop up */
	
retdivide:	ldmfd sp!, {lr}				/* pop lr from the stack */
		/* quotient is in r0, remainder is in r1 */
		mov pc, lr				/* return from function */

/*========================================================================================*/
	
.global main
main:		stmfd sp!, {lr}				/* Push program counter to stack */

		/* printf("Input a positive integer: ") */
		ldr r0, =prompt				/* load address of prompt */
		bl printf

		/* scanf(buffer, &n) */
		ldr r0, =buffer				/* load address of buffer */
		ldr r1, =n				/* load address of n */
		bl scanf

		ldr r1, =n				/* load n */
		ldr r1, [r1]
	
		mov r5, #1				/* i = 0 */
		mov r6, r1				/* Copy r1 (n) to r6 */
		mov r7, #0				/* numprimes */

		/* for(int i = 0; i < n; i++) */
startc:		cmp r5, r6				/* r6 is n */
		beq endc				/* If we need to end */

		mov r0, r5				/* set up arguments for r0 */
		bl isprime				/* call isprime */

		cmp r0, #0				/* r0 == 0 if the number is prime */
		addeq r7, r7, #1			/* numprimes++ */
		ldreq r0, =outbuffer			/* load format string for output */
		moveq r1, r5				/* copy our prime number */
		bleq printf				/* call printf */

		add r5, r5, #1				/* increment i */
		b startc				/* loop up */
endc:
		ldr r0, =output				/* load final output string */
		mov r1, r7				/* copy numprimes */
		bl printf				/* make call to printf */
	
		ldmfd sp!, {lr}				/* pop lr from the stack */
		mov r0, #0				/* return value from main */
		mov pc, lr				/* return from main */
.end
