	.file	1 "source/src/units/main_14400/func_800ED448.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_800ED448
	.type	 func_800ED448,@function
	.ent	func_800ED448
func_800ED448:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, extra= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,32
	sw	$31,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lw	$3,0($5)
	#nop
	sltu	$2,$3,25
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L20
	move	$16,$4
	.set	macro
	.set	reorder

	sll	$2,$3,2
	lw	$2,.L21($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L21:
	.word	.L3
	.word	.L7
	.word	.L8
	.word	.L20
	.word	.L9
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L11
	.word	.L11
	.word	.L16
	.word	.L20
	.word	.L15
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L20
	.word	.L17
	.word	.L18
	.word	.L20
	.word	.L19
	.text
.L3:
	lw	$2,260($16)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L4
	move	$17,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800E8694
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L5
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800E1CC4
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	sltu	$17,$2,1
.L5:
	.set	noreorder
	.set	nomacro
	beq	$17,$0,.L6
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800E8350
	move	$4,$16
	.set	macro
	.set	reorder

	j	.L22
.L6:
	lw	$5,260($4)
	.set	noreorder
	.set	nomacro
	jal	func_800E74E0
	li	$6,3			# 0x00000003
	.set	macro
	.set	reorder

	j	.L22
.L4:
	.set	noreorder
	.set	nomacro
	j	.L22
	move	$2,$0
	.set	macro
	.set	reorder

.L7:
	.set	noreorder
	.set	nomacro
	jal	func_800ED5FC
	move	$4,$16
	.set	macro
	.set	reorder

	j	.L22
.L8:
	.set	noreorder
	.set	nomacro
	jal	func_800ED72C
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L22
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L9:
	.set	noreorder
	.set	nomacro
	jal	func_800EDA7C
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L22
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L11:
	lbu	$2,D_80142F18+8
	#nop
	xori	$2,$2,0x004f
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L22
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

	lw	$4,16($5)
	#nop
	lw	$2,0($4)
	sw	$0,D_80148090
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L13
	move	$6,$0
	.set	macro
	.set	reorder

	lbu	$3,10($2)
	li	$2,45			# 0x0000002d
	bne	$3,$2,.L13
	lbu	$2,31($16)
	lbu	$3,10($16)
	#nop
	xor	$2,$2,$3
	sltu	$6,$2,1
.L13:
	beq	$6,$0,.L14
	lhu	$2,14($4)
	#nop
	ori	$2,$2,0x4000
	sh	$2,14($4)
.L14:
	.set	noreorder
	.set	nomacro
	jal	func_800EA67C
	move	$4,$16
	.set	macro
	.set	reorder

	li	$4,137			# 0x00000089
	.set	noreorder
	.set	nomacro
	jal	func_80049CB4
	move	$5,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L22
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L15:
	.set	noreorder
	.set	nomacro
	jal	func_800EDAE8
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L22
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L16:
	.set	noreorder
	.set	nomacro
	jal	func_800EDB04
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L22
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L17:
	.set	noreorder
	.set	nomacro
	jal	func_800EDF50
	move	$4,$16
	.set	macro
	.set	reorder

	j	.L22
.L18:
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800EDF78
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L22
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L19:
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_800EDFF0
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L22
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L20:
	.set	noreorder
	.set	nomacro
	jal	func_800EA67C
	move	$4,$16
	.set	macro
	.set	reorder

.L22:
	lw	$31,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	func_800ED448
.Lfe1:
	.size	 func_800ED448,.Lfe1-func_800ED448
