
	.equ BLOCK_SIZE,(4*1024)
@@@ These are the physical addresses for the I/O devices (after
@@@ the firmware boot code has remapped them).
	.equ	PERI_BASE,  0x20000000
	@@ Base Physical Address of the GPIO registers
	.equ	GPIO_BASE, (PERI_BASE + 0x200000)
	@@ Base Physical Address of the PWM registers
	.equ	PWM_BASE,  (PERI_BASE + 0x20C000)
	@@ Base Physical Address of the Clock/timer registers
	.equ	CLOCK_BASE,(PERI_BASE + 0x101000)
	@@ Base Physical Address of the Pads registers
	.equ	GPIO_PADS, (PERI_BASE + 0x100000)
	@@ Base Physical Address of the SPI0 registers
	.equ	SPI0_BASE, (PERI_BASE + 0x204000)
	@@ Base Physical Address of the BSC0 registers
	.equ	BSC0_BASE, (PERI_BASE + 0x205000)
	@@ Base Physical Address of the BSC1 registers
	.equ	BSC1_BASE, (PERI_BASE + 0x804000)
	@@ Base Physical Address of the System Timer registers
	.equ	ST_BASE,   (PERI_BASE + 0x3000)

	.equ	MAP_FAILED,-1
	.equ	MAP_SHARED, 1
	.equ	PROT_READ,  1
	.equ	PROT_WRITE, 2

	@@ some constants from fcntl.h
	.equ	O_RDONLY,   00000000
	.equ	O_WRONLY,   00000001
	.equ	O_RDWR,	    00000002
	.equ	O_CREAT,    00000100 /* not fcntl */
	.equ	O_EXCL,	    00000200 /* not fcntl */
	.equ	O_NOCTTY,   00000400 /* not fcntl */
	.equ	O_TRUNC,    00001000 /* not fcntl */
	.equ	O_APPEND,   00002000
	.equ	O_NONBLOCK, 00004000
	.equ	O_NDELAY,   O_NONBLOCK
	.equ	O_SYNC,     00010000
	.equ	O_FSYNC,    O_SYNC
	.equ	O_ASYNC,    00020000


@@@ -------------------------------------------------------------------------
	.data
@@@ -------------------------------------------------------------------------
@@@ The following global variables will hold the addresses of
@@@ the devices that you can access directly after you call
@@@ IO_init.
	.global	gpiobase
gpiobase:	.word	0
	.global	pwmbase
pwmbase	:	.word	0
	.global	clkbase
clkbase	:	.word	0
	.global	padsbase
padsbase	:	.word	0
	.global	spi0base
spi0base:	.word	0
	.global	bsc0base
bsc0base:	.word	0
	.global	bsc1base
bsc1base:	.word	0
	.global	stbase
stbase:		.word	0	




memdev:		.asciz "/dev/mem"
successstr:	.asciz "Successfully opened /dev/mem\n"
mappedstr:	.asciz "Mapped %s device at 0x%08X\n"
openfailed:	.asciz	"IO_init: failed to open /dev/mem: "
mapfailedmsg:	.asciz	"IO_init: mmap of %s failed: "
gpiostr:.asciz	"GPIO"
pwmstr:	.asciz	"PWM"
clockstr:.asciz	"CLK"	
padsstr:.asciz	"PADS"	
spi0str:.asciz	"SPI0"	
bsc0str:.asciz	"BSC0"	
bsc1str:.asciz	"BSC1"	
ststr:.asciz	"ST"	

@@@ -------------------------------------------------------------------------
	.text
@@@ -------------------------------------------------------------------------
	.global	IO_init
IO_init:
	stmfd	sp!,{r4,r5,lr}
	@@ Try to open /dev/mem	
	ldr	r0,=memdev	@ load address of "/dev/mem"
	ldr	r1,=(O_RDWR + O_SYNC)
	bl	open		@ call the open syscall
	cmp	r0,#0		@ check result
	bge	init_opened
	ldr	r0,=openfailed	@ print message and exit if open failed
	bl	printf
	bl      __errno_location
        ldr     r0, [r0]
        bl      strerror
        bl      perror
	mov	r5,#0		@ return 0 for failure
	b	init_exit
