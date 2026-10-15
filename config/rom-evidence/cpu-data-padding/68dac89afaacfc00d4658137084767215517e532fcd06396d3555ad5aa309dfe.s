	.file	1 "/Users/mooki/Code/n64-experiments/projects/shiren2/scratch/omp/wave39/r4-candidate/stage/tree/src/units/main_14400/func_80092E98.cpp"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_80092E98
	.type	 func_80092E98,@function
	.ent	func_80092E98
func_80092E98:
	.frame	$sp,520,$31		# vars= 480, regs= 4/0, args= 24, extra= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,520
	sw	$17,508($sp)
	lw	$17,D_801476B8
	sw	$18,512($sp)
	move	$18,$4
	sw	$31,516($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80048380
	sw	$16,504($sp)
	.set	macro
	.set	reorder

	li	$3,7			# 0x00000007
	addu	$2,$sp,52
.L39:
	sw	$0,0($2)
	addu	$3,$3,-1
	.set	noreorder
	.set	nomacro
	bgez	$3,.L39
	addu	$2,$2,-4
	.set	macro
	.set	reorder

	addu	$16,$sp,56
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	func_80096140
	move	$5,$17
	.set	macro
	.set	reorder

	sw	$0,16($sp)
	move	$4,$16
	addu	$5,$sp,24
	li	$6,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	jal	func_800957C0
	addu	$7,$18,100
	.set	macro
	.set	reorder

	xori	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L43
	move	$2,$0
	.set	macro
	.set	reorder

	la	$3,D_80151F30
	sw	$3,76($16)
	la	$3,D_80151E38
	sw	$3,232($sp)
	.set	noreorder
	.set	nomacro
	j	.L422
	sw	$3,76($16)
	.set	macro
	.set	reorder

.L43:
	.set	noreorder
	.set	nomacro
	jal	func_800D0180
	addu	$4,$sp,480
	.set	macro
	.set	reorder

	lw	$2,24($sp)
	#nop
	addu	$3,$2,-27
	sltu	$2,$3,43
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L44
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lw	$2,.L107($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L107:
	.word	.L87
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L82
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L80
	.word	.L46
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L44
	.word	.L87
	.word	.L81
	.word	.L84
	.word	.L44
	.word	.L45
	.word	.L47
	.text
.L45:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DFD80
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L46:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DFED0
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L47:
	lw	$3,28($sp)
	li	$2,70			# 0x00000046
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L68
	slt	$2,$3,71
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L79
	li	$2,46			# 0x0000002e
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L49
	li	$2,61			# 0x0000003d
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L67
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L79:
	li	$2,71			# 0x00000047
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L50
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L49:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,12			# 0x0000000c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DF740
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L50:
	addu	$16,$sp,488
	move	$4,$16
	la	$17,D_80151350
	la	$2,D_80149F30
	sw	$17,12($16)
	sw	$0,492($sp)
	.set	noreorder
	.set	nomacro
	jal	func_800480A0
	sw	$2,12($16)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800918B0
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800480EC
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L44
	sw	$17,12($16)
	.set	macro
	.set	reorder

.L67:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DFE40
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L68:
	lw	$3,32($sp)
	li	$2,73			# 0x00000049
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L72
	slt	$2,$3,74
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L76
	li	$2,45			# 0x0000002d
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L70
	li	$2,72			# 0x00000048
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L71
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L76:
	li	$2,74			# 0x0000004a
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L73
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L70:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DEEC0
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L71:
	.set	noreorder
	.set	nomacro
	jal	func_80094DAC
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L423
	move	$2,$0
	.set	macro
	.set	reorder

.L72:
	.set	noreorder
	.set	nomacro
	jal	func_80094E10
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L423
	move	$2,$0
	.set	macro
	.set	reorder

.L73:
	.set	noreorder
	.set	nomacro
	jal	func_80094F30
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L423
	move	$2,$0
	.set	macro
	.set	reorder

.L80:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DA010
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L81:
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800D9F20
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L82:
	lw	$3,28($sp)
	li	$2,48			# 0x00000030
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L423
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DFC80
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L84:
	lw	$3,28($sp)
	li	$2,48			# 0x00000030
	bne	$3,$2,.L87
	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,8			# 0x00000008
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800DFC80
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L87:
	lw	$3,32($sp)
	li	$2,67			# 0x00000043
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L88
	addu	$4,$sp,488
	.set	macro
	.set	reorder

	la	$16,D_80141104
	.set	noreorder
	.set	nomacro
	jal	func_80097F20
	move	$5,$16
	.set	macro
	.set	reorder

	lw	$8,488($sp)
	lw	$9,492($sp)
	sw	$8,480($sp)
	sw	$9,484($sp)
	.set	noreorder
	.set	nomacro
	jal	func_800D0248
	addu	$4,$sp,480
	.set	macro
	.set	reorder

	xori	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L423
	move	$2,$0
	.set	macro
	.set	reorder

	lw	$2,40($sp)
	lw	$3,44($sp)
	sw	$2,32($sp)
	.set	noreorder
	.set	nomacro
	j	.L90
	sw	$3,36($sp)
	.set	macro
	.set	reorder

.L88:
	la	$16,D_801404E0
	.set	noreorder
	.set	nomacro
	jal	func_80097F20
	move	$5,$16
	.set	macro
	.set	reorder

	lw	$8,488($sp)
	lw	$9,492($sp)
	sw	$8,480($sp)
	sw	$9,484($sp)
.L90:
	lw	$3,32($sp)
	li	$2,39			# 0x00000027
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L91
	li	$2,21			# 0x00000015
	.set	macro
	.set	reorder

	lw	$2,36($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L423
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,16			# 0x00000010
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	func_800DD970
	addu	$5,$sp,480
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L91:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L93
	addu	$4,$18,100
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_80095CC0
	li	$5,2			# 0x00000002
	.set	macro
	.set	reorder

	lw	$2,484($sp)
	la	$16,D_801408EC
	sw	$2,136($18)
	lw	$2,716($16)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L94
	move	$3,$0
	.set	macro
	.set	reorder

	lw	$2,720($16)
	#nop
	slt	$3,$3,$2
.L94:
	.set	noreorder
	.set	nomacro
	beq	$3,$0,.L93
	li	$2,42			# 0x0000002a
	.set	macro
	.set	reorder

	sw	$2,32($sp)
.L93:
	lw	$5,32($sp)
	li	$2,44			# 0x0000002c
	.set	noreorder
	.set	nomacro
	beq	$5,$2,.L100
	slt	$2,$5,45
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L105
	slt	$2,$5,43
	.set	macro
	.set	reorder

	li	$2,76			# 0x0000004c
	.set	noreorder
	.set	nomacro
	beq	$5,$2,.L102
	addu	$16,$sp,480
	.set	macro
	.set	reorder

	j	.L425
.L105:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L96
	slt	$2,$5,40
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L425
	addu	$16,$sp,480
	.set	macro
	.set	reorder

	move	$4,$18
	addu	$6,$sp,480
	.set	noreorder
	.set	nomacro
	jal	func_80093944
	move	$7,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L100:
	lw	$2,36($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L44
	move	$4,$18
	.set	macro
	.set	reorder

	li	$5,44			# 0x0000002c
	addu	$6,$sp,480
	.set	noreorder
	.set	nomacro
	jal	func_80093944
	move	$7,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L102:
	lw	$4,D_801476B8
	jal	func_800EBA54
	move	$16,$2
	lw	$2,4($16)
	move	$5,$0
	lh	$4,56($2)
	lw	$2,60($2)
	#nop
	.set	noreorder
	.set	nomacro
	jal	$31,$2
	addu	$4,$16,$4
	.set	macro
	.set	reorder

	addu	$17,$sp,488
	move	$4,$17
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	func_800D0190
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_800D8FB0
	li	$4,24			# 0x00000018
	.set	macro
	.set	reorder

	move	$4,$2
	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	func_800DB7DC
	addu	$6,$sp,480
	.set	macro
	.set	reorder

	addu	$4,$sp,56
	la	$3,D_80151F30
	sw	$3,76($4)
	la	$3,D_80151E38
	sw	$3,232($sp)
	.set	noreorder
	.set	nomacro
	j	.L422
	sw	$3,76($4)
	.set	macro
	.set	reorder

.L96:
	addu	$16,$sp,480
.L425:
	.set	noreorder
	.set	nomacro
	jal	func_800D0248
	move	$4,$16
	.set	macro
	.set	reorder

	xori	$2,$2,0x0001
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L423
	move	$2,$0
	.set	macro
	.set	reorder

	move	$4,$18
	lw	$5,32($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80093530
	move	$6,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L424
	addu	$3,$sp,56
	.set	macro
	.set	reorder

.L44:
	move	$2,$0
.L423:
	addu	$3,$sp,56
.L424:
	la	$4,D_80151F30
	sw	$4,76($3)
	la	$4,D_80151E38
	sw	$4,232($sp)
	sw	$4,76($3)
.L422:
	lw	$31,516($sp)
	lw	$18,512($sp)
	lw	$17,508($sp)
	lw	$16,504($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,520
	.set	macro
	.set	reorder

	.end	func_80092E98
.Lfe1:
	.size	 func_80092E98,.Lfe1-func_80092E98
