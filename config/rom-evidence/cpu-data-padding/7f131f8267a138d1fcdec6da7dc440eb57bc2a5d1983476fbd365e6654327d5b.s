	.file	1 "source/src/units/main_14400/func_8009CE64.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_8009CE64
	.type	 func_8009CE64,@function
	.ent	func_8009CE64
func_8009CE64:
	.frame	$sp,56,$31		# vars= 8, regs= 4/0, args= 32, extra= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,56
	sw	$17,44($sp)
	move	$17,$4
	sll	$2,$5,1
	addu	$2,$2,$5
	sll	$2,$2,2
	addu	$2,$17,$2
	sw	$31,52($sp)
	sw	$18,48($sp)
	sw	$16,40($sp)
	lw	$3,100($2)
	#nop
	sltu	$2,$3,9
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L2
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lw	$2,.L24($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L24:
	.word	.L11
	.word	.L12
	.word	.L3
	.word	.L11
	.word	.L21
	.word	.L11
	.word	.L2
	.word	.L18
	.word	.L22
	.text
.L3:
	lw	$2,364($17)
	#nop
	.set	noreorder
	.set	nomacro
	blez	$2,.L5
	move	$3,$0
	.set	macro
	.set	reorder

	move	$4,$17
.L7:
	sw	$0,736($4)
	lw	$2,364($17)
	addu	$3,$3,1
	slt	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L7
	addu	$4,$4,12
	.set	macro
	.set	reorder

.L5:
	sw	$0,728($17)
.L11:
	sll	$3,$5,1
	addu	$3,$3,$5
	sll	$3,$3,2
	addu	$3,$17,$3
	lw	$3,100($3)
	addu	$2,$17,368
	.set	noreorder
	.set	nomacro
	j	.L26
	sw	$3,88($2)
	.set	macro
	.set	reorder

.L12:
	lw	$2,364($17)
	#nop
	.set	noreorder
	.set	nomacro
	blez	$2,.L14
	move	$3,$0
	.set	macro
	.set	reorder

	move	$4,$17
.L16:
	sw	$0,736($4)
	lw	$2,364($17)
	addu	$3,$3,1
	slt	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L16
	addu	$4,$4,12
	.set	macro
	.set	reorder

.L14:
	.set	noreorder
	.set	nomacro
	j	.L26
	addu	$2,$17,756
	.set	macro
	.set	reorder

.L18:
	la	$5,D_80141D9C
	la	$6,D_80138F20
	la	$7,D_80138F30
	.set	noreorder
	.set	nomacro
	jal	func_80097240
	addu	$4,$17,204
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80045D00
	li	$16,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$16,.L19
	addu	$4,$sp,32
	.set	macro
	.set	reorder

	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	func_8006A810
	li	$6,8			# 0x00000008
	.set	macro
	.set	reorder

	j	.L20
.L19:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	func_8006A810
	li	$6,8			# 0x00000008
	.set	macro
	.set	reorder

	sw	$16,32($sp)
.L20:
	lw	$8,32($sp)
	lw	$9,36($sp)
	sw	$8,264($17)
	sw	$9,268($17)
	.set	noreorder
	.set	nomacro
	j	.L26
	addu	$2,$17,204
	.set	macro
	.set	reorder

.L21:
	la	$16,D_801425F0
	.set	noreorder
	.set	nomacro
	jal	func_8009EDE0
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L26
	move	$2,$16
	.set	macro
	.set	reorder

.L22:
	.set	noreorder
	.set	nomacro
	jal	func_8009D068
	move	$4,$17
	.set	macro
	.set	reorder

	move	$18,$2
	li	$2,-1			# 0xffffffff
	.set	noreorder
	.set	nomacro
	bne	$18,$2,.L23
	move	$4,$17
	.set	macro
	.set	reorder

	jal	func_801E63C4
	.set	noreorder
	.set	nomacro
	j	.L26
	move	$2,$0
	.set	macro
	.set	reorder

.L23:
	lw	$6,360($17)
	.set	noreorder
	.set	nomacro
	jal	func_8009D198
	move	$5,$18
	.set	macro
	.set	reorder

	la	$16,D_80141DB4
	move	$4,$16
	addu	$5,$17,944
	la	$6,D_80138F38
	la	$7,D_80138F48
	sw	$2,0($6)
	.set	noreorder
	.set	nomacro
	jal	func_80097240
	sw	$2,0($7)
	.set	macro
	.set	reorder

	move	$4,$16
	addu	$5,$17,848
	move	$6,$18
	la	$7,D_80138F50
	addu	$2,$17,296
	sw	$2,16($sp)
	lw	$3,360($17)
	la	$2,D_80138F08
	sw	$2,24($sp)
	.set	noreorder
	.set	nomacro
	jal	func_801E5F0C
	sw	$3,20($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L26
	move	$2,$16
	.set	macro
	.set	reorder

.L2:
	move	$2,$0
.L26:
	lw	$31,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,56
	.set	macro
	.set	reorder

	.end	func_8009CE64
.Lfe1:
	.size	 func_8009CE64,.Lfe1-func_8009CE64
