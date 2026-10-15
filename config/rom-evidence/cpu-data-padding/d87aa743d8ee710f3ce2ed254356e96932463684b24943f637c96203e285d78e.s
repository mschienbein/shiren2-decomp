	.file	1 "source/src/units/main_14400/func_8005E714.c"
	.version	"01.01"
gcc2_compiled.:
	.data
	.align	2
	.type	 D_8013B62C,@object
	.size	 D_8013B62C,21
D_8013B62C:
	.string	"                    "
	.align	2
	.type	 D_8013B644,@object
	.size	 D_8013B644,21
D_8013B644:
	.string	"00000000000000000000"
.section	.rodata
	.align	2
	.type	 D_8014C3E0.2,@object
D_8014C3E0.2:
	.byte	32
	.byte	43
	.byte	45
	.byte	35
	.byte	48
	.byte	0
	.align	2
	.type	 D_8014C3E8.3,@object
D_8014C3E8.3:
	.word	1
	.word	2
	.word	4
	.word	8
	.word	16
	.word	0
	.align	2
.LC0:
	.string	"hlL"
	.text
	.align	2
	.globl	func_8005E714
	.type	 func_8005E714,@function
	.ent	func_8005E714
func_8005E714:
	.frame	$sp,136,$31		# vars= 88, regs= 8/0, args= 16, extra= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,136
	sw	$20,120($sp)
	move	$20,$4
	sw	$19,116($sp)
	move	$19,$5
	move	$5,$6
	sw	$22,128($sp)
	li	$22,240			# 0x000000f0
	sw	$21,124($sp)
	li	$21,37			# 0x00000025
	sw	$31,132($sp)
	sw	$18,112($sp)
	sw	$17,108($sp)
	sw	$16,104($sp)
	sw	$7,148($sp)
	sw	$0,60($sp)
.L4:
	move	$18,$5
	move	$4,$0
.L7:
	addu	$2,$18,$4
	lbu	$16,0($2)
	#nop
	andi	$2,$16,0x00f0
	.set	noreorder
	.set	nomacro
	bne	$2,$22,.L8
	addu	$4,$4,1
	.set	macro
	.set	reorder

	addu	$2,$18,$4
	addu	$4,$4,1
	lbu	$3,0($2)
	sll	$2,$16,8
	or	$16,$3,$2
.L8:
	andi	$2,$16,0xffff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L112
	subu	$17,$18,$5
	.set	macro
	.set	reorder

	addu	$18,$18,$4
	.set	noreorder
	.set	nomacro
	bne	$2,$21,.L7
	move	$4,$0
	.set	macro
	.set	reorder

	addu	$18,$18,-1
	subu	$17,$18,$5
.L112:
	.set	noreorder
	.set	nomacro
	blez	$17,.L12
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	$31,$20
	move	$6,$17
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,.L106
	li	$2,-1			# 0xffffffff
	.set	macro
	.set	reorder

	lw	$2,60($sp)
	#nop
	addu	$2,$2,$17
	sw	$2,60($sp)
.L12:
	andi	$2,$16,0xffff
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L15
	addu	$18,$18,1
	.set	macro
	.set	reorder

	lw	$2,60($sp)
	j	.L106
.L15:
	sw	$0,64($sp)
	la	$16,D_8014C3E0.2
.L16:
	lbu	$5,0($18)
	.set	noreorder
	.set	nomacro
	jal	func_80032D30
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L17
	subu	$2,$2,$16
	.set	macro
	.set	reorder

	sll	$2,$2,2
	lw	$3,64($sp)
	lw	$2,D_8014C3E8.3($2)
	addu	$18,$18,1
	or	$3,$3,$2
	.set	noreorder
	.set	nomacro
	j	.L16
	sw	$3,64($sp)
	.set	macro
	.set	reorder

.L17:
	lbu	$3,0($18)
	li	$2,42			# 0x0000002a
	.set	noreorder
	.set	nomacro
	bnel	$3,$2,.L20
	sw	$0,56($sp)
	.set	macro
	.set	reorder

	lw	$2,148($sp)
	li	$3,-4			# 0xfffffffc
	addu	$2,$2,3
	and	$2,$2,$3
	addu	$2,$2,4
	sw	$2,148($sp)
	lw	$2,-4($2)
	#nop
	.set	noreorder
	.set	nomacro
	bgez	$2,.L21
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	lw	$3,64($sp)
	subu	$2,$0,$2
	sw	$2,56($sp)
	ori	$3,$3,0x0004
	sw	$3,64($sp)
