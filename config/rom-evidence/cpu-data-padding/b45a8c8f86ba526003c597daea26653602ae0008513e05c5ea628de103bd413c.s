	.file	1 "/Users/mooki/Code/n64-experiments/projects/shiren2/scratch/omp/wave39/r4-candidate/stage/tree/src/units/overlay_136dc0/func_801F2888_1447A8.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_801F2888_1447A8
	.type	 func_801F2888_1447A8,@function
	.ent	func_801F2888_1447A8
func_801F2888_1447A8:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, extra= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,32
	sw	$17,20($sp)
	move	$17,$4
	sw	$16,16($sp)
	andi	$16,$17,0x00ff
	sw	$31,24($sp)
	.set	noreorder
	.set	nomacro
	jal	func_801F2E30_144D50
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L31
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800B74C0
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L31
	move	$2,$0
	.set	macro
	.set	reorder

	lbu	$2,D_801F5228_147148+2
	#nop
	sltu	$2,$2,253
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L32
	andi	$2,$17,0x00ff
	.set	macro
	.set	reorder

	lbu	$2,D_80142F18+3
	#nop
	srl	$2,$2,2
	andi	$2,$2,0x0001
	xori	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L31
	move	$2,$0
	.set	macro
	.set	reorder

	andi	$2,$17,0x00ff
.L32:
	addu	$3,$2,-24
	sltu	$2,$3,5
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L10
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lw	$2,.L29($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L29:
	.word	.L17
	.word	.L26
	.word	.L11
	.word	.L13
	.word	.L15
	.text
.L11:
	.set	noreorder
	.set	nomacro
	jal	func_801E9F70_13BE90
	li	$4,37			# 0x00000025
	.set	macro
	.set	reorder

	andi	$2,$2,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L10
	li	$2,276			# 0x00000114
	.set	macro
	.set	reorder

	j	.L31
.L13:
	.set	noreorder
	.set	nomacro
	jal	func_801E9F70_13BE90
	li	$4,35			# 0x00000023
	.set	macro
	.set	reorder

	andi	$2,$2,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L10
	li	$2,271			# 0x0000010f
	.set	macro
	.set	reorder

	j	.L31
.L15:
	.set	noreorder
	.set	nomacro
	jal	func_801E9F70_13BE90
	li	$4,36			# 0x00000024
	.set	macro
	.set	reorder

	andi	$2,$2,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L10
	li	$2,273			# 0x00000111
	.set	macro
	.set	reorder

	j	.L31
.L17:
	lbu	$3,D_801F5228_147148+2
	#nop
	sltu	$2,$3,100
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L31
	move	$2,$0
	.set	macro
	.set	reorder

	sltu	$2,$3,166
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L34
	sltu	$2,$3,253
	.set	macro
	.set	reorder

	sltu	$2,$3,100
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L31
	move	$2,$0
	.set	macro
	.set	reorder

	sltu	$2,$3,253
.L34:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L31
	li	$2,278			# 0x00000116
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801E9F70_13BE90
	li	$4,41			# 0x00000029
	.set	macro
	.set	reorder

	andi	$2,$2,0x00ff
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L31
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L31
	li	$2,278			# 0x00000116
	.set	macro
	.set	reorder

.L26:
	lbu	$3,D_801F5228_147148+2
	li	$2,220			# 0x000000dc
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L10
	sltu	$3,$3,160
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$0,.L31
	li	$2,287			# 0x0000011f
	.set	macro
	.set	reorder

.L10:
	move	$2,$0
.L31:
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

	.end	func_801F2888_1447A8
.Lfe1:
	.size	 func_801F2888_1447A8,.Lfe1-func_801F2888_1447A8
