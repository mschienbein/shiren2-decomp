	.file	1 "/Users/mooki/Code/n64-experiments/projects/shiren2/scratch/omp/wave39/r4-candidate/stage/tree/src/units/overlay_136dc0/func_801F23AC_1442CC.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
	.align	2
.LC0:
	.half	1244
	.half	1261
	.half	1273
	.text
	.align	2
	.globl	func_801F23AC_1442CC
	.type	 func_801F23AC_1442CC,@function
	.ent	func_801F23AC_1442CC
func_801F23AC_1442CC:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, extra= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	subu	$sp,$sp,8
	la	$10,.LC0
	lwl	$7,0($10)
	lwr	$7,3($10)
	lh	$8,4($10)
	swl	$7,0($sp)
	swr	$7,3($sp)
	sh	$8,4($sp)
	li	$5,2			# 0x00000002
	andi	$4,$4,0xffff
	li	$6,-1			# 0xffffffff
	addu	$3,$sp,4
.L5:
	lhu	$2,0($3)
	#nop
	.set	noreorder
	.set	nomacro
	bnel	$4,$2,.L4
	addu	$5,$5,-1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L8
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L4:
	.set	noreorder
	.set	nomacro
	bne	$5,$6,.L5
	addu	$3,$3,-2
	.set	macro
	.set	reorder

	move	$2,$0
.L8:
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,8
	.set	macro
	.set	reorder

	.end	func_801F23AC_1442CC
.Lfe1:
	.size	 func_801F23AC_1442CC,.Lfe1-func_801F23AC_1442CC