.L21:
	.set	noreorder
	.set	nomacro
	j	.L22
	addu	$18,$18,1
	.set	macro
	.set	reorder

.L20:
.L23:
	lbu	$4,0($18)
	jal	func_8005E708
	beq	$2,$0,.L22
	lw	$3,56($sp)
	#nop
	slt	$2,$3,999
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L25
	sll	$2,$3,2
	.set	macro
	.set	reorder

	addu	$2,$2,$3
	lbu	$3,0($18)
	sll	$2,$2,1
	addu	$2,$2,$3
	addu	$2,$2,-48
	sw	$2,56($sp)
.L25:
	.set	noreorder
	.set	nomacro
	j	.L23
	addu	$18,$18,1
	.set	macro
	.set	reorder

.L22:
	lbu	$3,0($18)
	li	$2,46			# 0x0000002e
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L28
	li	$2,-1			# 0xffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L29
	sw	$2,52($sp)
	.set	macro
	.set	reorder

.L28:
	addu	$18,$18,1
	lbu	$3,0($18)
	li	$2,42			# 0x0000002a
	.set	noreorder
	.set	nomacro
	bnel	$3,$2,.L30
	sw	$0,52($sp)
	.set	macro
	.set	reorder

	lw	$2,148($sp)
	li	$3,-4			# 0xfffffffc
	addu	$2,$2,3
	and	$2,$2,$3
	addu	$2,$2,4
	sw	$2,148($sp)
	lw	$2,-4($2)
	addu	$18,$18,1
	.set	noreorder
	.set	nomacro
	j	.L29
	sw	$2,52($sp)
	.set	macro
	.set	reorder

.L30:
.L32:
	lbu	$4,0($18)
	jal	func_8005E708
	beq	$2,$0,.L29
	lw	$3,52($sp)
	#nop
	slt	$2,$3,999
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L34
	sll	$2,$3,2
	.set	macro
	.set	reorder

	addu	$2,$2,$3
	lbu	$3,0($18)
	sll	$2,$2,1
	addu	$2,$2,$3
	addu	$2,$2,-48
	sw	$2,52($sp)
.L34:
	.set	noreorder
	.set	nomacro
	j	.L32
	addu	$18,$18,1
	.set	macro
	.set	reorder

.L29:
	lbu	$5,0($18)
	la	$4,.LC0
	jal	func_80032D30
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L37
	move	$2,$0
	.set	macro
	.set	reorder

	lbu	$2,0($18)
	addu	$18,$18,1
.L37:
	addu	$4,$sp,16
	addu	$5,$sp,148
	sb	$2,68($sp)
	lbu	$6,0($18)
	.set	noreorder
	.set	nomacro
	jal	func_8005E414
	addu	$7,$sp,72
	.set	macro
	.set	reorder

	lw	$3,28($sp)
	lw	$2,40($sp)
	#nop
	addu	$3,$3,$2
	lw	$2,32($sp)
	lw	$4,44($sp)
	addu	$3,$3,$2
	addu	$3,$3,$4
	lw	$2,36($sp)
	lw	$4,48($sp)
	addu	$3,$3,$2
	addu	$3,$3,$4
	lw	$4,56($sp)
	lw	$2,64($sp)
	subu	$4,$4,$3
	andi	$2,$2,0x0004
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L39
	sw	$4,56($sp)
	.set	macro
	.set	reorder

	blez	$4,.L39
	move	$17,$4
	sltu	$2,$17,21
.L114:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L45
	li	$16,20			# 0x00000014
	.set	macro
	.set	reorder

	move	$16,$17
.L45:
	.set	noreorder
	.set	nomacro
	blezl	$16,.L113
	subu	$17,$17,$16
	.set	macro
	.set	reorder

	move	$4,$19
	la	$5,D_8013B62C
	.set	noreorder
	.set	nomacro
	jal	$31,$20
	move	$6,$16
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,.L107
	subu	$17,$17,$16
	.set	macro
	.set	reorder

	lw	$2,60($sp)
	#nop
	addu	$2,$2,$16
	sw	$2,60($sp)
.L113:
	.set	noreorder
	.set	nomacro
	bgtz	$17,.L114
	sltu	$2,$17,21
	.set	macro
	.set	reorder

