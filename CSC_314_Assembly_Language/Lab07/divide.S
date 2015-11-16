@@@ ----------------------------------------------------------------
@@@ divide.S
@@@ Author: Larry Pyeatt
@@@ Date: 10/16/2014
@@@ 
@@@ Division functions in ARM assembly language
@@@ ----------------------------------------------------------------

	.text
	.align	2
@@@ -----------------------------------------------------------------
	@@ udiv32 takes a 32-bit unsigned dividend in r0 and
	@@ divides it by a 32-bit unsigned divisor in r1.
	@@ Returns the quotient in r0 and remainder in r1
	@@ It calls no other functions and only 
        @@ uses r0-r3. We don't need to use the stack
	.global	udiv32
udiv32:	cmp	r1,#0		@ if divisor == zero
	beq	quitudiv32	@   exit immediately
	mov     r2,r1           @ move divisor to r2
        mov     r1,r0           @ move dividend to r1
        mov     r0,#0           @ clear r0 to accumulate result
        mov     r3,#1           @ set "current" bit in r3
divstrt:cmp	r2,#0           @ WHILE ((msb of r2 != 1)
	blt	divloop
	cmp     r2,r1		@ && (r2 < r1))
        lslls   r2,r2,#1        @  shift dividend left
        lslls   r3,r3,#1        @  shift "current" bit left
        bls     divstrt		@ end WHILE
divloop:cmp     r1,r2           @ if dividend >= divisor
        subge   r1,r1,r2        @   subtract divisor from dividend
        addge   r0,r0,r3        @   set "current" bit in the result
        lsr	r2,r2,#1	@ shift dividend right
        lsrs    r3,r3,#1	@ Shift current bit right into carry
        bcc     divloop         @ If carry not clear, R3 has shifted
                                @ one bit past where it started, and we
                                @ are done.
quitudiv32:	
        mov     pc,lr

@@@ -----------------------------------------------------------------
	@@ sdiv32 takes a 32-bit signed dividend in r0 and
	@@ divides it by a 32-bit signed divisor in r1.
	@@ Returns the quotient in r0 and remainder in r1
	@@ It calls udiv32 to do the real work
	.global	sdiv32	
sdiv32:	stmfd	sp!,{r4,lr}
	@@ If dividend is negative
	cmp	r0,#0
	rsblt	r0,r0,#0	@ complement it
	movlt	r4,#1		@ and set sign bit for result
	movge	r4,#0		@ else clear sign bit for result
	@@ If divisor is negative
	cmp	r1,#0
	rsblt	r1,r1,#0	@ complement it
	eorlt	r4,#1		@ and complement sign bit for result

	bl 	udiv32

	@@ complement result if needed
	cmp	r4,#0
	rsbne	r0,r0,#0	@ complement it

	ldmfd	sp!,{r4,pc}

@@@ -----------------------------------------------------------------
	@@ udiv64 takes a 64 bit unsigned dividend in r1:r0
	@@ and divides it by a 64 bit unsigned divisor in r3:r2
        @@ Returns a 64-bit result in r1:r0 and
	@@ 64-bit modulus in r3:r2
	.global	udiv64
udiv64:
	@@ check for divisor of zero
	cmp	r2,#0
	cmpeq	r3,#0
	beq	quitudiv64
	stmfd	sp!,{r4-r6}
	mov	r4,r2		@ move divisor to r5:r4
	mov	r5,r3
	mov	r2,r0		@ move dividend to r3:r2
	mov	r3,r1
        mov     r0,#0           @ clear r1:r0 to accumulate result
        mov     r1,#0
        mov     r6,#0           @ set counter to zero
divstrt64:
	@@ shift divisor left until its msb is set, or
	@@   until divisor>=dividend
	cmp	r5,#0		@ is msb of divisor set?
	blt	divloop64	@ end loop if msb of divisor is set
	cmp     r5,r3           @ compare high words
	cmpeq	r4,r2		@ if they are equal, compare low words
	bhs	divloop64	@ end loop if divisor >= dividend (unsigned!)
	lsl	r5,#1		@   shift r5:r4 (divisor) left
	lsls	r4,#1
	orrcs	r5,r5,#1
        add	r6,r6,#1	@   increment count
        b     	divstrt64	@ end WHILE

divloop64:
	lsl	r1,#1		@ shift quotient left
	lsls	r0,#1
	orrcs	r1,#1
	
	cmp     r5,r3           @ compare divisor to dividend
	cmpeq	r4,r2		@ if they are equal, compare low words
	bhi	NoSub		@ IF (divisor<=dividend) Unsigned!
	subs	r2,r2,r4	@   subtract divisor from dividend
	sbc	r3,r3,r5	
	orr	r0,r0,#1	@   set lsb of quotient
	
NoSub:	lsr	r4,#1		@ shift divisor right
	lsrs	r5,#1
	orrcs	r4,#0x80000000
	subs	r6,#1		@ decrement count
	bge	divloop64	@ continue until count is negative

        ldmfd	sp!,{r4-r6}
quitudiv64:	
        mov     pc,lr

@@@ -----------------------------------------------------------------
	@@ sdiv64 takes a 64 bit signed dividend in r1:r0
	@@ and divides it by a 64 bit signed divisor in r3:r2
        @@ Returns a 64-bit result in r1:r0 and
	@@ 64-bit modulus in r3:r2
	.global	sdiv64
sdiv64:
	stmfd	sp!,{r4,lr}
	mov	r4,#0		@ r4 will hold the sign of the result

	@@ Complement dividend if it is negative
	cmp	r1,#0
	bge	NotNeg1
	mvn	r0,r0		@ complement if negative
	mvn	r1,r1
	adds	r0,r0,#1	@ add one to get two's complement
	adc	r1,r1,#0
	eor	r4,r4,#1	@ keep track of sign

NotNeg1:	
	@@ Complement divisor if it is negative
	cmp	r3,#0
	bge	NotNeg2
	mvn	r2,r2		@ complement if negative
	mvn	r3,r3
	adds	r2,r2,#1	@ add one to get two's complement
	adc	r3,r3,#0
	eor	r4,r4,#1	@ keep track of sign
	
NotNeg2:
	bl	udiv64		@ do unsigned division

	@@ Complement result if sign bit is set
	cmp	r4,#0
	beq	NoComplement
	mvn	r0,r0		@ complement if negative
	mvn	r1,r1
	adds	r0,r0,#1	@ add one to get 2's complement
	adc	r1,r1,#0
	
NoComplement:	
        ldmfd	sp!,{r4,pc} 

