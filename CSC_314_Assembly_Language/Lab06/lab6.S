.data

prompt:	.asciz "Enter the number of pennies, nickels, dimes, and quarters for week %d: (Pressing enter after each, in that order)\n"
	.align 2

out1:	.asciz "Over four weeks, you have collected %d pennies, %d nickles, %d dimes, and %d quarters\n"
	.align 2

out2:	.asciz "This comes to $%d.%d\n"
	.align 2

out20:	.asciz "This comes to $%d.0%d\n"
	.align 2
	
out3:	.asciz "Your weekly average is $%d.%d\n"
	.align 2

out30:	.asciz "Your weekly average is $%d.0%d\n"
	.align 2

out4:	.asciz "Your estimated yearly savings is $%d.%d\n"
	.align 2

out40:	.asciz "Your estimated yearly savings is $%d.0%d\n"
	.align 2
	
inp:	.asciz "%d"
	.align 2

temp:	.word 0
	
.text

/* ======================================================================= */
	/* divide(r0 = X, r1 = Y) X/Y -> Returns r0 = Quotient, r1 = Remainder */
divide:	mov r2, #100
	mov r1, r0
	mov r0, #0
	mov r3, #1
start:	cmp r2, r1
	movls r2, r2, lsl#1
	movls r3, r3, lsl#1
	bls start
loop:	cmp r1, r2
	subcs r1, r1, r2
	addcs r0, r0, r3
	movs r3, r3, lsr#1
	movcc r2, r2, lsr#1
	bcc loop
	mov pc, lr

/* ======================================================================= */
	/* main || Pennies r6, nickles r7, dimes r8, quarters r9 */
.global main
main:	stmfd sp!, {lr}				/* Push link reg to stack  */

	mov r6, #0				/* load 0 into all local variables */
	mov r7, #0
	mov r8, #0
	mov r9, #0				

	mov r5, #1				/* int i = 0 */
starta:	cmp r5, #5				/* if i == 5 */
	beq enda				/* end loop */

	ldr r0, =prompt				/* Load argument for printf */
	mov r1, r5
	bl printf				/* printf(prompt, i) */

	ldr r0, =inp				/* Arguments for scanf */
	ldr r1, =temp
	bl scanf				/* scanf("%d", nump) */
	ldr r1, =temp
	ldr r1, [r1]
	add r6, r6, r1				/* nump += temp */
	
	ldr r0, =inp				/* Arguments for scanf */
	ldr r1, =temp
	bl scanf				/* scanf("%d", nump) */
	ldr r1, =temp				/* Get temp's value */
	ldr r1, [r1]
	add r7, r7, r1				/* numn += temp */

	ldr r0, =inp				/* Arguments for scanf */
	ldr r1, =temp
	bl scanf				/* scanf("%d", nump) */
	ldr r1, =temp
	ldr r1, [r1]
	add r8, r8, r1				/* numd += temp */

	ldr r0, =inp				/* Arguments for scanf */
	ldr r1, =temp
	bl scanf				/* scanf("%d", nump) */
	ldr r1, =temp
	ldr r1, [r1]
	add r9, r9, r1				/* numq += temp */

	add r5, r5, #1				/* i++ */
	b starta				/* loop up */
enda:	

	/* printf("This amounts to %d, %d, %d, %d, and %d", nump, numn, numd, numq) */
	ldr r0, =out1				/* First argment for printf */
	mov r1, r6				/* nump */
	mov r2, r7				/* numn */
	mov r3, r8				/* numd */
	stmfd sp!, {r9}				/* numq to stack for printf */
	bl printf				/* Make call to printf */
	add sp, #4				/* add 4 bytes from stack pointer */

	/* add pennies to total count */
	mov r10, #0				/* total starts at zero */
	add r10, r10, r6			/* add pennies */

	/* multiply nickles by 5 and add to total */
	mov r0, #0				/* set r0 to 0 */
	add r0, r0, r7				/* add */
	add r0, r0, r7, lsl #2			/* add 4*r7 */
	mov r7, r0				/* save to numn */
	add r10, r10, r7			/* add numn to total */
	
	/* multiply by 5 then shift to get multiply by 10 to get dimes converted to cents */
	mov r0, #0				/* set r0 to 0 */
	add r0, r0, r8				/* add r8 */
	add r0, r0, r8, lsl #2			/* r0 now = r8 *5 */
	lsl r0, #1				/* r0 * 2 => 10 * numd */
	mov r8, r0				/* save to numd */
	add r10, r10, r8				/* add to total */

	/* multiply by 5 two times to get multiply by 25 (convert quarters to cents) */
	mov r0, #0				/* set r0 to 0 */
	add r0, r0, r9				/* add */
	add r0, r0, r9, lsl #2			/* shift and add */
	mov r9, r0
	mov r0, #0				/* start over */
	add r0, r0, r9				/* add */
	add r0, r0, r9, lsl #2			/* shift and add */
	mov r9, r0				/* save to r9 */
	add r10, r10, r9			/* add to total */
	
	/* Get (total / 100) and (total % 100) */
	mov r0, r10				/* first argument */
	bl divide				/* call to divide -- r0 has quotient, r1 has remainder */

	/* pritnf("This comes to $%d.%d", total/100, total%100) */
	mov r2, r1				/* third argument */
	mov r1, r0				/* second argument */
	cmp r2, #10				/* compare cents to 10 */
	ldrge r0, =out2				/* formatted string - If >= 10, we don't need an extra 0 */
	bge call2
	ldrlt r0, =out20				/* blah */
call2:	bl printf

	/* divide total by 4 and split into dollars and cents */
	mov r11, r10				/* copy into temp */
	lsr r11, #2				/* divide by 4 */
	mov r0, r11				/* first argument */
	bl divide				/* call divide */
	
	/* printf("Your weekly average is $%d.%d", temp/100, temp%100) */\
	mov r2, r1				/* cents */
	mov r1, r0				/* dollars */ 
	cmp r2, #10				/* compare cents to 10 */
	ldrge r0, =out3				/* formatted string - If >= 10, we don't need an extra 0 */
	bge call3
	ldrlt r0, =out30			/* blah */
call3:	bl printf

	/* Multiply by 52 = 2 * 26 = 2 * 2 * 13 */
	mov r0, #0				/* result */
	add r0, r0, r10				/* add total */
	add r0, r0, r10, lsl #2			/* shift and add */
	add r0, r0, r10, lsl #1			/* shift again and add */
	lsl r0, #2				/* Multiply by 4 */
	mov r11, r0				/* save to temp */

	/* Split temp into dolalrs and cents */
	mov r0, r11				/* first argument */
	bl divide 				/* call divide */

	/* printf("Your estimated yearly average is $%d.%d", temp/100, temp%100) */
	mov r2, r1				/* remainder into r2 */
	mov r1, r0				/* quotient into r1 */
	cmp r2, #10				/* compare to 10 */
	ldrge r0, =out4				/* formatted string - If >= 10, we don't need an extra 0 */
	bge call4
	ldrlt r0, =out40			/* blah */
call4:	bl printf
	
	ldmfd sp!, {lr}				/* Pop link reg from stack */
	mov r0, #0				/* Load return value */
	mov pc, lr				/* Return from main */

.end
