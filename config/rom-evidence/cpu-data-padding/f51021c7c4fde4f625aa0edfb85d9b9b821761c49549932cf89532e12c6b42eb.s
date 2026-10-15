	.file	1 "source/src/units/main_14400/func_800AD188.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_800AD188
	.type	 func_800AD188,@function
	.ent	func_800AD188
func_800AD188:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	li	$2,100			# 0x00000064
	andi	$4,$4,0x00ff
	lbu	$3,D_8014303C
	addu	$4,$4,-1
	sb	$2,0($5)
	sll	$2,$3,1
	addu	$2,$2,$3
	sll	$2,$2,2
	subu	$2,$2,$3
	la	$3,D_80156BA0
	addu	$3,$2,$3
	sltu	$2,$4,10
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L13
	sb	$0,0($6)
	.set	macro
	.set	reorder

	sll	$2,$4,2
	lw	$2,.L11($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L11:
	.word	.L3
	.word	.L4
	.word	.L5
	.word	.L6
	.word	.L2
	.word	.L7
	.word	.L8
	.word	.L2
	.word	.L9
	.word	.L10
	.text
.L3:
	lbu	$2,7($3)
	.set	noreorder
	.set	nomacro
	j	$31
	sb	$2,0($5)
	.set	macro
	.set	reorder

.L4:
	lbu	$2,6($3)
	.set	noreorder
	.set	nomacro
	j	$31
	sb	$2,0($5)
	.set	macro
	.set	reorder

.L5:
	lbu	$2,0($3)
	.set	noreorder
	.set	nomacro
	j	$31
	sb	$2,0($6)
	.set	macro
	.set	reorder

.L6:
	lbu	$2,1($3)
	.set	noreorder
	.set	nomacro
	j	$31
	sb	$2,0($6)
	.set	macro
	.set	reorder

.L7:
	lbu	$2,4($3)
	#nop
	sb	$2,0($5)
	lbu	$2,5($3)
	.set	noreorder
	.set	nomacro
	j	$31
	sb	$2,0($6)
	.set	macro
	.set	reorder

.L8:
	lbu	$2,2($3)
	#nop
	sb	$2,0($5)
	lbu	$2,3($3)
	.set	noreorder
	.set	nomacro
	j	$31
	sb	$2,0($6)
	.set	macro
	.set	reorder

.L9:
	lbu	$2,8($3)
	.set	noreorder
	.set	nomacro
	j	$31
	sb	$2,0($5)
	.set	macro
	.set	reorder

.L10:
	lbu	$2,10($3)
	#nop
	sb	$2,0($5)
.L2:
.L13:
	j	$31
	.end	func_800AD188
.Lfe1:
	.size	 func_800AD188,.Lfe1-func_800AD188
