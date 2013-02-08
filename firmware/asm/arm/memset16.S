	.arch armv7-a
	.eabi_attribute 27, 3
	.fpu neon
	.eabi_attribute 23, 1
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"d.c"
	.text
	.align	2
	.global	memset16
	.type	memset16, %function
memset16:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	stmfd	sp!, {r4, r5}
	ands	r4, r2, #7
	beq	.L2
	mov	ip, r0
	mov	r3, #0
.L4:
	add	r3, r3, #1
	strh	r1, [ip], #2	@ movhi
	cmp	r4, r3
	bhi	.L4
	add	r0, r0, r4, asl #1
.L2:
	movs	r4, r2, lsr #2
	beq	.L1
	mov	r2, r2, lsr #3
	cmp	r2, #0
	cmpne	r4, #1
	mov	r5, r2, asl #1
	movhi	r3, #0
	movls	r3, #1
	bls	.L13
	vdup.16	q8, r1
	mov	ip, r0
.L8:
	add	r3, r3, #1
	vst1.16	{q8}, [ip]!
	cmp	r2, r3
	bhi	.L8
	cmp	r4, r5
	mov	r3, r5
	add	r0, r0, r5, asl #3
	beq	.L1
.L10:
	vdup.16	d16, r1
.L16:
	add	r3, r3, #1
	vst1.16	{d16}, [r0]!
	cmp	r4, r3
	bgt	.L16
.L1:
	ldmfd	sp!, {r4, r5}
	bx	lr
.L13:
	mov	r3, #0
	b	.L10
	.size	memset16, .-memset16
	.ident	"GCC: (Chomper-4.7.2) 4.7.2"
	.section	.note.GNU-stack,"",%progbits
