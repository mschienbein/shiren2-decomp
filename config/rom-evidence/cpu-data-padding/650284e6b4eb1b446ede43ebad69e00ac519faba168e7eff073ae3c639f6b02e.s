	.file	1 "/Users/mooki/Code/n64-experiments/projects/shiren2/scratch/omp/wave39/r4-candidate/stage/tree/src/units/overlay_136dc0/func_801E976C_13B68C.cpp"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_801E976C_13B68C
	.type	 func_801E976C_13B68C,@function
	.ent	func_801E976C_13B68C
func_801E976C_13B68C:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, extra= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,24
	sw	$16,16($sp)
	move	$16,$0
	sltu	$2,$4,8
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L2
	sw	$31,20($sp)
	.set	macro
	.set	reorder

	sll	$2,$4,2
	lw	$2,.L12($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L12:
	.word	.L3
	.word	.L4
	.word	.L6
	.word	.L7
	.word	.L8
	.word	.L9
	.word	.L10
	.word	.L11
	.text
.L3:
	.set	noreorder
	.set	nomacro
	j	.L15
	li	$4,193			# 0x000000c1
	.set	macro
	.set	reorder

.L4:
	.set	noreorder
	.set	nomacro
	jal	func_801E9F70_13BE90
	li	$4,191			# 0x000000bf
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L5
	li	$4,192			# 0x000000c0
	.set	macro
	.set	reorder

.L6:
	.set	noreorder
	.set	nomacro
	j	.L15
	li	$4,194			# 0x000000c2
	.set	macro
	.set	reorder

.L7:
	.set	noreorder
	.set	nomacro
	jal	func_801E9F70_13BE90
	li	$4,195			# 0x000000c3
	.set	macro
	.set	reorder

	li	$4,196			# 0x000000c4
.L5:
	.set	noreorder
	.set	nomacro
	jal	func_801E9F70_13BE90
	move	$16,$2
	.set	macro
	.set	reorder

	andi	$2,$2,0x00ff
	sll	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	.L2
	addu	$16,$16,$2
	.set	macro
	.set	reorder

.L8:
	.set	noreorder
	.set	nomacro
	j	.L15
	li	$4,197			# 0x000000c5
	.set	macro
	.set	reorder

.L9:
	.set	noreorder
	.set	nomacro
	j	.L15
	li	$4,198			# 0x000000c6
	.set	macro
	.set	reorder

.L10:
	.set	noreorder
	.set	nomacro
	j	.L15
	li	$4,199			# 0x000000c7
	.set	macro
	.set	reorder

.L11:
	li	$4,164			# 0x000000a4
.L15:
	jal	func_801E9F70_13BE90
	move	$16,$2
.L2:
	andi	$2,$16,0x00ff
	lw	$31,20($sp)
	lw	$16,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	func_801E976C_13B68C
.Lfe1:
	.size	 func_801E976C_13B68C,.Lfe1-func_801E976C_13B68C
