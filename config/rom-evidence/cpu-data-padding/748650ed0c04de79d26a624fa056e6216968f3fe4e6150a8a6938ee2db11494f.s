	.file	1 "source/src/units/main_14400/func_8008B8A8.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_8008B8A8
	.type	 func_8008B8A8,@function
	.ent	func_8008B8A8
func_8008B8A8:
	.frame	$sp,40,$31		# vars= 0, regs= 4/0, args= 24, extra= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,40
	sw	$16,24($sp)
	move	$16,$4
	sw	$17,28($sp)
	move	$17,$6
	sw	$18,32($sp)
	sw	$31,36($sp)
	.set	noreorder
	.set	nomacro
	jal	func_800751B4
	move	$18,$7
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_80075E5C
	move	$5,$17
	.set	macro
	.set	reorder

	move	$4,$0
	.set	noreorder
	.set	nomacro
	jal	func_8007946C
	move	$5,$16
	.set	macro
	.set	reorder

	move	$3,$2
	sltu	$2,$17,8
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L13
	sll	$2,$17,2
	.set	macro
	.set	reorder

	lw	$2,.L14($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L14:
	.word	.L13
	.word	.L5
	.word	.L3
	.word	.L13
	.word	.L13
	.word	.L13
	.word	.L13
	.word	.L13
	.text
.L3:
	lh	$3,2($3)
	li	$2,23			# 0x00000017
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L16
	li	$2,3			# 0x00000003
	.set	macro
	.set	reorder

	li	$2,1			# 0x00000001
	sw	$2,16($sp)
	move	$4,$16
	move	$5,$18
	move	$6,$2
	.set	noreorder
	.set	nomacro
	j	.L15
	li	$7,36			# 0x00000024
	.set	macro
	.set	reorder

.L5:
	lh	$3,2($3)
	li	$2,23			# 0x00000017
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L16
	li	$2,3			# 0x00000003
	.set	macro
	.set	reorder

	sw	$0,16($sp)
	move	$4,$16
	move	$5,$18
	li	$6,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	j	.L15
	li	$7,34			# 0x00000022
	.set	macro
	.set	reorder

.L13:
	li	$2,3			# 0x00000003
.L16:
	sw	$2,16($sp)
	move	$4,$16
	move	$5,$18
	li	$6,1			# 0x00000001
	li	$7,8			# 0x00000008
.L15:
	jal	func_80076044
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,40
	.set	macro
	.set	reorder

	.end	func_8008B8A8
.Lfe1:
	.size	 func_8008B8A8,.Lfe1-func_8008B8A8
