	.cpu arm7tdmi
	.arch armv4t
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"program.c"
	.text
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%d %d\012\000"
	.text
	.align	2
	.global	compute
	.syntax unified
	.arm
	.type	compute, %function
compute:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #40
	str	r0, [fp, #-40]
	str	r1, [fp, #-44]
	ldr	r3, [fp, #-40]
	lsl	r3, r3, #3
	add	r3, r3, #25
	lsr	r2, r3, #31
	add	r3, r2, r3
	asr	r3, r3, #1
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-44]
	ldr	r2, [fp, #-8]
	mul	r3, r2, r3
	str	r3, [fp, #-12]
	ldr	r2, [fp, #-44]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-12]
	and	r3, r3, #3
	ldr	r2, [fp, #-16]
	lsl	r3, r2, r3
	str	r3, [fp, #-20]
	ldr	r2, [fp, #-20]
	ldr	r3, [fp, #-16]
	orr	r3, r2, r3
	ldr	r2, [fp, #-12]
	eor	r3, r3, r2
	str	r3, [fp, #-24]
	ldr	r2, [fp, #-20]
	ldr	r1, [fp, #-16]
	ldr	r0, .L3
	bl	printf
	str	r0, [fp, #-28]
	ldr	r2, [fp, #-28]
	ldr	r1, [fp, #-24]
	ldr	r0, .L3
	bl	printf
	str	r0, [fp, #-32]
	ldr	r3, [fp, #-28]
	ldr	r2, [fp, #-32]
	mul	r3, r2, r3
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
.L4:
	.align	2
.L3:
	.word	.LC0
	.size	compute, .-compute
	.section	.rodata
	.align	2
.LC1:
	.ascii	"%f %f\012\000"
	.text
	.align	2
	.global	main
	.syntax unified
	.arm
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, fp, lr}
	add	fp, sp, #12
	sub	sp, sp, #16
	mov	r1, #42
	mov	r0, #69
	bl	compute
	mov	r3, r0
	mov	r0, r3
	bl	__aeabi_i2f
	mov	r3, r0
	str	r3, [fp, #-16]	@ float
	mov	r1, #69
	mov	r0, #42
	bl	compute
	mov	r3, r0
	mov	r0, r3
	bl	__aeabi_i2f
	mov	r3, r0
	str	r3, [fp, #-20]	@ float
	ldr	r1, [fp, #-20]	@ float
	ldr	r0, [fp, #-16]	@ float
	bl	__aeabi_fadd
	mov	r3, r0
	str	r3, [fp, #-16]	@ float
	ldr	r0, [fp, #-16]	@ float
	bl	__aeabi_f2d
	mov	r4, r0
	mov	r5, r1
	ldr	r0, [fp, #-20]	@ float
	bl	__aeabi_f2d
	mov	r2, r0
	mov	r3, r1
	stm	sp, {r2-r3}
	mov	r2, r4
	mov	r3, r5
	ldr	r0, .L7
	bl	printf
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #12
	@ sp needed
	pop	{r4, r5, fp, lr}
	bx	lr
.L8:
	.align	2
.L7:
	.word	.LC1
	.size	main, .-main
	.global	__aeabi_f2d
	.global	__aeabi_fadd
	.global	__aeabi_i2f
	.ident	"GCC: (Fedora 14.1.0-1.fc40) 14.1.0"
