	.file	1 "source/src/units/main_14400/func_8004F52C.c"
	.version	"01.01"
gcc2_compiled.:
	.text
	.align	2
	.globl	func_8004F52C
	.type	 func_8004F52C,@function
	.ent	func_8004F52C
func_8004F52C:
	.frame	$sp,72,$31		# vars= 24, regs= 6/0, args= 24, extra= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,72
	move	$8,$5
	sw	$31,68($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	lw	$2,4($8)
	lw	$3,D_8013968C
	sw	$2,24($sp)
	lw	$2,0($8)
	move	$7,$6
	sw	$2,28($sp)
	lw	$2,4($7)
	#nop
	sw	$2,32($sp)
	lw	$2,0($7)
	addu	$3,$3,-184
	sw	$2,36($sp)
	sltu	$2,$3,26
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L25
	move	$20,$4
	.set	macro
	.set	reorder

	sll	$2,$3,2
	lw	$2,.L90($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L90:
	.word	.L89
	.word	.L30
	.word	.L30
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L67
	.word	.L67
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L25
	.word	.L67
	.word	.L30
	.text
.L30:
	li	$2,15			# 0x0000000f
	lbu	$16,0($20)
	lbu	$17,1($20)
	.set	noreorder
	.set	nomacro
	beq	$16,$2,.L25
	move	$5,$0
	.set	macro
	.set	reorder

	slt	$2,$16,16
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L46
	li	$2,5			# 0x00000005
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$16,$2,.L33
	move	$4,$16
	.set	macro
	.set	reorder

	j	.L92
.L46:
	li	$2,19			# 0x00000013
	.set	noreorder
	.set	nomacro
	beq	$16,$2,.L42
	move	$4,$16
	.set	macro
	.set	reorder

	j	.L92
.L33:
	addu	$4,$17,-118
	sltu	$2,$4,5
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L34
	sll	$2,$4,2
	.set	macro
	.set	reorder

	lw	$2,.L40($2)
	#nop
	j	$2
.section	.rodata
	.align	3
	.align	2
.L40:
	.word	.L35
	.word	.L36
	.word	.L37
	.word	.L38
	.word	.L39
	.text
.L35:
	.set	noreorder
	.set	nomacro
	j	.L34
	move	$5,$0
	.set	macro
	.set	reorder

.L36:
	.set	noreorder
	.set	nomacro
	j	.L34
	li	$5,1			# 0x00000001
	.set	macro
	.set	reorder

.L37:
	.set	noreorder
	.set	nomacro
	j	.L34
	li	$5,2			# 0x00000002
	.set	macro
	.set	reorder

.L38:
	.set	noreorder
	.set	nomacro
	j	.L34
	li	$5,3			# 0x00000003
	.set	macro
	.set	reorder

.L39:
	li	$5,4			# 0x00000004
.L34:
	li	$4,51			# 0x00000033
	li.s	$f0,1.00000000000000000000e0
	move	$6,$8
	.set	noreorder
	.set	nomacro
	jal	func_80050CEC
	s.s	$f0,16($sp)
	.set	macro
	.set	reorder

	j	.L25
.L42:
	li	$2,242			# 0x000000f2
	bne	$17,$2,.L92
	lbu	$17,12($20)
	.set	noreorder
	.set	nomacro
	jal	func_800AC1AC
	move	$4,$17
	.set	macro
	.set	reorder

	andi	$16,$2,0x00ff
	move	$4,$16
.L92:
	lw	$6,24($sp)
	lw	$7,28($sp)
	.set	noreorder
	.set	nomacro
	jal	func_8007920C
	move	$5,$17
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	bltz	$19,.L25
	li	$4,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_8007946C
	move	$5,$19
	.set	macro
	.set	reorder

	li	$4,4			# 0x00000004
	move	$5,$19
	li	$6,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	jal	func_80079560
	move	$18,$2
	.set	macro
	.set	reorder

	li	$2,16			# 0x00000010
	.set	noreorder
	.set	nomacro
	bne	$16,$2,.L48
	li	$2,213			# 0x000000d5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$17,$2,.L49
	li	$2,231			# 0x000000e7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$17,$2,.L50
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	j	.L48
.L49:
	lbu	$2,16($20)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L48
	sltu	$3,$0,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L48
	sb	$3,62($18)
	.set	macro
	.set	reorder

.L50:
	lbu	$2,16($20)
	#nop
	andi	$3,$2,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$3,$4,.L54
	sb	$2,40($sp)
	.set	macro
	.set	reorder

	slt	$2,$3,3
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L48
	sb	$0,62($18)
	.set	macro
	.set	reorder

	li	$2,4			# 0x00000004
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L58
	li	$2,6			# 0x00000006
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L60
	li	$2,3			# 0x00000003
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L48
	sb	$0,62($18)
	.set	macro
	.set	reorder

.L54:
	.set	noreorder
	.set	nomacro
	j	.L60
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L58:
	.set	noreorder
	.set	nomacro
	j	.L48
	sb	$4,62($18)
	.set	macro
	.set	reorder

.L60:
	sb	$2,62($18)
.L48:
	la	$4,func_800893D8
	lw	$9,32($sp)
	lw	$10,36($sp)
	sw	$9,16($sp)
	sw	$10,20($sp)
	lw	$6,24($sp)
	lw	$7,28($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80085294
	move	$5,$19
	.set	macro
	.set	reorder

	move	$4,$2
	lw	$3,D_8013968C
	li	$2,186			# 0x000000ba
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L93
	li	$2,209			# 0x000000d1
	.set	macro
	.set	reorder

	lhu	$2,18($4)
	#nop
	ori	$2,$2,0x1000
	sh	$2,18($4)
	lw	$3,D_8013968C
	li	$2,209			# 0x000000d1
.L93:
	bne	$3,$2,.L25
	lhu	$2,18($4)
	#nop
	ori	$2,$2,0x2000
	.set	noreorder
	.set	nomacro
	j	.L25
	sh	$2,18($4)
	.set	macro
	.set	reorder

.L67:
	li	$2,15			# 0x0000000f
	lbu	$3,0($20)
	lbu	$16,1($20)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L25
	move	$18,$3
	.set	macro
	.set	reorder

	li	$2,242			# 0x000000f2
	.set	noreorder
	.set	nomacro
	bne	$16,$2,.L94
	move	$4,$18
	.set	macro
	.set	reorder

	lbu	$16,12($20)
	.set	noreorder
	.set	nomacro
	jal	func_800AC1AC
	move	$4,$16
	.set	macro
	.set	reorder

	andi	$18,$2,0x00ff
	move	$4,$18
.L94:
	lw	$6,24($sp)
	lw	$7,28($sp)
	.set	noreorder
	.set	nomacro
	jal	func_8007920C
	move	$5,$16
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	bltz	$19,.L25
	li	$4,4			# 0x00000004
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	func_8007946C
	move	$5,$19
	.set	macro
	.set	reorder

	li	$4,4			# 0x00000004
	move	$5,$19
	li	$6,1			# 0x00000001
	.set	noreorder
	.set	nomacro
	jal	func_80079560
	move	$17,$2
	.set	macro
	.set	reorder

	li	$2,16			# 0x00000010
	.set	noreorder
	.set	nomacro
	bne	$18,$2,.L71
	li	$2,213			# 0x000000d5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$16,$2,.L72
	li	$2,231			# 0x000000e7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$16,$2,.L73
	li	$4,2			# 0x00000002
	.set	macro
	.set	reorder

	j	.L71
.L72:
	lbu	$2,16($20)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L71
	sltu	$3,$0,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L71
	sb	$3,62($17)
	.set	macro
	.set	reorder

.L73:
	lbu	$2,16($20)
	#nop
	andi	$3,$2,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$3,$4,.L77
	sb	$2,41($sp)
	.set	macro
	.set	reorder

	slt	$2,$3,3
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L71
	sb	$0,62($17)
	.set	macro
	.set	reorder

	li	$2,4			# 0x00000004
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L81
	li	$2,6			# 0x00000006
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L83
	li	$2,3			# 0x00000003
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L71
	sb	$0,62($17)
	.set	macro
	.set	reorder

.L77:
	.set	noreorder
	.set	nomacro
	j	.L83
	li	$2,1			# 0x00000001
	.set	macro
	.set	reorder

.L81:
	.set	noreorder
	.set	nomacro
	j	.L71
	sb	$4,62($17)
	.set	macro
	.set	reorder

.L83:
	sb	$2,62($17)
.L71:
	la	$4,func_800897AC
	lw	$9,32($sp)
	lw	$10,36($sp)
	sw	$9,16($sp)
	sw	$10,20($sp)
	lw	$6,24($sp)
	lw	$7,28($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80085294
	move	$5,$19
	.set	macro
	.set	reorder

	move	$3,$2
	lw	$4,D_8013968C
	li	$2,208			# 0x000000d0
	.set	noreorder
	.set	nomacro
	bne	$4,$2,.L86
	li	$2,193			# 0x000000c1
	.set	macro
	.set	reorder

	lhu	$2,18($3)
	#nop
	ori	$2,$2,0x1000
	.set	noreorder
	.set	nomacro
	j	.L25
	sh	$2,18($3)
	.set	macro
	.set	reorder

.L86:
	bne	$4,$2,.L25
	lhu	$2,18($3)
	#nop
	ori	$2,$2,0x2000
	.set	noreorder
	.set	nomacro
	j	.L25
	sh	$2,18($3)
	.set	macro
	.set	reorder

.L89:
	li	$4,11			# 0x0000000b
	lw	$6,24($sp)
	lw	$7,28($sp)
	.set	noreorder
	.set	nomacro
	jal	func_8007920C
	li	$5,200			# 0x000000c8
	.set	macro
	.set	reorder

	move	$19,$2
	li	$4,4			# 0x00000004
	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	func_80079560
	li	$6,1			# 0x00000001
	.set	macro
	.set	reorder

	la	$4,func_800897AC
	lw	$9,32($sp)
	lw	$10,36($sp)
	sw	$9,16($sp)
	sw	$10,20($sp)
	lw	$6,24($sp)
	lw	$7,28($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80085294
	move	$5,$19
	.set	macro
	.set	reorder

.L25:
	lw	$31,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,72
	.set	macro
	.set	reorder

	.end	func_8004F52C
.Lfe1:
	.size	 func_8004F52C,.Lfe1-func_8004F52C
