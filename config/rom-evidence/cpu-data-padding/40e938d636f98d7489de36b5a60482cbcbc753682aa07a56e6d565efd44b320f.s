	.file	1 "/Users/mooki/Code/n64-experiments/projects/shiren2/scratch/omp/wave39/r4-candidate/stage/tree/src/units/overlay_136dc0/func_801EB728_13D648.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_801EB728_13D648
	.type	 func_801EB728_13D648,@function
	.ent	func_801EB728_13D648
func_801EB728_13D648:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, extra= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	lbu	$2,D_801F5CCD_147BED
	subu	$sp,$sp,32
	sw	$31,28($sp)
	addu	$3,$2,-82
	sltu	$2,$3,14
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L14
	sw	$16,24($sp)
	.set	macro
	.set	reorder

	sll	$2,$3,2
	lw	$2,.L15($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L15:
	.word	.L8
	.word	.L6
	.word	.L14
	.word	.L14
	.word	.L14
	.word	.L9
	.word	.L14
	.word	.L14
	.word	.L7
	.word	.L12
	.word	.L14
	.word	.L14
	.word	.L13
	.word	.L3
	.text
.L3:
	lw	$3,D_801F51E8_147108
	#nop
	lw	$2,268($3)
	#nop
	bne	$2,$0,.L17
	lhu	$2,266($3)
	#nop
	sh	$2,288($3)
	lw	$3,D_801F51E8_147108
.L17:
	lw	$2,272($3)
	#nop
	beq	$2,$0,.L5
	lhu	$2,288($3)
	#nop
	ori	$2,$2,0x8000
	sh	$2,288($3)
.L5:
	lw	$3,D_801F51E8_147108
	lbu	$2,D_801F5CCC_147BEC
	.set	noreorder
	.set	nomacro
	j	.L2
	sb	$2,2549($3)
	.set	macro
	.set	reorder

.L6:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$4,264($2)
	.set	noreorder
	.set	nomacro
	jal	func_80041434
	sltu	$4,$0,$4
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
	jal	func_801EB6A0_13D5C0
	li	$4,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,3			# 0x00000003
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	#nop
	lw	$4,268($2)
	lw	$5,272($2)
	lw	$6,276($2)
	jal	func_801E8634_13A554
	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L8:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$4,264($2)
	jal	func_80045CA0
	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L9:
	lw	$3,D_801F51EC_14710C
	#nop
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	andi	$2,$2,0x00ff
	bne	$2,$0,.L10
	lw	$2,D_801F51E8_147108
	#nop
	lw	$4,268($2)
	lw	$5,272($2)
	lw	$6,276($2)
	jal	func_801E8758_13A678
	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L10:
	lw	$2,D_801F51E8_147108
	#nop
	lw	$5,272($2)
	lw	$4,268($2)
	.set	noreorder
	.set	nomacro
	jal	func_801E8AB8_13A9D8
	sltu	$5,$5,1
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
	jal	func_801EB6A0_13D5C0
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	lw	$2,D_801F51E8_147108
	#nop
	l.s	$f0,272($2)
	cvt.s.w	$f0,$f0
	lw	$4,264($2)
	lw	$5,268($2)
	mfc1	$6,$f0
	jal	func_801E817C_13A09C
	.set	noreorder
	.set	nomacro
	j	.L16
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L13:
	lw	$3,D_801F51EC_14710C
	#nop
	lw	$2,4($3)
	#nop
	move	$4,$2
	addu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	func_801E53A4_1372C4
	sw	$2,4($3)
	.set	macro
	.set	reorder

	move	$4,$0
	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	andi	$16,$2,0x00ff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,1			# 0x00000001
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_801EB6A0_13D5C0
	li	$4,3			# 0x00000003
	.set	macro
	.set	reorder

	lw	$3,D_801F51E8_147108
	#nop
	lhu	$2,266($3)
	#nop
	sh	$2,16($sp)
	lhu	$2,270($3)
	#nop
	sh	$2,18($sp)
	lhu	$2,274($3)
	addu	$4,$sp,16
	sh	$2,20($sp)
	lhu	$2,278($3)
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	func_801E830C_13A22C
	sh	$2,22($sp)
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
	j	.L16
	move	$2,$0
	.set	macro
	.set	reorder

.L2:
	li	$2,1			# 0x00000001
.L16:
	lw	$31,28($sp)
	lw	$16,24($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	func_801EB728_13D648
.Lfe1:
	.size	 func_801EB728_13D648,.Lfe1-func_801EB728_13D648