.L39:
	lw	$6,28($sp)
	#nop
	.set	noreorder
	.set	nomacro
	blez	$6,.L51
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	$31,$20
	addu	$5,$sp,72
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,.L106
	li	$2,-1			# 0xffffffff
	.set	macro
	.set	reorder

	lw	$2,60($sp)
	lw	$3,28($sp)
	#nop
	addu	$2,$2,$3
	sw	$2,60($sp)
.L51:
	lw	$2,40($sp)
	#nop
	blez	$2,.L54
	move	$17,$2
	sltu	$2,$17,21
.L116:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L59
	li	$16,20			# 0x00000014
	.set	macro
	.set	reorder

	move	$16,$17
.L59:
	.set	noreorder
	.set	nomacro
	blezl	$16,.L115
	subu	$17,$17,$16
	.set	macro
	.set	reorder

	move	$4,$19
	la	$5,D_8013B644
	.set	noreorder
	.set	nomacro
	jal	$31,$20
	move	$6,$16
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,.L107
	subu	$17,$17,$16
	.set	macro
	.set	reorder

	lw	$2,60($sp)
	#nop
	addu	$2,$2,$16
	sw	$2,60($sp)
.L115:
	.set	noreorder
	.set	nomacro
	bgtz	$17,.L116
	sltu	$2,$17,21
	.set	macro
	.set	reorder

.L54:
	lw	$6,32($sp)
	#nop
	blez	$6,.L65
	lw	$5,24($sp)
	.set	noreorder
	.set	nomacro
	jal	$31,$20
	move	$4,$19
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,.L106
	li	$2,-1			# 0xffffffff
	.set	macro
	.set	reorder

	lw	$2,60($sp)
	lw	$3,32($sp)
	#nop
	addu	$2,$2,$3
	sw	$2,60($sp)
.L65:
	lw	$2,44($sp)
	#nop
	blez	$2,.L68
	move	$17,$2
	sltu	$2,$17,21
.L118:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L73
	li	$16,20			# 0x00000014
	.set	macro
	.set	reorder

	move	$16,$17
.L73:
	.set	noreorder
	.set	nomacro
	blezl	$16,.L117
	subu	$17,$17,$16
	.set	macro
	.set	reorder

	move	$4,$19
	la	$5,D_8013B644
	.set	noreorder
	.set	nomacro
	jal	$31,$20
	move	$6,$16
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,.L107
	subu	$17,$17,$16
	.set	macro
	.set	reorder

	lw	$2,60($sp)
	#nop
	addu	$2,$2,$16
	sw	$2,60($sp)
.L117:
	.set	noreorder
	.set	nomacro
	bgtz	$17,.L118
	sltu	$2,$17,21
	.set	macro
	.set	reorder

.L68:
	lw	$6,36($sp)
	#nop
	.set	noreorder
	.set	nomacro
	blez	$6,.L79
	move	$4,$19
	.set	macro
	.set	reorder

	lw	$2,24($sp)
	lw	$5,32($sp)
	.set	noreorder
	.set	nomacro
	jal	$31,$20
	addu	$5,$2,$5
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,.L106
	li	$2,-1			# 0xffffffff
	.set	macro
	.set	reorder

	lw	$2,60($sp)
	lw	$3,36($sp)
	#nop
	addu	$2,$2,$3
	sw	$2,60($sp)
.L79:
	lw	$2,48($sp)
	#nop
	blez	$2,.L82
	move	$17,$2
	sltu	$2,$17,21
.L120:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L87
	li	$16,20			# 0x00000014
	.set	macro
	.set	reorder

	move	$16,$17
.L87:
	.set	noreorder
	.set	nomacro
	blezl	$16,.L119
	subu	$17,$17,$16
	.set	macro
	.set	reorder

	move	$4,$19
	la	$5,D_8013B644
	.set	noreorder
	.set	nomacro
	jal	$31,$20
	move	$6,$16
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,.L107
	subu	$17,$17,$16
	.set	macro
	.set	reorder

	lw	$2,60($sp)
	#nop
	addu	$2,$2,$16
	sw	$2,60($sp)
.L119:
	.set	noreorder
	.set	nomacro
	bgtz	$17,.L120
	sltu	$2,$17,21
	.set	macro
	.set	reorder

.L82:
	lw	$2,64($sp)
	#nop
	andi	$2,$2,0x0004
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L4
	addu	$5,$18,1
	.set	macro
	.set	reorder

	lw	$2,56($sp)
	#nop
	.set	noreorder
	.set	nomacro
	blez	$2,.L4
	move	$17,$2
	.set	macro
	.set	reorder

	sltu	$2,$17,21
