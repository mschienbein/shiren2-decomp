	.file	1 "source/src/units/main_14400/func_800CC0E8.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
	.align	2
.LC0:
	.string	"Ranking::_fwriteRawDungeon() cannot write\n"
	.text
	.align	2
	.globl	func_800CC0E8
	.type	 func_800CC0E8,@function
	.ent	func_800CC0E8
func_800CC0E8:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, extra= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,24
	sw	$16,16($sp)
	move	$16,$4
	andi	$4,$5,0x00ff
	sw	$31,20($sp)
	.set	noreorder
	.set	nomacro
	jal	func_800CBEBC
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800CBE18
	li	$4,1840			# 0x00000730
	.set	macro
	.set	reorder

	xori	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L2
	li	$5,1840			# 0x00000730
	.set	macro
	.set	reorder

	la	$4,.LC0
	jal	func_80136964
	j	.L3
.L2:
	lw	$2,D_80147F44
	#nop
	lw	$3,24($2)
	#nop
	lh	$4,24($3)
	#nop
	addu	$4,$2,$4
	lw	$2,28($3)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	move	$6,$16
	.set	macro
	.set	reorder

	lw	$4,D_80147F44
	jal	func_800CA2C0
.L3:
	lw	$31,20($sp)
	lw	$16,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	func_800CC0E8
.Lfe1:
	.size	 func_800CC0E8,.Lfe1-func_800CC0E8
