.data

prompt:	.asciz "Enter 3 numbers, pressing Enter after each\n"  @ Format string for prompt
	.align 2

strin:	.asciz "%d" @ A place to take in an integer
	.align 2

str2:	.asciz "You entered %d, %d, %d\n" @ Output string
	.align 2

neg:	.asciz "You entered %d negative numbers and %d positive integers\n" @ Output string for negative numbers
	.align 2

minmsg:	.asciz "%d is the minimum number\n" @ Output string for minimum number
	.align 2
	
maxmsg:	.asciz "%d is the maximum number\n" @ Output string for maximum number
	.align 2
	
int1:	.word 0
int2:	.word 0
int3:	.word 0
ncount:	.word 0
npos:	.word 3
min:	.word 0
max:	.word 0
	
	.text
	.global main 
main:	stmfd sp!, {lr}  /* Push lr onto the stack */

	/* printf to prompt the user */
	ldr r0, =prompt 		/* Load reg0 with the address of prompt */
	bl printf			/* Call printf */

	/* scanf to take in integers */
	ldr r0, =strin 			/* Load r0 with the input string */
	ldr r1, =int1			/* Load address of int variable */
	bl scanf			/* Make call to scanf */
	ldr r0, =strin			/* Load empty string */
	ldr r1, =int2			/* Load address of second int variable */
	bl scanf			/* Make call to scanf */
	ldr r0, =strin			/* Load empty string */
	ldr r1, =int3			/* Load address of thrid int variable */
	bl scanf			/* Make call to scanf */

	/* printf("You entered %d, %d, %d\n", int1, int2, int3) */
	ldr r0, = str2			/* Load r0 with address of prompt */
	ldr r1, =int1			/* Load r1 with address of first int */
	ldr r1, [r1]			/* Load value */
	ldr r2, =int2			/* Load r2 with adress of second int */
	ldr r2, [r2]			/* Load value */
	ldr r3, =int3			/* Load r3 with address of second int */
	ldr r3, [r3]			/* Load value */
	bl printf			/* Make call to printf */

	/* Min */
	ldr r1, =int1
	ldr r1, [r1]
	ldr r2, =int2
	ldr r2, [r2]
	ldr r3, =int3
	ldr r3, [r3]
	cmp r1, r2 			/* compare r1 - r2 */
	movge r10, r2			/* if r1 >= r2 */
	movlt r10, r1			/* if r2 > r1 */
	cmp r3, r10			/* compare r3 - r0(current min) */
	movlt r10, r3			/* if r3 < min, min = r3 */
	
	/* printf("%d is the minimum number", &min) */
	mov r1, r10			/* r10 still has min from last operation */
	ldr r0, =minmsg			/* load address of message */
	bl printf			/* make call to printf */

	/* Max */
	ldr r1, =int1
	ldr r1, [r1]
	ldr r2, =int2
	ldr r2, [r2]
	ldr r3, =int3
	ldr r3, [r3]
	cmp r1, r2			/* compare r1 - r2 */
	movlt r10, r2			/* if r1 < r2 */
	movge r10, r1			/* if r2 <= r1 */
	cmp r3, r10			/* compare r10 - r2 */
	movgt r10, r3			/* if r3 > r10 */

	/* printf("%d is the maximum number", &max) */
	mov r1, r10			/* r10 has max value from above */
	ldr r0, =maxmsg			/* load message address */
	bl printf			/* make call to printf */
	
	/* if(negative) negcount++ */
	mov r10, #0			/* Load r0 with 0 */
	ldr r1, =int1                   /* Load r1 with address of first int */
        ldr r1, [r1]                    /* Load value */
        ldr r2, =int2                   /* Load r2 with adress of second int */
        ldr r2, [r2]                    /* Load value */
        ldr r3, =int3                   /* Load r3 with address of second int */
        ldr r3, [r3]                    /* Load value */
	cmp r1, #0			/* if(r1 < 0) */
	addmi r10, r10, #1		/* negcount++ */
	cmp r2, #0			/* if(r2 < 0) */
	addmi r10, r10, #1		/* negcount++ */
	cmp r3, #0			/* if(r3 < 0) */
	addmi r10, r10, #1		/* negcount++ */

	/* positive count = 3 - neg count */
	mov r11, #3			/* Load r11 with the literal #3 */
	sub r11, r11, r10		/* pos count = 3 - r10 (negcount) */
	
	/* printf("You entered %d negative numbers",negcount) */
	ldr r0, =neg			/* Load r0 with address of output string */
	mov r1, r10 			/* Load r1 with negative count (r10) */
	mov r2, r11			/* Load r2 with positive count (r4) */
	bl printf			/* Make call to printf */
	
	ldmfd sp!, {lr}			/* pop from stack */
	mov r0, #0			/* load return value */
	mov pc, lr			/* return from main */
	.end