.L122:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L99
	li	$16,20			# 0x00000014
	.set	macro
	.set	reorder

	move	$16,$17
.L99:
	.set	noreorder
	.set	nomacro
	blezl	$16,.L121
	subu	$17,$17,$16
	.set	macro
	.set	reorder

	move	$4,$19
	la	$5,D_8013B62C
	.set	noreorder
	.set	nomacro
	jal	$31,$20
	move	$6,$16
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,.L107
	subu	$17,$17,$16
	.set	macro
	.set	reorder

	lw	$2,60($sp)
	#nop
	addu	$2,$2,$16
	sw	$2,60($sp)
.L121:
	.set	noreorder
	.set	nomacro
	bgtz	$17,.L122
	sltu	$2,$17,21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L4
	addu	$5,$18,1
	.set	macro
	.set	reorder

.L107:
	li	$2,-1			# 0xffffffff
.L106:
	lw	$31,132($sp)
	lw	$22,128($sp)
	lw	$21,124($sp)
	lw	$20,120($sp)
	lw	$19,116($sp)
	lw	$18,112($sp)
	lw	$17,108($sp)
	lw	$16,104($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,136
	.set	macro
	.set	reorder

	.end	func_8005E714
.Lfe1:
	.size	 func_8005E714,.Lfe1-func_8005E714
	.align	2
	.globl	func_8005EC74
	.type	 func_8005EC74,@function
	.ent	func_8005EC74
func_8005EC74:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, extra= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	subu	$sp,$sp,32
	sw	$17,20($sp)
	move	$17,$4
	sw	$16,16($sp)
	sw	$31,24($sp)
	.set	noreorder
	.set	nomacro
	jal	func_80032D94
	move	$16,$6
	.set	macro
	.set	reorder

	addu	$2,$17,$16
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

	.end	func_8005EC74
.Lfe2:
	.size	 func_8005EC74,.Lfe2-func_8005EC74
	.align	2
	.globl	func_8005ECA8
	.type	 func_8005ECA8,@function
	.ent	func_8005ECA8
func_8005ECA8:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, extra= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,24
	sw	$16,16($sp)
	move	$16,$4
	move	$2,$5
	move	$7,$6
	la	$4,func_8005EC74
	move	$5,$16
	sw	$31,20($sp)
	.set	noreorder
	.set	nomacro
	jal	func_8005E714
	move	$6,$2
	.set	macro
	.set	reorder

	move	$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$3,.L125
	addu	$2,$16,$3
	.set	macro
	.set	reorder

	sb	$0,0($2)
.L125:
	lw	$31,20($sp)
	lw	$16,16($sp)
	move	$2,$3
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	func_8005ECA8
.Lfe3:
	.size	 func_8005ECA8,.Lfe3-func_8005ECA8
.section	.rodata
	.align	2
.LC1:
	.string	"\360\317\360\301\360\254\360\255"
	.text
	.align	2
	.globl	func_8005ECF8
	.type	 func_8005ECF8,@function
	.ent	func_8005ECF8
func_8005ECF8:
	.frame	$sp,544,$31		# vars= 520, regs= 2/0, args= 16, extra= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,544
	sw	$16,536($sp)
	move	$16,$4
	sw	$31,540($sp)
	la	$13,.LC1
	lwl	$10,0($13)
	lwr	$10,3($13)
	lwl	$11,4($13)
	lwr	$11,7($13)
	swl	$10,528($sp)
	swr	$10,531($sp)
	swl	$11,532($sp)
	swr	$11,535($sp)
	.set	noreorder
	.set	nomacro
	jal	func_8005ECA8
	addu	$4,$sp,16
	.set	macro
	.set	reorder

	move	$4,$0
	addu	$6,$sp,16
	lbu	$2,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L128
	move	$9,$16
	.set	macro
	.set	reorder

	li	$8,45			# 0x0000002d
	move	$7,$6
	addu	$3,$6,$4
.L157:
	lbu	$5,0($3)
	#nop
	addu	$2,$5,-48
	sltu	$2,$2,10
	.set	noreorder
	.set	nomacro
	beql	$2,$0,.L130
	andi	$3,$5,0x00ff
	.set	macro
	.set	reorder

	lbu	$2,528($sp)
	addu	$4,$4,1
	sb	$2,0($16)
	lbu	$2,529($sp)
	lbu	$3,0($3)
	addu	$16,$16,1
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	j	.L153
	addu	$2,$2,-48
	.set	macro
	.set	reorder

.L130:
	li	$2,32			# 0x00000020
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L132
	li	$2,43			# 0x0000002b
	.set	macro
	.set	reorder

	lbu	$2,530($sp)
	addu	$4,$4,1
	sb	$2,0($16)
	lbu	$2,531($sp)
	.set	noreorder
	.set	nomacro
	j	.L153
	addu	$16,$16,1
	.set	macro
	.set	reorder

.L132:
	bne	$3,$2,.L134
	lbu	$2,532($sp)
	addu	$4,$4,1
	sb	$2,0($16)
	lbu	$2,533($sp)
	.set	noreorder
	.set	nomacro
	j	.L153
	addu	$16,$16,1
	.set	macro
	.set	reorder

.L134:
	.set	noreorder
	.set	nomacro
	bne	$3,$8,.L136
	li	$2,10			# 0x0000000a
	.set	macro
	.set	reorder

	lbu	$2,534($sp)
	addu	$4,$4,1
	sb	$2,0($16)
	lbu	$2,535($sp)
	.set	noreorder
	.set	nomacro
	j	.L153
	addu	$16,$16,1
	.set	macro
	.set	reorder

.L136:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L138
	li	$2,35			# 0x00000023
	.set	macro
	.set	reorder

	sb	$5,0($16)
	.set	noreorder
	.set	nomacro
	j	.L154
	addu	$4,$4,1
	.set	macro
	.set	reorder

.L138:
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L141
	li	$2,64			# 0x00000040
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$2,.L140
	andi	$3,$5,0x00f0
	.set	macro
	.set	reorder

.L141:
	addu	$4,$4,1
	addu	$2,$6,$4
	sb	$5,0($16)
	lbu	$2,0($2)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$8,.L142
	addu	$16,$16,1
	.set	macro
	.set	reorder

	sb	$2,0($16)
	addu	$4,$4,1
	addu	$16,$16,1
.L142:
	addu	$2,$6,$4
	lbu	$2,0($2)
	#nop
	addu	$2,$2,-48
	sltu	$2,$2,10
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L144
	addu	$3,$4,$7
	.set	macro
	.set	reorder

.L145:
	lbu	$2,0($3)
	addu	$3,$3,1
	addu	$4,$4,1
	sb	$2,0($16)
	lbu	$2,0($3)
	#nop
	addu	$2,$2,-48
	sltu	$2,$2,10
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L145
	addu	$16,$16,1
	.set	macro
	.set	reorder

.L144:
	addu	$2,$7,$4
	lbu	$3,0($2)
	#nop
	addu	$2,$3,-97
	sltu	$2,$2,26
	.set	noreorder
	.set	nomacro
	bnel	$2,$0,.L155
	sb	$3,0($16)
	.set	macro
	.set	reorder

	addu	$2,$3,-65
	sltu	$2,$2,26
	.set	noreorder
	.set	nomacro
	beq	$2,$0,.L156
	addu	$6,$sp,16
	.set	macro
	.set	reorder

	sb	$3,0($16)
.L155:
	.set	noreorder
	.set	nomacro
	j	.L154
	addu	$4,$4,1
	.set	macro
	.set	reorder

.L140:
	li	$2,240			# 0x000000f0
	.set	noreorder
	.set	nomacro
	beq	$3,$2,.L150
	addu	$4,$4,1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	.L154
	sb	$5,0($16)
	.set	macro
	.set	reorder

.L150:
	sb	$5,0($16)
	addu	$16,$16,1
	addu	$2,$6,$4
	lbu	$2,0($2)
	addu	$4,$4,1
.L153:
	sb	$2,0($16)
.L154:
	addu	$16,$16,1
	addu	$6,$sp,16
.L156:
	addu	$2,$6,$4
	lbu	$2,0($2)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$0,.L157
	addu	$3,$6,$4
	.set	macro
	.set	reorder

.L128:
	subu	$2,$16,$9
	sb	$0,0($16)
	lw	$31,540($sp)
	lw	$16,536($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,544
	.set	macro
	.set	reorder

	.end	func_8005ECF8
.Lfe4:
	.size	 func_8005ECF8,.Lfe4-func_8005ECF8
