	.file	1 "source/src/units/main_14400/func_800CC934.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
	.align	2
.LC0:
	.string	"Fatal Error Ranking Data Normaly Open\n"
	.text
	.align	2
	.globl	func_800CC934
	.type	 func_800CC934,@function
	.ent	func_800CC934
func_800CC934:
	.frame	$sp,56,$31		# vars= 0, regs= 10/0, args= 16, extra= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,56
	sw	$23,44($sp)
	move	$23,$5
	andi	$4,$4,0x00ff
	sw	$31,52($sp)
	sw	$fp,48($sp)
	sw	$22,40($sp)
	sw	$21,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	.set	noreorder
	.set	nomacro
	jal	func_800CB510
	sw	$16,16($sp)
	.set	macro
	.set	reorder

	move	$22,$2
	sll	$2,$22,24
	sra	$2,$2,24
	li	$3,-1			# 0xffffffff
	beq	$2,$3,.L1
	jal	func_800CBD2C
	move	$16,$2
	addu	$2,$16,-1
	sltu	$2,$2,2
	bne	$2,$0,.L4
	jal	func_800CBE44
	xori	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L27
	li	$2,3			# 0x00000003
	.set	macro
	.set	reorder

.L4:
	jal	func_800CC16C
	jal	func_800CC248
	jal	func_800CBD2C
	move	$16,$2
	li	$2,3			# 0x00000003
.L27:
	beq	$16,$2,.L5
	la	$4,.LC0
	jal	func_80136964
	j	.L6
.L5:
	la	$16,D_801476D8
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800CBFC0
	andi	$5,$22,0x00ff
	.set	macro
	.set	reorder

	li	$19,-1			# 0xffffffff
	move	$17,$0
	la	$18,D_80147EA8
.L7:
	slt	$2,$17,30
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L28
	li	$2,-1			# 0xffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800CBCF8
	andi	$4,$17,0x00ff
	.set	macro
	.set	reorder

	addu	$4,$2,$16
	slt	$2,$17,10
	beq	$2,$0,.L11
	la	$5,D_80147EA8
	jal	func_800CB96C
	j	.L12
.L11:
	la	$5,D_80147EA8
	jal	func_800CBC24
.L12:
	.set	noreorder
	.set	nomacro
	jal	func_800CC6E4
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L8
	move	$19,$17
	.set	macro
	.set	reorder

	lw	$4,12($23)
	lw	$3,12($18)
	#nop
	sltu	$2,$3,$4
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L8
	move	$19,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bnel	$4,$3,.L7
	addu	$17,$17,1
	.set	macro
	.set	reorder

	lw	$2,16($23)
	lw	$3,16($18)
	#nop
	sltu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L7
	addu	$17,$17,1
	.set	macro
	.set	reorder

	move	$19,$17
.L8:
	li	$2,-1			# 0xffffffff
.L28:
	.set	noreorder
	.set	nomacro
	beq	$19,$2,.L6
	li	$16,29			# 0x0000001d
	.set	macro
	.set	reorder

	la	$21,D_801476D8
	li	$fp,9			# 0x00000009
	la	$20,D_80147EA8
	move	$2,$16
.L26:
	slt	$2,$19,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L17
	addu	$16,$16,-1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800CBCF8
	andi	$4,$16,0x00ff
	.set	macro
	.set	reorder

	addu	$4,$16,1
	andi	$4,$4,0x00ff
	.set	noreorder
	.set	nomacro
	jal	func_800CBCF8
	addu	$17,$2,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$fp,.L20
	addu	$18,$2,$21
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	func_800CB96C
	move	$5,$20
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	func_800CB884
	move	$5,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L26
	move	$2,$16
	.set	macro
	.set	reorder

.L20:
	slt	$2,$16,9
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L22
	li	$6,20			# 0x00000014
	.set	macro
	.set	reorder

	li	$6,144			# 0x00000090
.L22:
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	func_80032D94
	move	$5,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L26
	move	$2,$16
	.set	macro
	.set	reorder

.L17:
	.set	noreorder
	.set	nomacro
	jal	func_800CBCF8
	andi	$4,$19,0x00ff
	.set	macro
	.set	reorder

	la	$3,D_801476D8
	addu	$5,$2,$3
	slt	$2,$19,10
	beq	$2,$0,.L24
	.set	noreorder
	.set	nomacro
	jal	func_800CB618
	move	$4,$23
	.set	macro
	.set	reorder

	j	.L25
.L24:
	.set	noreorder
	.set	nomacro
	jal	func_800CB884
	move	$4,$23
	.set	macro
	.set	reorder

.L25:
	la	$4,D_801476D8
	.set	noreorder
	.set	nomacro
	jal	func_800CC0E8
	andi	$5,$22,0x00ff
	.set	macro
	.set	reorder

.L6:
	jal	func_800CC248
.L1:
	lw	$31,52($sp)
	lw	$fp,48($sp)
	lw	$23,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,56
	.set	macro
	.set	reorder

	.end	func_800CC934
.Lfe1:
	.size	 func_800CC934,.Lfe1-func_800CC934