init_opened:
	@@ Open succeeded.  Now map the devices into our address space
	mov	r4,r0		@ move file descriptor to r4

	ldr	r0,=successstr
	bl	printf

	@@ Map the GPIO device
	ldr	r5,=GPIO_BASE
	bl	trymap
	cmp	r5,#MAP_FAILED
	ldrne	r0,=gpiobase	@ if succeeded, load variable address
	ldreq	r1,=gpiostr	@ if failed, load identifier string
	strne	r5,[r0]		@ if succeeded,	store value
	beq	map_failed_exit	@ if failed, print message and bail out

	ldr	r0,=mappedstr
	ldr	r1,=gpiostr
	ldr	r2,=gpiobase
	ldr	r2,[r2]
	bl	printf

	@@ Map the PWM device
	ldr	r5,=PWM_BASE
	bl	trymap
	cmp	r5,#MAP_FAILED
	ldrne	r0,=pwmbase	@ if succeeded, load variable address
	ldreq	r1,=pwmstr
	strne	r5,[r0]		@ if succeeded,	store value
	beq	map_failed_exit

	ldr	r0,=mappedstr
	ldr	r1,=pwmstr
	ldr	r2,=pwmbase
	ldr	r2,[r2]
	bl	printf

	@@ Map the clock control device (needed for PWM)
	ldr	r5,=CLOCK_BASE
	bl	trymap
	cmp	r5,#MAP_FAILED
	ldrne	r0,=clkbase	@ if succeeded, load variable address
	ldreq	r1,=clockstr
	strne	r5,[r0]		@ if succeeded,	store value
	beq	map_failed_exit

	ldr	r0,=mappedstr
	ldr	r1,=clockstr
	ldr	r2,=clkbase
	ldr	r2,[r2]
	bl	printf

	@@ Map the pads
	ldr	r5,=GPIO_PADS
	bl	trymap
	cmp	r5,#MAP_FAILED
	ldrne	r0,=padsbase	@ if succeeded, load variable address
	ldreq	r1,=padsstr
	strne	r5,[r0]		@ if succeeded,	store value
	beq	map_failed_exit
	
	ldr	r0,=mappedstr
	ldr	r1,=padsstr
	ldr	r2,=padsbase
	ldr	r2,[r2]
	bl	printf

	@@ Map spi0
	ldr	r5,=SPI0_BASE
	bl	trymap
	cmp	r5,#MAP_FAILED
	ldrne	r0,=spi0base	@ if succeeded, load variable address
	ldreq	r1,=spi0str
	strne	r5,[r0]		@ if succeeded,	store value
	beq	map_failed_exit

	ldr	r0,=mappedstr
	ldr	r1,=spi0str
	ldr	r2,=spi0base
	ldr	r2,[r2]
	bl	printf

	@@ Map bsc0 (I2C)
	ldr	r5,=BSC0_BASE
	bl	trymap
	cmp	r5,#MAP_FAILED
	ldrne	r0,=bsc0base	@ if succeeded, load variable address
	ldreq	r1,=bsc0str
	strne	r5,[r0]		@ if succeeded,	store value
	beq	map_failed_exit

	ldr	r0,=mappedstr
	ldr	r1,=bsc0str
	ldr	r2,=bsc0base
	ldr	r2,[r2]
	bl	printf

	@@ Map bsc1
	ldr	r5,=BSC1_BASE
	bl	trymap
	cmp	r5,#MAP_FAILED
	ldrne	r0,=bsc1base	@ if succeeded, load variable address
	ldreq	r1,=bsc1str
	strne	r5,[r0]		@ if succeeded,	store value
	beq	map_failed_exit
	
	ldr	r0,=mappedstr
	ldr	r1,=bsc1str
	ldr	r2,=bsc1base
	ldr	r2,[r2]
	bl	printf

	@@ Map ST device
	ldr	r5,=ST_BASE
	bl	trymap
	cmp	r5,#MAP_FAILED
	ldrne	r0,=stbase	@ if succeeded, load variable address
	ldreq	r1,=ststr
	strne	r5,[r0]		@ if succeeded,	store value
	beq	map_failed_exit
	
	ldr	r0,=mappedstr
	ldr	r1,=ststr
	ldr	r2,=stbase
	ldr	r2,[r2]
	bl	printf

	@@ All mmaps have succeded. Close file and return 1 for success
	mov	r5,#1
	b	init_close

map_failed_exit:
	@@ At least one mmap failed. Print error, unmap everthing and return
	ldr	r0,=mapfailedmsg
	bl	printf
	bl      __errno_location
	ldr     r0, [r0, #0]
	bl      strerror
	bl      perror
	bl	IO_close
	mov	r5,#0
	
init_close:
	mov	r0,r4		@ close /dev/mem
	bl	close

init_exit:	
	mov	r0,r5		@ move return code to r0
	ldmfd	sp!,{r4,r5,pc}	@ return

@@@ -------------------------------------------------------------------------
@@@ Calls mmap.  Does not conform to ARM function call standard.
@@@ parameters are passed in r0-r5.  Return value is in r5.
trymap:	stmfd	sp!,{lr}
	mov	r0,r5		@ request mapping at physical address
	mov	r1,#BLOCK_SIZE
	mov	r2,#(PROT_READ + PROT_WRITE)
	mov	r3,#MAP_SHARED
	stmfd	sp!,{r4,r5}
	bl	mmap
	add	sp,sp,#8
	mov	r5,r0		@ copy return code to r5
	ldmfd	sp!,{pc}

@@@ -------------------------------------------------------------------------
	.global	IO_close
IO_close:
	stmfd	sp!,{r4,r5,lr}
	ldr	r4,=gpiobase
	mov	r5,#7
IO_closeloop:
	ldr	r0,[r4]
	mov	r1,#BLOCK_SIZE
	cmp	r0,#0
	blgt	munmap
	mov	r0,#0
	str	r0,[r4],#4	@ store and increment
	subs	r5,r5,#1
	bgt	IO_closeloop
	ldmfd	sp!,{r4,r5,pc}
