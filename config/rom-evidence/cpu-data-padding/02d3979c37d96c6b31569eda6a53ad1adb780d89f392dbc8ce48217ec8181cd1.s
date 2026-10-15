	.file	1 "source/src/units/main_14400/func_800EA67C.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_800EA67C
	.type	 func_800EA67C,@function
	.ent	func_800EA67C
func_800EA67C:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, extra= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,24
	move	$6,$5
	sw	$31,16($sp)
	lw	$2,0($6)
	#nop
	addu	$3,$2,-2
	sltu	$2,$3,23
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L14
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lw	$2,.L15($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L15:
	.word	.L3
	.word	.L14
	.word	.L4
	.word	.L5
	.word	.L14
	.word	.L14
	.word	.L6
	.word	.L7
	.word	.L14
	.word	.L14
	.word	.L14
	.word	.L14
	.word	.L14
	.word	.L14
	.word	.L14
	.word	.L14
	.word	.L14
	.word	.L8
	.word	.L9
	.word	.L10
	.word	.L11
	.word	.L12
	.word	.L13
	.text
.L3:
	.set	noreorder
	.set	nomacro
	jal	func_800EA784
	move	$5,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L4:
	jal	func_800EA7D4
	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L5:
	.set	noreorder
	.set	nomacro
	jal	func_800EA80C
	move	$5,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L6:
	.set	noreorder
	.set	nomacro
	jal	func_800EA8FC
	move	$5,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L7:
	.set	noreorder
	.set	nomacro
	jal	func_800EA998
	move	$5,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L8:
	lw	$2,20($6)
	lw	$5,4($6)
	lbu	$7,0($2)
	lw	$6,16($6)
	.set	noreorder
	.set	nomacro
	jal	func_800EAAC0
	sll	$7,$7,24
	.set	macro
	.set	reorder

	j	.L16
.L9:
	.set	noreorder
	.set	nomacro
	jal	func_800EAC8C
	move	$5,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L10:
	.set	noreorder
	.set	nomacro
	jal	func_800EAE8C
	move	$5,$6
	.set	macro
	.set	reorder

	j	.L16
.L11:
	.set	noreorder
	.set	nomacro
	jal	func_800EAF38
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L12:
	.set	noreorder
	.set	nomacro
	jal	func_800EAF7C
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	j	.L16
.L13:
	.set	noreorder
	.set	nomacro
	jal	func_800EAFB8
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L14:
	.set	noreorder
	.set	nomacro
	jal	func_800E4B60
	move	$5,$6
	.set	macro
	.set	reorder

.L16:
	lw	$31,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	func_800EA67C
.Lfe1:
	.size	 func_800EA67C,.Lfe1-func_800EA67C
